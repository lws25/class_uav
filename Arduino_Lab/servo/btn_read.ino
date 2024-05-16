
bool btn_read(int pin) {
  int val = digitalRead(pin);
  while(val != digitalRead(pin)) {
    val = digitalRead(pin);
    delay(5);
  }
    
  if(val == LOW) {
    if(!btnDown) {
      btnDownMs = millis();
      Serial.println("btnDown");
    }
    btnDown = true;
  }
  else{
    if(btnDown){
      btnDown = false;
      btnClick = true;
      Serial.println("btnClick");
    }
  }
  return btnClick;  
}
