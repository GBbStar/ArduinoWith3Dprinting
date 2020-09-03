#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "max6675.h"



char temp[20];
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

int menu=9;
int timer =5;
int reset = 10; 
int moveup = 6;
int movedown = 4;
int settemp = 0;
int setminute =0;
int setsec = 0;
int speakerpin =7;
int alarmtemp = 0;
int alarmminute=0;
int alarmsec=0;

byte timerState = false;
byte menuState = false;
byte tempalarm = false;
byte timealarm = false;

int menuselect=1;
int count=0;

unsigned long currentMillis;
unsigned long currentMillis_2=0;
static unsigned long tempMillis=0;
unsigned long currentMin = 0;

int WhatToshow=1;

 
void setup()   {               
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  pinMode(menu, INPUT_PULLUP);
  pinMode(timer, INPUT_PULLUP);
  pinMode(reset, INPUT_PULLUP);
  pinMode(movedown, INPUT_PULLUP);
  pinMode(moveup, INPUT_PULLUP);
 
 // WhatToshow;
 tone(speakerpin, 5000, 1000);
}

void loop() {
  if (digitalRead(menu) == LOW && menuState == false && timerState == false ){
    WhatToshow = 2;
    menuselect=1;
    menuState = true;
    delay(100);
  }
  else if (digitalRead(movedown) == LOW && menuselect == 1 && WhatToshow == 2){
    WhatToshow = 22;
    menuselect=2;
    delay(100);
  }
  else if (digitalRead(movedown) == LOW && menuselect== 2 && WhatToshow == 22){
    WhatToshow = 2;
    menuselect =1 ;
    delay(100);
  }
  else if (digitalRead(menu)==LOW && menuState== true && timerState == false){
    WhatToshow = 1;
    menuState = false;
    delay(100);
  }
  
  else if (digitalRead(timer)==LOW && timerState == false && menuState==false){
    WhatToshow = 12;
    timerState = true;
    delay(100);
  }
  else if (digitalRead(timer)==LOW && timerState == true && menuState==false){
    WhatToshow = 1;
    timerState = false;
    delay(100);
  }
  else if(digitalRead(reset)==LOW && timerState == false && menuState==false){
    currentMillis=0;
    tempMillis=0;
    delay(100);
  }

  else if (digitalRead(reset)==LOW && menuState==true && menuselect==1){
    WhatToshow = 31;
    tempalarm = false;
    delay(100);
  }
  
  else if (digitalRead(reset)==LOW && menuState==true && menuselect==2){
    WhatToshow = 32;
    timealarm = false;
    delay(100);
    
  }
  else if (digitalRead(movedown)==LOW && WhatToshow == 31){
    settemp--;
    delay(10);
  }
  
  else if (digitalRead(moveup)==LOW && WhatToshow == 31){
    settemp++;
    delay(10);
  }
  
  else if (digitalRead(movedown)==LOW && WhatToshow == 32){
    if (setminute<=0 && setsec<1){
      return;
    }
    if (setsec<1){
      setminute--;
      setsec=59;
      }
    else setsec--;
    delay(50);
  }
  
  else if (digitalRead(moveup)==LOW && WhatToshow == 32){
    if (setsec>58){
      setsec=0;
      setminute++;
    }
    else setsec++;
    delay(50);
  }

  else if (WhatToshow == 31 && digitalRead(timer)==LOW){
    tempalarm=true;
    
    WhatToshow = 2;
    alarmtemp=settemp;
  }
  
  else if (WhatToshow == 32 && digitalRead(timer)==LOW){
    timealarm=true;
    
    WhatToshow = 2;
    alarmminute=setminute;
    alarmsec = setsec;
  } 
  
  else {
    
  }
  
  switch (WhatToshow){
    case 1: 
            print_menu1();
            break;
    case 2:
            print_menu2();
            break;
    case 12:
            
            print_menu1_2();
            break;
    case 22:
            print_menu2_2();
            break;

    case 31:
            print_menu3_1();
            break;
            
    case 32:
            print_menu3_2();
            break;       
  }
  
}
void print_menu1(){
  count=0;
  MAX6675 ts(16, 15, 14);
  float t = ts.readCelsius();
  Serial.print(ts.readCelsius(), 2);
  
  dtostrf(t, 4, 2, temp);

  sprintf(temp, "%s", temp);
  
   // 시간 
  currentMillis_2 = currentMillis;
  unsigned long temp2 = currentMillis_2 % 3600000;
  unsigned long minute_2 = currentMin + temp2/60000;
  unsigned long second_2 = temp2%60000;
  unsigned long sec_2 = second_2/1000;
  
  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print("TIME ");
  display.print((unsigned long)minute_2);
  display.print(":");
  display.print((unsigned long)sec_2);   
  
  // 온도 출력
  display.setCursor(0,40);
  display.print("TEMP:");
  display.print(temp);
  display.display();
  delay(10);
  Serial.println(1);
 
 if (tempalarm==true){
  if(alarmtemp<=t){
    tone(speakerpin, 5000, 1000);
  }
 }
 if (timealarm==true){
  if(alarmminute<=minute_2 && alarmsec <=sec_2){
    tone(speakerpin, 5000, 1000);
  }
 }
}

void print_menu1_2(){
  MAX6675 ts(16, 15, 14);
  float t = ts.readCelsius();
  Serial.print(ts.readCelsius(), 2);
  
  dtostrf(t, 4, 1, temp);

  sprintf(temp, "%s", temp);

  if(count==0){
    tempMillis = millis();
    count++;
  }
  else{}
  
  // 시간
    
  currentMillis = millis()-(tempMillis-currentMillis_2);


  currentMillis = currentMillis%3600000;
  unsigned long minute = currentMin + currentMillis/60000;
  unsigned long second = currentMillis%60000;
  unsigned long sec = second/1000;

  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print("TIME ");
  display.print((unsigned long)minute);
  display.print(":");
  display.print((unsigned long)sec);   
  
  // 온도 출력
  display.setCursor(0,40);
  display.print("TEMP:");
  display.print(temp);
  display.display();
  delay(10);
  Serial.println(12);

 if (tempalarm==true){
  if(alarmtemp<=t){
    tone(speakerpin, 5000, 1000);
  }
 }
 if (timealarm==true){
  if(alarmminute<=minute && alarmsec <=sec){
    tone(speakerpin, 5000, 1000);
  }
 }
}

void print_menu2(){
  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("> SET TEMP ALARM");
        
  display.setCursor(0,40);
  display.print("SET TIME ALARM");
  display.display();
  delay(10);
  Serial.println(2);
}

void print_menu2_2(){
  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("SET TEMP ALARM");
        
  display.setCursor(0,40);
  display.print("> SET TIME ALARM");
  display.display();
  delay(10);
  Serial.println(2);
}

void print_menu3_1(){
  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(0,10);
  display.print(settemp);
  display.print(" C");
  display.display();
  delay(10);
}

void print_menu3_2(){
  display.clearDisplay();

  // 시간출력
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(0,10);
  display.print(setminute);
  display.print(" : ");
  display.print(setsec);
  display.display();
  delay(10);
}
