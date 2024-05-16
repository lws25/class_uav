
void batt_lcd(float volts) {
  lcd.setCursor(1,0);
  lcd.print(volts);
  lcd.print("V");
}

float batt_read() {
  static float preVolt = 0;
  int voltIn = analogRead(PIN_BATT); 
  // volts = (voltReading/1023)*100; //battFull 5V
  float volts = voltIn *(4.8/1023)*2; //分壓電路 Vo = Vin x R2/(R1+R2)
  if( abs(preVolt - volts) > 0.2 ) {
    preVolt = volts;
    //Serial.print("V=");
    //Serial.println(volts);
    batt_lcd(volts);
  }
  return volts;
}
