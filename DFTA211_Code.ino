  //  Arroseur Automatique, simple et rustique pour plante d'appartement.
  //  Le code est prévu pour un microcntroleur ATtiny85.
  //  Les sondes sont des sondes DIY nettoyables et interchangeables. Le code est fait en sorte qu'elles ne puissent (ou très peu) subir l'effet de corrosion galvanique.
  //  Sonde de référence : elle plantée dans le terreau du pot, elle est reliée directement sur PB1 c'est un pinMode OUTPUT qui est positionné à HIGH en dehors des mesures 
  //  et à LOW durant les mesures.
  //  Sonde de lecture : elle reliée sur A1 qui elle même est reliée à +5V via une résistance de 12K pour effectuer une mesure entre elle et la sonde de référence, comme
  //  cela si la sonde référence est positionnée à LOW, sur A1 je lis la division potentiométrique par le diviseur 12K/Résistance terreau.
  //  Le bouton sert à bypasser la tempo "periodicite" pour forcer ou tester une séquence d'arrosage, il intervient directement et matériellement sur PB5 (RESET)
  //  Le Buzzer sonne un coup long si OUI il faut arroser ou 6 coups courts si il ne faut pas arroser.
  //  la séquence d'arrossage est constituée de 3 arrosages successifs (si nécessaires) avec durée d'arrosage dégressive et temps de pause entre chaque test pour laisser
  //  apparaître l'efficacité de l'arrosage. Le buzzer sonne 2 coups courts en fin de séquence de commande pompe
  //  Matériel utilisé :
  //                  1 ATtiny85
  //                  1 micropompe 5V + tuyau souple
  //                  1 BP Normalement ouvert (NO)
  //                  1 Diode de roue libre à brancher en inverse sur la sortie alimentant le moteur
  //                  1 Diode LED
  //                  1 Buzzer 5V (facultatif)
  //                  1 Résistance de 12K
  //                  1 Résistance de 470 ohms
  //                  1 Résistance de 150 ohms
  //                  1 Condensateur de 100nF
  //                  1 Condensateur de 150nF
  //                  1 Bloc convertisseur 5V
  //                  1 Connecteur Barrel Jack
  //                  2 Plaquettes pastillées montées en sandwich
  //                  1 optocoupleur mosfet ASSR311 mais on peut aussi bien monter un transitor BJT ou mosfet tout dépend de ce que l'on a sous la main.
  //
  //  ==========================================================================  CLAUDE DUFOURMONT ========================================================================
  //
  //  ..................................................... Ma chaîne YouTube : https://www.youtube.com/c/ClaudeDufourmont .................................................
  //
  //  ............................................................ Mon GitHub : https://github.com/claudiusmarius ..........................................................
  //
  //  ......................  Vidéo YouTube sur ce sujet :  https://youtu.be/836giWG3lic
  //
  //  ...................................... GitHub sur ce sujet : https://github.com/claudiusmarius/ARROSEUR-AUTOMATIQUE-NEW-CONCEPT-DFTA211  .............................
  //
  //  ======================================================================================================================================================================
  //
  //  *****************************************  Pour la programmation des ATtiny85 je vous conseille les 2 vidéos YouTube ci dessous :  ***********************************  
  //
  //  DFT_#A91 PLATINE DE DEVELOPPEMENT POUR ATTINY85-NEW CONCEPT : https://youtu.be/x3gs_hjUjcw
  //
  //  DFT_#A16 ATTINY85 PROGRAMMER : https://youtu.be/1XOWKyjqMR4
  //
  //  ======================================================================================================================================================================
  
  #define BrochesondeLecture A1        // PB2
  #define BrochesondeReference 1       // PB1
  #define brocheLActionPompe 0         // PB0
  #define BrocheBuzzer 3               // PB3

  unsigned long periodicite = 86400000; // 24H Il s'agit du laps de temps entre 2 mesures, lorsque la fonction mesure n'est pas activée la DDP entre les 2 sondes est nulle.

  void setup() 
  {
  // Forçage à l'arrêt de la pompe pendant le démarrage
  pinMode(brocheLActionPompe, OUTPUT);
  digitalWrite(brocheLActionPompe, LOW);

  // Les 4 lignes ci-dessous ont pour objectif d'annuler la DDP entre les 2 sondes (rappel la sonde de mesure est reliée au +5C via une réistance de 12K)
  pinMode(BrochesondeReference, OUTPUT);
  digitalWrite(BrochesondeReference, HIGH);
  pinMode(BrochesondeLecture, OUTPUT);
  digitalWrite(BrochesondeLecture, HIGH);
  
  //  Animation sonore spécifique de démarrage
  pinMode(BrocheBuzzer, OUTPUT);
  Buzzer(300, 50, 10);
  }

  void loop() 
  {
  //  renvoi vers la fonction mesure puis activation de la tempo entre 2 mesures (temps long)
  mesure();               // Lance la séquence de 3 tests/arrosages
  delay(periodicite);     // Attente avant prochaine séquence
  }

  //  ======================================================================= DEBUT FONCTION MESURE ==========================================================================
  void mesure() 
  {
  const uint16_t seuilSec = 300;  //............................................  Seuil en desous duquel on n'arrose plus
  const unsigned long durees[3] = {12000, 3000, 1000};  //......................  3 durées dégressives d'arrosage 
  const unsigned long attenteApresArrosage = 8000;  //..........................  Cette durée a pour but d'attendre l'efficacité de l'arrosage sur la conductivité du terreau

  for (int i = 0; i < 3; i++) 
  {
  // Préparation de la lecture : La sonde de référence est placée à zéro volts et la sonde de lecture est prête
  pinMode(BrochesondeReference, OUTPUT);
  digitalWrite(BrochesondeReference, LOW);
  pinMode(BrochesondeLecture, INPUT);
  delay(10);
  
  // Lecture moyenné de la tension résultante du pont diviseur constitué de R3 (12K) et de la résistance du terreau (entre les 2 sondes)
  uint16_t Tension = 0;
  for (int j = 0; j < 5; j++) 
  {
  Tension += analogRead(BrochesondeLecture);
  delay(5);
  }
  Tension /= 5;
  delay(2000);
  
  // Remise en configuration DDP nulle entre les sondes
  pinMode(BrochesondeLecture, OUTPUT);
  digitalWrite(BrochesondeLecture, HIGH);
  digitalWrite(BrochesondeReference, HIGH);

  // Test humidité, l'opération est réitérée sur les 3 durées dégressives
  if (Tension > seuilSec) 
  {
  Buzzer(600, 50, 1); //.............................................................  Début arrosage
  digitalWrite(brocheLActionPompe, HIGH);
  delay(durees[i]);
  digitalWrite(brocheLActionPompe, LOW);
  Buzzer(200, 200, 2); //............................................................  Fin test
  } 
    
  else 
  {
  Buzzer(50, 50, 6); //.............................................................   Pas d’arrosage nécessaire
  }

  delay(attenteApresArrosage);
  }
  }
  //  ========================================================================= FIN FONCTION MESURE ==========================================================================
  
  
  //  ======================================================================== DEBUT FONCTION BUZZER =========================================================================
  void Buzzer(int TempsH, int TempsL, int nb) 
  {
  for (int i = 0; i < nb; i++) 
  {
  digitalWrite(BrocheBuzzer, HIGH);
  delay(TempsH);
  digitalWrite(BrocheBuzzer, LOW);
  delay(TempsL);
  }
  }
  //  ========================================================================== FIN FONCTION BUZZER ==========================================================================