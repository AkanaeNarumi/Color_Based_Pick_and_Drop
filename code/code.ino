#include<Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 8, 12, 13);
Servo base;
Servo arm;

//inputs
int pickupclr=0, dropclr=0;
//servo
int basepin=5, armpin=6;
//RGBled and LDR 
int bluepin=11, greenpin=10, redpin=9;
int ldrpin=A2;
int red=0,green=0,blue=0;
int redint=255, blueint=255,greenint=255 ;
//IR
int irPin = A1;
int irOut = HIGH;
//stopper
int stopper = 1;
//EM
int empin=7;
//lcd
int swit = 8;
int now=1, past=1, count=0;

void setup() {

  /*pinMode(red_button, INPUT);
  pinMode(green_button, INPUT);
  pinMode(blue_button, INPUT);
*/

  pinMode(empin, OUTPUT);
  digitalWrite(empin, LOW);

  pinMode(swit, INPUT_PULLUP);
  
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Select the");
  lcd.setCursor(0,1);
  lcd.print("pickup color");

  delay(2000);

  lcd.setCursor(0,0);
  lcd.print("  R     G     B ");
  count=0;
  for(int i=0; i<100; i++){

    now = digitalRead(swit);
    if (past == 1 && now == 0 ) {
      count = count+1;
    }
    
    past=now;
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    if ((count%3) == 0){
      lcd.setCursor(2,1);
      lcd.print((char)255);
    }
    else if (count%3 == 1){
      lcd.setCursor(8,1);
      lcd.print((char)255);
    }
    else if (count%3 == 2){
      lcd.setCursor(14,1);
      lcd.print((char)255);
    }
    delay(50);
  }

  pickupclr = 3+(count%3);

  Serial.print("pickup:");
  Serial.print(pickupclr);
  
  count=0;
  lcd.setCursor(0,0);
  lcd.print("Select the      ");
  lcd.setCursor(0,1);
  lcd.print("drop color      ");

  delay(2000);

  lcd.setCursor(0,0);
  lcd.print("  R     G     B ");

  for(int i=0; i<100; i++){

    now = digitalRead(swit);
    if (past == 1 && now == 0 ) {
      count = count+1;
    }
    
    past=now;
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    if ((count%3) == 0){
      lcd.setCursor(2,1);
      lcd.print((char)255);
    }
    else if (count%3 == 1){
      lcd.setCursor(8,1);
      lcd.print((char)255);
    }
    else if (count%3 == 2){
      lcd.setCursor(14,1);
      lcd.print((char)255);
    }
    delay(50);
  }

  dropclr = 3+(count%3);

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
  base.attach(basepin);
  arm.attach(armpin);

  pinMode(irPin, INPUT);

  base.write(30);
  arm.write(0);
  delay(150);
}

int get_clr(int r,int g,int b){
    
    if (r<160 && g<160 && b<160){
      return 1;
    }
    else if (r>200 && g>200 && b>200 ){
      return 2;
    }
    else if (r<180 && r<g && r<b){
      return 3;
    }
    else if (g<200 && g>140 && g<r && g<b){
      return 4;
    }
    else if (b<200 && b>100 && b<g && b<r ){
      return 5;
    }
    
    return 0;
  }

int read_clr(int tclr){
  for (int pos = 30; pos <= 160; pos += 1) { 
    

  analogWrite(redpin, redint);
  delay(10);
  red=analogRead(ldrpin);
  delay(10);
  analogWrite(redpin, 0);
  
  analogWrite(bluepin, blueint);
  delay(10);
  blue=analogRead(ldrpin);
  delay(10);
  analogWrite(bluepin, 0);

  analogWrite(greenpin, greenint);
  delay(10);
  green=analogRead(ldrpin);
  delay(10);
  analogWrite(greenpin, 0);      
    delay(150);        

  if (get_clr(red,green,blue) == tclr){
    return 1;
  }
  base.write(pos);
  }
  return 0;
}

int get_obj(){
    
    
  }

int read_obj(){
  for (int pos = 0; pos <= 180; pos += 1) { 
    

    irOut = digitalRead(irPin);
    
    if (irOut == LOW){
      Serial.println("none");
    }
    else {
      return 1;
    }
                
    delay(150);

  arm.write(pos); 
  }
  return 0;
}



void loop() {
/*
  if (digitalRead(red_button) == HIGH) {
    Serial.println("red");
  }
  if (digitalRead(green_button) == HIGH) {
    Serial.println("green");
  }
  if (digitalRead(blue_button) == HIGH) {
    Serial.println("blue");
  }
*/

  lcd.setCursor(0,0);
  lcd.print("Searching for");
  lcd.setCursor(0,1);
  lcd.print("     color     ");
  
  while (stopper){
    if (read_clr(pickupclr) == 1){
    Serial.println("Stop base");
    stopper=0;
  }
  }
  stopper=1;

  lcd.setCursor(0,1);
  lcd.print("     object     ");
    
  
  while (stopper){
    if (read_obj() == 1){
    stopper=0;
  }
  }
  stopper=1;

  lcd.setCursor(0,0);
  lcd.print("Picking up     ");
  lcd.setCursor(0,1);
  lcd.print("               ");
  
  //function for pickup
  digitalWrite(empin, HIGH);
  delay(5000);
  arm.write(0);

  lcd.setCursor(0,0);
  lcd.print("Searching for");
  lcd.setCursor(0,1);
  lcd.print("     color     ");
  
  while(stopper){
    if (read_clr(dropclr) == 1){
    stopper=0;
  }

  lcd.setCursor(0,1);
  lcd.print("     object     ");
  
  }
  stopper=1;
  while (stopper){
    if (read_obj() == 1){
    stopper=0;
  }
  }
  stopper=1;

  lcd.setCursor(0,0);
  lcd.print("Droping       ");
  lcd.setCursor(0,1);
  lcd.print("               ");

  //function for drop
  
  digitalWrite(empin, LOW);
  delay(5000);
  arm.write(0);

  int n;
  lcd.setCursor(0,0);
  lcd.print("Do you want to");
  lcd.setCursor(0,1);
  lcd.print("run it again?");

  delay(2000);
  
  lcd.setCursor(0,0);
  lcd.print("   Yes      No  ");
  
  count=0;
  for(int i=0; i<100; i++){

    now = digitalRead(swit);
    if (past == 1 && now == 0 ) {
      count = count+1;
    }
    
    past=now;
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    if ((count%2) == 0){
      lcd.setCursor(4,1);
      lcd.print((char)255);
    }
    else if (count%2 == 1){
      lcd.setCursor(12,1);
      lcd.print((char)255);
    }
    delay(50);
  }

  if (count%2 == 1){
    lcd.setCursor(0,0);
    lcd.print("Please turn off ");
    lcd.setCursor(0,1);
    lcd.print("the power       ");

    exit(0);
    
  }
  else {
  lcd.setCursor(0,0);
  lcd.print("Select the      ");
  lcd.setCursor(0,1);
  lcd.print("pickup color    ");

  delay(2000);

  lcd.setCursor(0,0);
  lcd.print("  R     G     B ");
  count=0;
  for(int i=0; i<100; i++){

    now = digitalRead(swit);
    if (past == 1 && now == 0 ) {
      count = count+1;
    }
    
    past=now;
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    if ((count%3) == 0){
      lcd.setCursor(2,1);
      lcd.print((char)255);
    }
    else if (count%3 == 1){
      lcd.setCursor(8,1);
      lcd.print((char)255);
    }
    else if (count%3 == 2){
      lcd.setCursor(14,1);
      lcd.print((char)255);
    }
    delay(50);
  }

  pickupclr = 3+(count%3);

  Serial.print("pickup:");
  Serial.print(pickupclr);
  
  count=0;
  lcd.setCursor(0,0);
  lcd.print("Select the     ");
  lcd.setCursor(0,1);
  lcd.print("drop color     ");

  delay(2000);

  lcd.setCursor(0,0);
  lcd.print("  R     G     B ");

  for(int i=0; i<100; i++){

    now = digitalRead(swit);
    if (past == 1 && now == 0 ) {
      count = count+1;
    }
    
    past=now;
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    if ((count%3) == 0){
      lcd.setCursor(2,1);
      lcd.print((char)255);
    }
    else if (count%3 == 1){
      lcd.setCursor(8,1);
      lcd.print((char)255);
    }
    else if (count%3 == 2){
      lcd.setCursor(14,1);
      lcd.print((char)255);
    }
    delay(50);
  }

  dropclr = 3+(count%3);

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
  }
  
}
