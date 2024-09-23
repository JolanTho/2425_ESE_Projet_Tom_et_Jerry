# Partie éléctronique 

## Sommaire
- [Présentation](#présentation)
- [Alimentation](#alimentation)

- [Auteurs et Contact](#auteurs-et-contact)

---

## Présentation
Bienvenue dans la partie éléctronique du projet, dans ce dossier sont présent tous les fichiers de conception du PCB KiCad ainsi que tous les fichiers de fabrications Gerber.
Il est détaillé chaque composante du Schématique/PCb dans ce Read.me

## Alimentation
Pour notre robot nous utilisons plusieurs système pour gérer le circuit d'alimentation. On a entre autre un BMS ( Batterie système management ), ainsi qu'un pseudo Power Path Management. La batterie utilisée est la suivante : (DOC)

Des connecteurs Pac-Man sont utilisés au niveau de la recharge de batterie afin de pouvoir déconnecter facilement le circuit de charge si il est foireux. De plus un interrupteur est ajouté pour allumer/Eteindre le robot ainsi qu'un fusible pour protéger des surcourants sur la batterie. 

- **Recharge de batterie**

    [[Capture]]

    Pour la recharge de la batterie nous prévoyons deux connecteurs : Type-C et Bornier afin de pouvoir recharger la batterie par alimentation stabilisée de labo ainsi que par cable USB-C (Plus difficile à mettre en oeuvre). 

    [[Capture]]
    
    Cette entrée d'energie est gérée par le BMS (BQ25172DSGR de TI). Celui-ci est cablé tel quel : 
        - IN : Entrée de tension de charge de batterie. Max 0.8 A (Cf : Doc)
        - VSET : Programme le nombre de cellule --> 6 cellules + Charge intermittente
        - ISET : Programme le courant de charge --> 0.8A 
        - TMR : Programme le Timer de sureté --> 6Hr
        - TS : Programme la température de sureté de charge --> Donnée constructeur  
        - OUT : Sortie de tension de charge de la batterie
        - STAT : Sortie d'indication de l'état de charge à drain ouvert
        - EXP : Ground (Pad thermique)
        - GND : Ground

    [[Capture]]

    On a un transistor à canal P pour bloquer la charge du circuit par batterie lorsqu'une tension vient du bornier ou de l'USB-C. De plus on ajoute des condensateur (C...) pour eviter les retours de courant dû a des freinage des roues dans la résistance et donc dans l'alimentation derrière. 

- **Convertisseur Buck (MP1475DJ-LF-P)**
    [[Capture]]
    On utilise un premier convertisseur Buck pour abaisser la tension à 5V. Celui-ci est à découpage donc plus cher mais possède un meilleur rendement et un plus faible échauffement. Cela est parfait puisqu'il alimente le lidar ainsi que tous les circuits 3.3v derrière et donc peut avoir des appels de courant assez élevé (Comme démarrage de lidar). 
        - IN : Tension d'entrée
        - BST : Bootstrap
        - SWT : Sortie de commutation
        - PG : Sortie de puissance
        - VCC : LDO interne de 5v
        - EN/SYNC : Pour les clocks externes
        - FB : Feedback
        - GND : Ground

    Actuellement les données sont celle ce la doc. La seule chose réglée est le pont diviseur avec une Vref = 1.25V pour avoir une sortie de 5V en sortie. 

   - **Convertisseur LDO (BU33SD5WG-TR)**
    [[Capture]] 
    Simple convertisseur linéaire qui abaisse de 5V vers 3.3V pour alimenter certains capteurs ou encore la STM32 qui n'accepte pas du 5V. L'utilisation d'un buck serai plus utile si on recherche la performance cependant un LDO est suffisant car moins couteux, plus simple à mettre en place. L'abaissement de tension est faible sur de faible courants par conséquent la dissipation thermique sera faible ce qui justifie un LDO plutôt qu'un buck. 
        - VIN : Tension d'entrée
        - GND : Ground
        - STBY : StandBy --> Ici sur High il marche dés qu'il est alimenté. 
        - N. C. : No Connected pin
        - Vout : Tension de sortie