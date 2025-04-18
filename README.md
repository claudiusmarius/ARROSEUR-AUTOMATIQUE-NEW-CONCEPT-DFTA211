# ARROSEUR-AUTOMATIQUE-NEW-CONCEPT-DFTA211

Ce système est rustique est très simple.
Evitons de tomber dans le gadget inutile.
Jusque maintenant je n’avais pas encore eu le besoin de d’arroseur automatique mais un ami m’a offert une très jolie plante d’intérieur et voyant les ponts du mois de mai arriver à grand pas, j’ai ressenti le besoin de développer ce système.
Je disposais déjà d’une mini pompe 5V DC de microcontrôleurs type ATtiny85, de connecteurs Barrel jack, de circuits pastillés et de blocs convertisseur 220V/5V DC.
J’ai tenu à fabriquer moi-même les sondes à partir de matériau que l’on trouve facilement dans les magasins de bricolage, pour que l’on puisse considérer celles-ci comme du consommable sachant que sur ces systèmes le point faible est la corrosion galvanique des sondes. J’ai voulu combattre au mieux ce phénomène en limitant au maximum la différence de potentiel entre les 2 sondes hors des phases de mesure (delta V = 0 volt) et limitant au maximum la durée et en augmentant la périodicité entre les mesures. Cela est très bien expliqué en commentaire dans le code de l’ATtiny 85.
J’ai prévu un bouton poussoir de forçage de la mesure pour me rassurer avant mon départ. L’ordre de grandeur de la périodicité sera de un ou plusieurs jours (l’expérience montre que je n’arrose pas ma plante tous les jours, bien au contraire).
Nous verrons à l’usage la robustesse de ce dispositif : vieillissement des sondes, fiabilité contre ordres intempestifs et les refus d’ordres. Peut nous irons en cas de succès vers la fabrication d’un petit PCB.
