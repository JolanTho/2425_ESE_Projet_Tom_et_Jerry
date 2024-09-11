# 2425_ESE_Tom&Jerry

## Sommaire
- [Présentation](#présentation)
- [Mécanique](#mécanique)
- [Électronique](#électronique)
- [Informatique](#informatique)
- [Stratégies](#stratégies)
- [Installation](#installation)
- [Contribution](#contribution)
- [Auteurs et Contact](#auteurs-et-contact)

---

## Présentation

Bienvenue sur le dépôt GitHub de **Tom&Jerry**. Ce projet a pour but de développer un robot autonome qui sur un plan plat joue à chat. Le plan est sans rebord, les matchs de 60secondes et le robot change de rôle (Chat/Souris) lors des contacts avec le(s) autre(s) robot(s).

Ce projet est organisé en trois sous-domaines principaux :
- **Mécanique** : Conception et fabrication des structures et composants mécaniques.
- **Électronique** : Conception des circuits électroniques et sélection des composants.
- **Informatique** : Développement du logiciel et de l'algorithme pour le contrôle du système.
- **Stratégies** : Focus sur les stratégies mises en place pour s'échapper ou capturer.

---

## Détails du Projet

### 1. Mécanique
Cette section est dédiée à la conception et à la fabrication des parties mécaniques du projet. 

#### Description :
- **Modélisation CAO** : Les pièces du projet ont été modélisées en utilisant **OnShape**.
- **Matériaux** : Les matériaux utilisés incluent **Plastique PLA, ...**.
- **Assemblage** : Le système est assemblé à l’aide de **(ex. vis, écrous, etc.)** et monté selon les plans fournis dans le dossier **/mechanical_drawings**.

#### Fichiers et Dossiers :
- **/mechanical_drawings/** : Contient les fichiers CAO et les plans techniques.
- **/materials_list.txt** : Liste des matériaux et composants utilisés.

---

### 2. Électronique
Cette partie couvre la conception et le développement des circuits électroniques nécessaires pour alimenter et contrôler le système.

#### Description :
- **Schémas électroniques** : Conception des circuits avec **Kicad**.
- **Liste des composants** : Utilisation de microcontrôleurs comme **STM32**, capteurs **TOF, Capteur fin de course, Lidar, IMU, Odom**, et actionneurs **Moteurs de roues**.
- **PCB** : Un circuit imprimé (PCB) a été conçu pour **Alimentation & Décision du robot**.

#### Fichiers et Dossiers :
- **/schematics/** : Contient les schémas électroniques.
- **/pcb_design/** : Fichiers Gerber et autres fichiers pour la fabrication du PCB.
- **/components_list.txt** : Liste des composants électroniques (BOM).

---

### 3. Informatique
Cette section inclut le code source et les algorithmes permettant de contrôler le système.

#### Description :
- **Langages utilisés** : Le logiciel a été développé en **C**.
- **Architecture logicielle** : L'architecture du programme suit un modèle **/programm_diagramm**.
- **Modules et fonctionnalités** : Le logiciel gère **(ex. les capteurs, la communication avec le microcontrôleur, les algorithmes de contrôle**.

#### Fichiers et Dossiers :
- **/src/** : Contient le code source du projet.
- **/docs/** : Documentation du code et guide d’utilisation.
- **/scripts/** : Scripts auxiliaires pour tester ou déployer le logiciel.

---

### 4. Stratégies
Cette section inclut tout ce qu'il faut savoir sur les stratégies décidées et mis en oeuvre.
---

## Installation

### Prérequis :
- **Matériel** : Liste du matériel nécessaire (ex. microcontrôleur, capteurs).
- **Logiciel** : Liste des logiciels et outils à installer (ex. IDE, bibliothèques, compilateurs).

### Instructions :
1. Cloner le dépôt : 
   ```bash
   git clone https://github.com/username/projet.git
   cd projet
   
## Contribution :

## Auteurs : 
Jolan Thomasset (jolan.thomasset@ensea.fr)
Romain Pace   (romain.pace@ensea.fr)
Rémi Weidle   (remi.weidle@ensea.fr)

