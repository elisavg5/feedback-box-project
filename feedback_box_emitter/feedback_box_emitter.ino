#include <LoRa.h>
#include <SPI.h>
#include <SD.h>

int seq_number = 0; 
const int PARAM_BOX_ID = 1;
const int QUESTIONS_NUMBER = 3; 
const int buttons_number = 3; 
int ANALOG_PIN_Q_1 = 1; 
int ANALOG_PIN_Q_2 = 2; 
int ANALOG_PIN_Q_3 = 3; 
int LED_PINS[] = {3,5,6,0,1,2,19,20,21}; 
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
  for (int k=0; k<6; k++) {
    pinMode(LED_PINS[k], OUTPUT); 
  }

  for (int j=0; j<QUESTIONS_NUMBER; j++) {
    buttons[j] = 0; 
    count[j]=0; 
  }
  path = "data.csv"; 
  Serial.begin(9600);
  
  //while (!Serial);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(868100000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  

  delay(1000);

}


void loop() {

answers[0] = analogRead(ANALOG_PIN_Q_1);              //Read value on analog pin 1 (the first question) 
answers[1] = analogRead(ANALOG_PIN_Q_2);              //Read value on analog pin 2 (the first question) 
answers[2] = analogRead(ANALOG_PIN_Q_3);             //Read value on analog pin 3 (the first question)  

//for each question determining wich buttons is push
for (int question =0; question<QUESTIONS_NUMBER; question++) { 
    
  if (answers[question] > 50 && answers[question] < 180) {        //3 buttons on 1 pin = the difference between them is the value received : between 15 and 180 is the first button
    debounceButtons(question, 1);          
    }
  else if (answers[question] > 200 && answers[question] < 830) {
    debounceButtons(question, 2); //second button for the question
    }
  else if (answers[question] > 940) {
    debounceButtons(question, 3); //third button for the question 
    }
  else {
      buttons[question] = 0; //otherwise is 0
    }
  }
}

//sending Data on LoRa technology 
void sendData(int question_number) {
    seq_number++; //increasing the sequence number
    Serial.print("Sending packet: ");
    Serial.println(seq_number);
    //create packet
    String packet = String(PARAM_BOX_ID); 
    packet+= ",";
    String packet = String(question_number); 
    packet+= ",";
    packet += String(buttons[question_number]); 
    packet += ",";
    packet += String(seq_number); 
    Serial.println(packet); 
    // send packet
    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.print(","); 
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
      count[question_number] = 0; //debounce utility 
      buttons[question_number] = button_response;
      sendData(question_number); //sendData on LoRA + store on SD card
      ledOn(question_number); //turn the right led on 
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
  int indice = question_number*3+buttons[question_number]-1;
  Serial.println(indice); 
  if(question_number != 2) {
    digitalWrite(LED_PINS[indice], HIGH); 
    delay(1000); 
    digitalWrite(LED_PINS[indice],LOW); 
  }
  else if (question_number == 2) {
    analogWrite(LED_PINS[indice], 255); 
    delay(1000); 
    analogWrite(LED_PINS[indice],0); 
  }  
}
