<<<<<<< Updated upstream
const int photo_r_input_pin=PA0; //A0=PA0
const int avg_filter_value=20000; //filter for calc min and max
const float freq_coef=1.7857; //coef for calc freq

int adc_thr_min;//threshold for freq meas
int adc_thr_max;//threshold for freq meas
=======
//схема устройства простая, поэтому в KiCade не оформлял:
//плата BluePill подключена к OLED-дисплею через I2C
//фоторизистор GL5528 один конец к +3,3 второй конец - к A0,A1. Подтягивающие резисторы испрользуются встроенные в BluePill
//АЦП считывается с одного из них

const int photo_r_input_pin0=PA0; //A0=PA0
const int photo_r_input_pin1=PA1; //A1=PA1
const int avg_filter_value=10000; //filter for calc min and max
>>>>>>> Stashed changes

#include <Oled.h>
Oled oled;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(photo_r_input_pin0, INPUT_PULLDOWN);
  pinMode(photo_r_input_pin1, INPUT_PULLDOWN);
  oled.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc_min=4096;
  int adc_max=1;
  int freq_counter=0;
  bool sine_upper_level;
  
  for(int i;i<avg_filter_value;i++){
    int adc=analogRead(photo_r_input_pin0);
    if(adc<adc_min)adc_min=adc;
    if(adc>adc_max)adc_max=adc;
    if((adc<adc_thr_min)and(sine_upper_level)){
      sine_upper_level=0;
      freq_counter++;      
    }
    if((adc>adc_thr_max)and(not(sine_upper_level))){
      sine_upper_level=1;
      freq_counter++;      
    }
    
    }
  adc_thr_min=adc_min+int(((adc_max-adc_min)/3));
  adc_thr_max=adc_max-int(((adc_max-adc_min)/3));

  //Serial.println("debug"); 
  oled.print(0,String((float(adc_max-adc_min)/adc_max)*100,1)+"%");
  oled.print(2,"Ярк: "+String(adc_max));  
  oled.print(3,String(int(freq_counter*freq_coef))+" Гц"); 
  oled.update();
  adc_min=4096;
  adc_max=0;
  
}
