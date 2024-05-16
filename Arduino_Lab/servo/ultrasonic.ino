
unsigned long pingTime = 0;
unsigned long usnc_ping() {
  static float myDistance = 0.0;
  unsigned long dT = millis();
  if(abs(dT - pingTime) < 400) {
    return myDistance;
  }
  
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);
  
  int duration = pulseIn(PIN_ECHO, HIGH);
  int dist = (duration*.0343)/2;
  if(dist >= 0.5) {
    myDistance = (duration*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(myDistance);
  }
  pingTime = millis();
  return myDistance;
}
