# 2425_ESE_Tom&Jerry

## Sommaire
- [Présentation](#présentation)
- [Mécanique](#1-mécanique)
- [Électronique](#2-électronique)
- [Informatique](#3-informatique)
- [Stratégies](#4-stratégies)
- [Installation](#installation)
- [Contribution](#contribution-)
- [Auteurs et Contact](#auteurs-)

---

## Présentation

>Bienvenue sur le dépôt GitHub de **Tom&Jerry**. Ce projet a pour but de développer un robot autonome qui sur un plan plat joue à chat. Le plan est sans rebord, les matchs de 60secondes et le robot change de rôle (Chat/Souris) lors des contacts avec le(s) autre(s) robot(s).

>Ce projet est organisé en trois sous-domaines principaux :
>* **Mécanique** : Conception et fabrication des structures et composants mécaniques.
>* **Électronique** : Conception des circuits électroniques et sélection des composants.
>* **Informatique** : Développement du logiciel et de l'algorithme pour le contrôle du système.
>* **Stratégies** : Focus sur les stratégies mises en place pour s'échapper ou capturer.

---

## Détails du Projet

### 1. Mécanique
>Cette section est dédiée à la conception et à la fabrication des parties mécaniques du projet. 

#### Description :
- **Modélisation CAO** : Les pièces du projet ont été modélisées en utilisant **OnShape**.
- **Matériaux** : Les matériaux utilisés incluent **Plastique PLA, ...**.
- **Assemblage** : Le système est assemblé à l’aide de **(ex. vis, écrous, etc.)** et monté selon les plans fournis dans le dossier **/mechanical_drawings**.

#### Fichiers et Dossiers :
>* **/mechanical_drawings/** : Contient les fichiers CAO et les plans techniques.
>* **/materials_list.txt** : Liste des matériaux et composants utilisés.

---

### 2. Électronique
>Cette partie couvre la conception et le développement des circuits électroniques nécessaires pour alimenter et contrôler le système.

#### Description :
>* **Schémas électroniques** : Conception des circuits avec **Kicad**.
>* **Liste des composants** : Utilisation de microcontrôleurs comme **STM32**, capteurs **TOF, Capteur fin de course, Lidar, IMU, Odom**, et actionneurs **Moteurs de roues**.
>* **PCB** : Un circuit imprimé (PCB) a été conçu pour **Alimentation & Décision du robot**.

#### Fichiers et Dossiers :
>* **electronique/Tom&Jerry_PCB/** : Projet Kicad du PCB principal.
>* **electronique/TOF_PCb/** : Projet Kicad du PCB annexe pour les TOF.
>* **[Read.me ToF](electronique/READMETom&Jerry.md)**
>* **[Read.me Tom&Jerry](electronique/READMETom&Jerry.md)**

---

### 3. Informatique
>Cette section inclut le code source et les algorithmes permettant de contrôler le système.  

#### Description :
>* **Langages utilisés** : Le logiciel a été développé en **C**.  
>* **Architecture logicielle** : L'architecture du programme suit un modèle **/programm_diagramm**.  
>* **Modules et fonctionnalités** : Le logiciel gère **(ex. les capteurs, la communication avec le microcontrôleur, les algorithmes de contrôle**.  

#### Fichiers et Dossiers :
>* **/src/** : Contient le code source du projet.  
>* **/docs/** : Documentation du code et guide d’utilisation.  
>* **/scripts/** : Scripts auxiliaires pour tester ou déployer le logiciel.  
>* **[Read.me](software/README.md)**
---

### 4. Stratégies
>Cette section inclut tout ce qu'il faut savoir sur les stratégies décidées et mis en oeuvre.  

>[!Note]
> Pour faire fonctionner le robot plusieurs stratégies peuvent être mises en place.   
> **Lorsque nous sommes souris**  
>* 1. On fait avancer le robot sur des stratégies prédéfinies et on espère ne pas tomber sur le chat.  
>* 2. On donne des trajectoires aléatoires au robot et on espère ne pas tomber sur le chat ou de la table.  
>* 3. On donne des trajectoires aléatoires en évitant le chat en priant pour ne pas tomber de la table.  
>* 4. On utilise un champ de potentiel pour se repérer et éviter le chat et les bordures.  
>  
> **Lorsque nous sommes Chat**  
>* 1. On fait avancer le robot sur des stratégies prédéfinies et on espère toucher une souris.   
>* 2. On donne des trajectoires aléatoires au robot et on espère toucher une souris sans tomber.  
>* 3. Le Lidar donne l'endroit ou on fonce a pleine balle.  
>* 4. Le lidar donne l'endroit ou on fonce a pleine balle en prenant en compte que l'objectif bouge.   

---

## Installation

### Prérequis :
>[!Important]
>* **Matériel** : La liste du matériel nécessaire ce situe dans le fichier BOM ici, ainsi que dans la partie mécanique ici.
>* **Logiciel** : Nous utilisons STM32 CubeIDE pour programmer notre microcontrolleur. De plus pour la simulation le python est utilisé. 
>* **Connaissance** : Pour comprendre plainement le projet il est nécessaire de maitriser (Kicad, STM32CubeIDE, Onshape). Avec une idée de comment marche les microcontrolleur STM32. (Un ensearque a réussi vous devriez aussi y parvenir).

---

### Instructions :

>[!Tip]
> **Pour utiliser le projet sur un robot**  

1. Cloner le dépôt : 
   ```bash
   git clone https://github.com/JolanTho/2425_ESE_Projet_Tom_et_Jerry.git
   cd projet
   
>[!Tip]
> **Pour utiliser la simulation de champs potentiel**  

1. Cloner le dépôt : 
   ```bash
   git clone https://github.com/JolanTho/2425_ESE_Projet_Tom_et_Jerry.git
   cd projet
---

## Contribution :

---

## Auteurs : 
>* Jolan Thomasset (jolan.thomasset@ensea.fr)  
>* Romain Pace   (romain.pace@ensea.fr)  
>* Rémi Weidle   (remi.weidle@ensea.fr)  

