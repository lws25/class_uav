void ir_sensor() {
  int irLL = digitalRead(PIN_IRDLL);
  int irRR = digitalRead(PIN_IRDRR);

  if(irLL == 0) {
      myservo.write(70);
      servAngle = 70;
      Serial.println("IR.Left");
      buzEn = true;
  }
  else if(irRR == 0) {
      myservo.write(110);
      servAngle = 110;
      Serial.println("IR.Right");
      buzEn = true;
  }
  else {
    myservo.write(90);
    buzEn = false;
  }  
}
