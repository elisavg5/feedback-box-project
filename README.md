# Projet Feedback Box

## Description du projet 

Il s'agit d'un projet réalisé dans le cadre d'un stage chez Handicap International. Le but de ce projet est de créer une boîte afin de recueillir les retours des bénéficiaires de Handicap International. 
Pour l'instant, la boîte est composée de 3 questions avec 3 boutons pour chaque question (9 boutons au total). 
La boîte peut être placée dans des endroits sans connexion internet, ou sans réseau performant. L'idée pour pallier à cela est d'utiliser la technologie LoRa : un protocole sans fil conçu pour les communications à longue distance et à faible puissance. 
Nous avons implémenté un émetteur (la boîte), et un récepteur (le Raspberry) qui sera connecté à internet afin de mettre les données reçues dans la base de données. 

# L'émetteur

## Matériel utilisé 

Pour la boîte, nous utilisons du contreplaqué de 3mm d'épaisseur (modèle pour la découpe laser dans la branche dft_box). 
L'arduino que nous utilisons est un Arduino MKR Wan 1310 (https://docs.arduino.cc/hardware/mkr-wan-1310), avec un module SD shield (https://tinyurl.com/MkrSdProtoShield), et une carte SD. 
De plus, nous utilisons : 
- 9 leds 
- 9 résistances de 220 ohms (pour les leds)
- 9 boutons 
- 3 résistances de 1 M ohm (pour les boutons) 
- 6 résistances de 100 K ohm (pour les boutons) 
- 3 résistances de 1 K (pour les boutons) 


## Schéma électrique 

Le MKR Wan 1310 n'a pas assez de broches disponibles pour 9 boutons et 9 leds, nous avons dû faire un pont diviseur de tension afin d'avoir 3 boutons sur une broche analogique. 

L'idée est la suivante : \
![image](https://user-images.githubusercontent.com/72611004/171127881-566f6fe3-9946-4786-88b5-37507eb51a37.png) \
Le tutoriel pour comprendre cela : 
https://www.instructables.com/How-to-Multiple-Buttons-on-1-Analog-Pin-Arduino-Tu/


## Programme Arduino (boîte à rétroaction_émetteur)

Dans le programme de l'émetteur, nous gérons : 
+ l'envoi de paquets avec la technologie LoRa 
+ différencier 9 boutons sur 3 broches analogiques 
+ allumer 9 leds en fonction du bouton sélectionné 
+ stocker les données des boutons sur une carte SD 


# Le Récepteur 

## Matériel utilisé 
Nous utilisons un Raspberry Pi 3 avec un Dragino Lora Hat v1.4. 

## Programme du Raspberry () 

Le programme du récepteur est divisé en deux : 
- un client recevant les données de lora avec un programme c. 
  . le client est basé sur le câblage du pi http://wiringpi.com/ et le master de la librairie rpi-lora-tranceiver https://github.com/dragino/rpi-lora-tranceiver/ 
- un serveur poussant les données avec un programme python 
  . le serveur utilise pyodbc et donc unixodbc freetds ... 


# COMMENT ÇA MARCHE 

## Arduino
-> Dans l'IDE Arduino, dans le gestionnaire de cartes, installez les cartes Arduino SAMD. \
-> Connectez tout le matériel nécessaire dans les MKRs (n'oubliez pas les antennes) \
-> Uploader feedback_box_emitter dans le MKR émetteur. 
 
## Raspberry 
$ sudo apt-get install git \
$ sudo raspi-config \
Activer SPI sur le Raspberry Pi : Choisir Interface Option, puis SPI, il faut l’autoriser (enable : yes), valider avec OK \
$ cd /tmp \
$ wget https://project-downloads.drogon.net/wiringpi-latest.deb \
$ sudo dpkg -i wiringpi-latest.deb \
Vous pouvez vérifier l’installation avec :  \
$ gpio –v 

### Télécharger le code server 
$ wget https://codeload.github.com/elisavg5/feedback-box-project/zip/raspb_receipt  \
$ unzip raspb_receipt \
$ sudo apt-get install unixodbc unixodbc-dev freetds-dev freetds-bin tdsodbc   

### Configuration fichiers ODBC  
On bouge les documents odbcinst.ini et odbc.ini dans le dossier /etc/  \
$ sudo mv /home/hi-raspberry/Documents/feedback-box-server-raspb_receipt/odbcinst.ini /etc/ \
$ sudo mv /home/hi-raspberry/Documents/feedback-box-server-raspb_receipt/odbc.ini /etc/ 
  
$ pip install pyodbc  

On va dans le dossier du serveur \
cd Documents/feedback-box-server-raspb_receipt/server 

On exécute le serveur \
$ python3 server.py 

### Configurer le récepteur Client 
On va dans le dossier feedback-box-project-raspb/receipt/rpi-lora-tranceiver-master/dragino_lora_app \
$ chmod +x dragino_lora_app \
$ make 

On exécute le code client \
$ sudo ./dragino_lorap_app rec 



# Caractéristiques 

Caractéristiques du logiciel : Dans la version finale, nous aimerions pouvoir modifier le code arduino via une application. 
Caractéristiques matérielles : haut-parleurs ? 
