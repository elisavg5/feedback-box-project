# Feedback Box Project

## Description of the project 

This is a project realized during an internship at Handicap International. The aim of the project is to create a box in order to collect feedbacks from the beneficiaries of Handicap International. 
For now, the box is composed with 3 questions with 3 buttons for each question (9 buttons in total). 
The box may be placed in places without internet connectivity, or a performant network. The idea to avoid this is using LoRa technology : a wireless protocol designed for long-distance and low-power communication. 
We have implemented an emitter (the box), and a receiver (the gate) which will be connected to internet in order to put the data received into the database. 

# The Emitter

## Hardware Used 

For the external box we are using 3mm thick plywood (3D model : ). 
The arduino we use is a Arduino MKR Wan 1310 (https://docs.arduino.cc/hardware/mkr-wan-1310), with a SD module shield (https://tinyurl.com/MkrSdProtoShield), and an SD card. 
In addition, we are using : 
- 9 leds 
- 9 220 ohm resistor (for the leds)
- 9 buttons 
- 3 1 M ohm resistor (for the buttons) 
- 6 100 K ohm resistor (for the buttons) 
- 3 1 K resistor (for the buttons) 


## Electrical scheme 

The MKR Wan 1310 doesn't have enough available pin for 9 buttons and 9 leds, we had to make a voltage divider bridge in order to have 3 buttons on one analogic pin. 

The idea is these : \
![image](https://user-images.githubusercontent.com/72611004/171127881-566f6fe3-9946-4786-88b5-37507eb51a37.png) \
The tutorial to understand that : 
https://www.instructables.com/How-to-Multiple-Buttons-on-1-Analog-Pin-Arduino-Tu/

The final scheme for our project is incoming. 

## Arduino program (feedback_box_emitter)

In the emitter program we handle : 
+ sending packets with LoRa Technology 
+ differentiate 9 buttons on 3 analog pin 
+ light 9 leds depending on which button is selected 
+ store the data from buttons on a SD card 


# The Receiver 

## Hardware Used 
We use also an Arduino MKR WAN 1310, with an antenna. 

## Arduino program () 

In the receiver program we handle : 
+ receiving packets with LoRa Technology 
+ Split the packets on a separator ","
+ Put the data into the database (incoming) 


# HOW IT WORKS 

-> In the Arduino IDE, in card manager install the Arduino SAMD Boards 
-> Connect all the hardware needed in the MKRs (do not forget antennas) 
-> Upload feedback_box_emitter in the emitter MKR 
-> Upload r_ceiver in the receiver MKR 

# Features 

Software features : In the final version we would like to be able to change the arduino code via an application. 
Hardware features : speakers ? 

Enjoy ! 

