#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // johnrickman / LiquidCrystal_I2C

#define PIN_BTN 2
#define PIN_TRIGGER  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define PIN_ECHO     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
                        // Used 14(A0) Because I was testing with Serial.print() 
                        // so I was unable to use pin 0, 1 and my digital pins where full
#define PIN_IRDLL  7
#define PIN_IRDRR  4
#define PIN_BUZZER 8
#define PIN_SERVO  9

#define PIN_LEDR 13 //LED_BUILTIN
#define PIN_LEDY 12
#define PIN_LEDG 11
#define BIT_LEDR 0x20 // pin13
#define BIT_LEDY 0x10 // pin12
#define BIT_LEDG 0x08 // pin11

#define PIN_BATT A0
#define PIN_VR   A1

#define APP_USE_LED    true
#define APP_USE_BTN    true
#define APP_USE_SERVO  true
#define APP_USE_LCD    true
#define APP_USE_IRSNR  true // IR sensor
#define APP_USE_VRES   true // variant resistor
#define APP_USE_USNC   true // ultrasonic
#define APP_USE_BUZZER true
#define APP_USE_BATT   true

void led_ctl_on(unsigned char bits);
bool btn_read(int pin);
void vr_ctl_servo(int pin);
void btn_ctl_servo();

#if APP_USE_LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

#if APP_USE_SERVO
Servo myservo;
#endif

bool sysPwrOn = false;
bool btnDown = false;
bool btnClick = false;
unsigned long btnDownMs = 0;
bool buzEn = false;

int servAngle = 2;
int servRev = 1;

void setup() { 
  Serial.begin(9600);

#if APP_USE_LED
  pinMode(PIN_LEDR, OUTPUT);
  digitalWrite(PIN_LEDR, LOW);
  pinMode(PIN_LEDY, OUTPUT);
  digitalWrite(PIN_LEDY, LOW);
  pinMode(PIN_LEDG, OUTPUT);
  digitalWrite(PIN_LEDG, LOW);
#endif  
#if APP_USE_LCD
  lcd.begin(16,2);
  lcd.init();
  lcd.noBacklight();
  lcd.clear();
#endif
#if APP_USE_SERVO
  myservo.attach(PIN_SERVO); 
  for(int pos = 0; pos <=90; pos+=1) {
    myservo.write(pos);
    delay(15);
  }
#endif
#if APP_USE_BTN
  pinMode(PIN_BTN, INPUT);
  digitalWrite(PIN_BTN, HIGH);
#endif
#if APP_USE_VRES
  pinMode(PIN_VR, INPUT);
#endif
#if APP_USE_IRSNR
  pinMode(PIN_IRDLL, INPUT);
  pinMode(PIN_IRDRR, INPUT);
#endif
#if APP_USE_BUZZER
  pinMode(PIN_BUZZER,OUTPUT);
#endif
#if APP_USE_BATT
  pinMode(PIN_BATT, INPUT);
#endif
#if APP_USE_USNC
  pinMode(PIN_TRIGGER, OUTPUT);  
  pinMode(PIN_ECHO, INPUT);
#endif

}

void loop(){
  float myDist = 0;

  btnClick = btn_read(PIN_BTN);
  if(!sysPwrOn && !btnClick) {
    delay(50);
    return;  
  }
 
  if(btnClick) {
    btnClick = false;
    sysPwrOn = !sysPwrOn;
    Serial.print("sysPwrOn: ");
    Serial.println(sysPwrOn);

    if(sysPwrOn) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sys PwrOn..");
      lcd.backlight();
      delay(1500);
      lcd.clear();
    }
    else {
      lcd.noBacklight();
      lcd.clear();      
    }
    delay(20);
  }

  //action after system pwr on
  if(sysPwrOn) {
    
    batt_read();
    myDist = usnc_ping();
    
    delay(50);
  }

  //any actions on loop
  if(btnDown) {
    //按住button, 持續控制servo
    btn_ctl_servo();
    delay(50);
  }
  else {
    //IR 接近偵測, 擺動控制servo
    ir_sensor();
    delay(50);
  }
  
#if APP_USE_BUZZER
  if(buzEn) {
    // tone() is the main function to use with a buzzer, 
    //it takes 2 or 3 parameteres (buzzer pin, sound frequency, duration)
    tone(PIN_BUZZER, 3000, 50);
    buzEn = false;
  }
#endif // end_APP_USE_USNC

#if APP_USE_USNC
  if(myDist <= 10.0) {
    led_ctl_on(BIT_LEDR|BIT_LEDY|BIT_LEDG);
  }
  else if(myDist <= 20.0) {
    led_ctl_on(BIT_LEDY|BIT_LEDG);
  }
  else if(myDist <= 30.0) {
    led_ctl_on(BIT_LEDG);
  }
  else {
    led_ctl_on(0);
  }
  delay(30);
#endif // end_APP_USE_USNC

  
} ////end_loop()

#if APP_USE_LED
void led_ctl_on(unsigned char bits) {
  PORTB &= bits; //clear
  PORTB |= bits;
}
#endif

#if APP_USE_VRES && APP_USE_SERVO
void vr_ctl_servo(int pin) {
  int servRev = 1;
  int vr_val = analogRead(pin);

  Serial.print("RA:");
  Serial.println(vr_val);
  vr_val = map(vr_val, 0, 1023, 2, 178);
  Serial.print("AG:");
  Serial.println(vr_val);
  myservo.write(vr_val); 
  delay(20);
}
#endif

#if APP_USE_BTN && APP_USE_SERVO
void btn_ctl_servo() {
  //Serial.println(servAngle);
  myservo.write(servAngle); 
  servAngle += (5 * servRev);
  if(servAngle > (180-2) ){ // offset deadzone 2~5
    servAngle = 178;
    servRev = -servRev;
  }
  else if(servAngle < 5){
    servAngle = 3;
    servRev = -servRev;
  }  
}
#endif
