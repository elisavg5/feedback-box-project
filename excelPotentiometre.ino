int pot = A1;

void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL, Time, Started Time, Date, Analog Value, POT%");
  Serial.println("RESETTIMER");
}

void loop() {
  int pot_val = analogRead(pot);
  int pot_percentage = map(pot_val, 0, 1023, 0, 100);
  Serial.print("DATA, TIME, TIMER? DATE");
  Serial.print(pot_val);
  Serial.print(" ,");
  Serial.print(pot_percentage);
  Serial.println(" ,");
  delay(1500);
}
