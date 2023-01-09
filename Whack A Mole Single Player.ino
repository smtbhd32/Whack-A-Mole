// IMPORTANT!
// DOWNLOAD the 3MD_Parola library
// https://github.com/MajicDesigns/MD_Parola

#include <ArduinoJson.h>
#include <stdio.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
//#include "Parola_Fonts_data.h"
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   10
#define DATA_PIN  8
#define CS_PIN    9
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int pin_light = 0;
bool GAME_ON = false;
long randNumber;
int step_counter = 0;
int button_values[] = {A8,A7,A15,A12,A13,A14,A9,A10,A11,A6}; //{A6,A7,A8,A9,A10,A11,A12,A13,A14,A15}
int leds_cnt = 10;
int p_leds[] = {22,23,24,25,26,27,28,29,30,31};
int p_score = 0;
int action_speed = 2500;
int action_speed_min = 250;
int max_score = 0;
int pin = 0;
int time = -50000;
int timer = time;

void setup()
{
  Serial.begin(9600);
  DynamicJsonDocument doc(1024);
  for (int i = 0; leds_cnt > i; i++) {
    pinMode(p_leds[i], OUTPUT);
    pinMode(button_values[i], INPUT);
  }
    

  P.begin(2);
  P.setZone(0,0,3);
  P.setZone(1,4,7);
  P.displayZoneText(0, "abc", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  //P.setZoneEffect(0, true, PA_FLIP_UD);
  P.displayZoneText(1, "abcd", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.setZoneEffect(1, true, PA_FLIP_UD);
  P.setZoneEffect(1, true, PA_FLIP_LR);
  P.displayAnimate();

}

void loop()
{ /*
  for (int i = 0; leds_cnt > i; i++) {
    digitalWrite(p_leds[i], HIGH);
    while(true){
      if(analogRead(button_values[i]) > 1000){
        digitalWrite(p_leds[i], LOW);
        break;
      }
    }
  }
  for(int i = 0; i < leds_cnt; i++){
    int vi = analogRead(button_values[i]);    
    Serial.println(vi);
    if(vi > 1000){
      Serial.print("on ");     
      Serial.println(i);     
    }
    delay(100);
  }*/
  DynamicJsonDocument doc(1024);
  if(GAME_ON == false){
  if(analogRead(button_values[6]) > 1000){
    if(analogRead(button_values[7]) > 1000){
      GAME_ON = true;
    }
  }
  }
  else 
  { 
    if(timer <= time) 
    {
      step_counter++;
      bool step_action = false;
      if (step_counter > action_speed) {
        step_counter = 0;
        step_action = true;  
        action_speed -= round(action_speed/50);
        if (action_speed < action_speed_min) {
          action_speed = action_speed_min;
        }
      }
    
      if (step_action) {
        digitalWrite(p_leds[pin_light], LOW);
        pin_light = random(0,10);
        digitalWrite(p_leds[pin_light], HIGH);
      }
      if (analogRead(button_values[pin_light]) > 1000){
          if(digitalRead(p_leds[pin_light]) == HIGH){
            digitalWrite(p_leds[pin_light], LOW);
            p_score++;
          }
        }
      timer -= 1;
      if (step_counter % 100 == 0){
        char Score1[80];
        sprintf(Score1, "%d", p_score);
        char *chrdisp[] = {Score1};
    
        char Score2[80];
        sprintf(Score2, "%d", max_score);
        char *chrdisp2[] = {Score2};      
    
        P.displayZoneText(0, chrdisp[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayZoneText(1, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();
        //Serial.print("YOUR SCORE: ");
        //Serial.println(p_score);
        doc["Your Score: "] = p_score;
        doc["Max Score: "] = max_score;
        serializeJson(doc,Serial);
        Serial.println();
      }
    }
    else if(timer > time)
    {
      if(p_score > max_score)
      {
        max_score = p_score;
        char Score2[80];
        sprintf(Score2, "%d", max_score);
        char *chrdisp2[] = {Score2};
        P.displayZoneText(0, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayZoneText(1, "New Record!", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayAnimate();  
        //delay(2500);
        // Serial.println("New Record !!!");   
        // Serial.println();
      }
      digitalWrite(p_leds[pin_light], LOW);
      GAME_ON = false;
      timer = 0;
      p_score = 0;
      action_speed = 2500; 
      // Serial.print("MAX SCORE: ");
      // Serial.println(max_score); 
      // Serial.print("YOUR SCORE: ");
      // Serial.println(p_score);
      doc["Your Score: "] = p_score;
      doc["Max Score: "] = max_score;
      serializeJson(doc,Serial);
      Serial.println(); 
      P.displayZoneText(0, "To Start!", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(1, "Press", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayAnimate();
    }
    else
    {
      // Serial.print("MAX SCORE: ");
      // Serial.println(max_score);
      doc["Max Score: "] = max_score;
      serializeJson(doc,Serial);
      Serial.println(); 
    }
  }   
}