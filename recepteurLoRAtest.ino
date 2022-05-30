#include <SPI.h>
#include <LoRa.h>

String contents = "";
String buttonPress = "button pressed";
bool x;

int led = 2;

void setup() {
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    while (!Serial);
    //Wire.begin();
    Serial.println("LoRa Receiver"); 
    if (!LoRa.begin(868E6)) {
      Serial.println("Starting LoRa failed!");
      while (1);
    }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) { 
      contents += (char)LoRa.read();
    }
    
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.println(contents);

 
    if(x == true) {
      digitalWrite(led, HIGH);
      Serial.println("led on");
    }
    else {
      digitalWrite(led, LOW);
      Serial.println("led off");
    }
    
    contents = "";
  }
}
