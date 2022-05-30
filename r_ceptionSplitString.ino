#include <SPI.h>
#include <LoRa.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 30

//Data Model--> 1,3,450!2,2,12!1,2,56 avec question,answer,seqNb and ! as a data separator 

int question;
int answer;
//int seqNb;
char* input;


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

   // Get next command from Serial (add 1 for final 0)
  //char input[INPUT_SIZE + 1];
  //byte taille = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  //char input[taille] = 0;
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.println("available");
      input += (char)LoRa.read();
      Serial.println(input);
      char* command = strtok(input, "!");
      Serial.println(command);
      while (command != 0)
      {
          Serial.println("!!!");
          // Split the command in two values
          char* separator = strtok(command, ",");
          while (separator != 0)
          {
              Serial.println(",,,");
              // Actually split the string in 2(3): replace ',' with !
              Serial.println("uno");
              //*separator = 0;
              Serial.println("dos");
              question = atoi(command);
              Serial.println("tres");
              Serial.println(question);
              ++separator;
              answer = atoi(separator);
              Serial.println(answer);
              //++separator;
              //seqNb = atoi(separator);
              //Serial.println(seqNb);
          }
          // Find the next command in input string
          command = strtok(0, "!");
      }
    }
    
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    //Serial.println(contents);

}
}
