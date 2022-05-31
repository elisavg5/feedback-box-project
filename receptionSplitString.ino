#include <SPI.h>
#include <LoRa.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 30

//Data Model--> 1,3,35!2,2,36!1,2,37 avec question,answer,seqNb and ! as a data separator 

int question;
int answer;
int seqNb;
String input;


void setup() {
  // debug LoRa
  Serial.begin(9600);
  while (!Serial);
  //Wire.begin();
  Serial.println("LoRa Receiver"); 
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
    }
}

//to get different datas
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Received packet ");

    // read packet
    while (LoRa.available()) {
      input += (char)LoRa.read();
      //Serial.println(input);
    
      //split thanks '!' between two datas sessions
      int found = input.indexOf('!');
      if(found != -1)
      {

      //get datas : question, answer ant sequence number
      String quest = getValue(input, ',', 0);
      String ans = getValue(input, ',', 1);
      String seq = getValue(input, ',', 2);
      
      //string to int data
      question = quest.toInt();
      answer = ans.toInt();
      seqNb = seq.toInt();
      
      //print datas
      /*Serial.print("question int is ");
      Serial.println(question);
      Serial.print(",   answer int is ");
      Serial.println(answer);
      Serial.print("sequence Number int is ");
      Serial.println(seqNb);*/

      found = -1;
      input = " ";
      }
    }
    
    
    // print RSSI of packet
    /*Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());*/
  }
}
