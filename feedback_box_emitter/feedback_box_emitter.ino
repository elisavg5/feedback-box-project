#include <AudioZero.h>
#include <LoRa.h>
#include <SPI.h>
#include <SD.h>

int seq_number = 0; 
const int QUESTIONS_NUMBER = 3; 
const int buttons_number = 3; 
int ANALOG_PIN_Q_1 = 1; 
int ANALOG_PIN_Q_2 = 2; 
int ANALOG_PIN_Q_3 = 3; 
int LED_PINS[] = {0,1,2,3,4,5}; 
int answers[QUESTIONS_NUMBER];   //value on the questions 
int buttons[QUESTIONS_NUMBER]; //which buttons on the question
int count[buttons_number]; // decount for debouncing buttons 
int DEBOUNCE_COUNT = 3; //number of reading with the same value to change really the array with the state of the button
const int chipSelect = 4;
char acquis_donnees;
String path; 
String dataString; 

void setup() {
  pinMode(ANALOG_PIN_Q_1, INPUT);
  pinMode(ANALOG_PIN_Q_2, INPUT); 
  pinMode(ANALOG_PIN_Q_3, INPUT); 
  for (int k=0; k<sizeof(LED_PINS); k++) {
    pinMode(LED_PINS[k], OUTPUT); 
  }


  for (int j=0; j<QUESTIONS_NUMBER; j++) {
    buttons[j] = 0; 
    count[j]=0; 
  }
  path = "data.csv"; 
  Serial.begin(9600);
  
  while (!Serial);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  

  delay(1000);

}


void loop() {

answers[0] = analogRead(ANALOG_PIN_Q_1);              //Read value on analog pin 1 (the first question) 
//Serial.println("BUTTONS QUESTION 1"); 
//Serial.println(answers[0]); 
answers[1] = analogRead(ANALOG_PIN_Q_2);              //Read value on analog pin 2 (the first question) 
//Serial.println("BUTTONS QUESTION 2"); 
//Serial.println(answers[1]); 
answers[2] = analogRead(ANALOG_PIN_Q_3);             //Read value on analog pin 3 (the first question) 
//Serial.println("BUTTONS QUESTION 3"); 
//Serial.println(answers[2]); 

//for each question determining wich buttons is push
for (int question =0; question<QUESTIONS_NUMBER; question++) { 
    
  if (answers[question] > 50 && answers[question] < 180) {        //3 buttons on 1 pin = the difference between them is the value received : between 15 and 180 is the first button
    debounceButtons(question, 1); 
    //digitalWrite(0, HIGH); 
    //Serial.println("led1"); 
         
    }
  else if (answers[question] > 200 && answers[question] < 830) {
    debounceButtons(question, 2); //second button for the question
    //Serial.println("led2"); 

    }
  else if (answers[question] > 940) {
    debounceButtons(question, 3); //third button for the question
    //digitalWrite(2, HIGH);
    //Serial.println("led3"); 
 
    }
  else {
      buttons[question] = 0; //otherwise is 0
      /*if(millis() - led_time > TURN_ON_TIME) {
        ledOff(); 
        Serial.println("switch off lights"); 
      }*/
    }
  }
}

//sending Data on LoRa technology 
void sendData(int question_number) {
    seq_number++; //increasing the sequence number
    Serial.print("Sending packet: ");
    Serial.println(seq_number);
    //create packet
    String packet = String(question_number); 
    packet+= ",";
    packet += String(buttons[question_number]); 
    packet += ",";
    packet += String(seq_number); 
    Serial.println(packet); 
    // send packet
    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.print("!"); 
    LoRa.endPacket();
    LoRa.beginPacket();
    LoRa.print("0,1,2");
    LoRa.print("!"); 
    LoRa.endPacket();
 
    storingData(question_number,packet); 
    
}

//in order to avoid the bounce effect of buttons search the reference for this code 
void debounceButtons(int question_number, int button_response) {
  if(buttons[question_number] == button_response && count[question_number]>0){
      count[question_number]--; 
    }
    if(buttons[question_number] != button_response) {
      count[question_number]++; 
    }
    if(count[question_number]>= DEBOUNCE_COUNT) {
      Serial.print("appui");
      Serial.println(button_response); 
      count[question_number] = 0;
      buttons[question_number] = button_response;
      sendData(question_number);
      ledOn(question_number); 
    }
}

//store data on SD card
void storingData(int question_number, String packet) {
    File dataFile = SD.open(path, FILE_WRITE); 
    //store data on SD
    if (dataFile) {
      dataFile.println(packet); 
    }
    else {
      Serial.println("error opening the file"); 
    }
    dataFile.close(); 
}

void ledOn(int question_number) {
  if(question_number == 0 && buttons[question_number] == 1) {
    Serial.println("led D0"); 
    digitalWrite(0, HIGH); 
    delay(1000); 
    digitalWrite(0,LOW); 
  }
  if(question_number == 0 && buttons[question_number] == 2) {
    Serial.println("ledD1"); 
    digitalWrite(1, HIGH); 
    delay(1000); 
    digitalWrite(1,LOW); 
  }
  if(question_number == 0 && buttons[question_number] == 3) {
    Serial.println("led D2"); 
    digitalWrite(2, HIGH); 
    delay(1000); 
    digitalWrite(2,LOW); 
  }
    if(question_number == 1 && buttons[question_number] == 1) {
    Serial.println("led D3"); 
    digitalWrite(3, HIGH); 
    delay(1000); 
    digitalWrite(3,LOW); 
  }
  if(question_number == 1 && buttons[question_number] == 2) {
    Serial.println("ledD4"); 
    digitalWrite(4, HIGH); 
    delay(1000); 
    digitalWrite(4,LOW); 
  }
  if(question_number == 1 && buttons[question_number] == 3) {
    Serial.println("led D5"); 
    digitalWrite(5, HIGH); 
    delay(1000); 
    digitalWrite(5,LOW); 
  }
    if(question_number == 2 && buttons[question_number] == 1) {
    Serial.println("led A4"); 
    analogWrite(19, 255); 
    delay(1000); 
    analogWrite(19,0); 
  }
  if(question_number == 2 && buttons[question_number] == 2) {
    Serial.println("ledA5"); 
    analogWrite(20, 255); 
    delay(1000); 
    analogWrite(20,0); 
  }
  if(question_number == 2 && buttons[question_number] == 3) {
    Serial.println("led A6"); 
    analogWrite(21, 255); 
    delay(1000); 
    analogWrite(21,0); 
  }

}

void ledOff() {
  digitalWrite(0,LOW); 
  digitalWrite(1, LOW); 
  digitalWrite(2, LOW); 
}
