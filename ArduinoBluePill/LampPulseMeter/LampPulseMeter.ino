const int photo_r_input_pin=PA0; //A0=PA0
const int avg_filter_value=10000; //filter for calc min and max

#include <Oled.h>

Oled oled;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(photo_r_input_pin, INPUT_PULLDOWN);
  oled.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc_min=4096;
  int adc_max=0;
  
  for(int i;i<avg_filter_value;i++){
    int adc=analogRead(photo_r_input_pin);
    if(adc<adc_min)adc_min=adc;
    if(adc>adc_max)adc_max=adc;
  }
  Serial.println("debug");  
//  oled.print(0,String(adc_max-adc_min));
//  oled.print(2,String(adc_max));
//  oled.print(3,String(adc_min));
  oled.print(0,"debug");
  oled.update();
  adc_min=4096;
  adc_max=0;
  
}
