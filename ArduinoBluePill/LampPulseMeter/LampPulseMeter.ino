//схема устройства простая, поэтому в KiCade не оформлял:
//плата BluePill подключена к OLED-дисплею через I2C: GND-GND  +5V-VCC  B6-SCL  B7-SDA
//фоторизистор GL5528 один конец к +3,3 второй конец - к A0-A4. 
//Подтягивающие резисторы испрользуются встроенные в BluePill (к земле)
//Подключаются по мере необходимости для регулировки уровня
//АЦП считывается с одного из них

const int photo_r_input_pin0=PA0; //A0=PA0
const int photo_r_input_pin1=PA1; //A1=PA1
const int photo_r_input_pin2=PA2; //A2=PA2
const int photo_r_input_pin3=PA3; //A3=PA3
const int photo_r_input_pin4=PA4; //A4=PA4
const int avg_filter_value=20000; //filter for calc min and max
const float freq_coef=3.5714; //coef for calc freq
const int adc_max_gain_control_top=3000;//gain control threshold
const int adc_max_gain_control_bot=1000;//gain control threshold

int gain=0; //automaic gain level//0-max gain//4-min gain
int adc_thr_min;//threshold for freq meas
int adc_thr_max;//threshold for freq meas

#include <Oled.h>
Oled oled;

void gain_dec(){//decrement gain level
  delay(100);
  if(gain==0){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2,          INPUT);pinMode(photo_r_input_pin3,          INPUT);pinMode(photo_r_input_pin4,          INPUT);gain=1;return;}
  if(gain==1){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2, INPUT_PULLDOWN);pinMode(photo_r_input_pin3,          INPUT);pinMode(photo_r_input_pin4,          INPUT);gain=2;return;}
  if(gain==2){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2, INPUT_PULLDOWN);pinMode(photo_r_input_pin3, INPUT_PULLDOWN);pinMode(photo_r_input_pin4,          INPUT);gain=3;return;}
  if(gain==3){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2, INPUT_PULLDOWN);pinMode(photo_r_input_pin3, INPUT_PULLDOWN);pinMode(photo_r_input_pin4, INPUT_PULLDOWN);gain=4;return;}  
}

void gain_inc(){//increment gain level
  delay(100);
  if(gain==4){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2, INPUT_PULLDOWN);pinMode(photo_r_input_pin3, INPUT_PULLDOWN);pinMode(photo_r_input_pin4,        INPUT);gain=3;return;}
  if(gain==3){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2, INPUT_PULLDOWN);pinMode(photo_r_input_pin3,          INPUT);pinMode(photo_r_input_pin4,        INPUT);gain=2;return;}
  if(gain==2){pinMode(photo_r_input_pin1, INPUT_PULLDOWN);pinMode(photo_r_input_pin2,          INPUT);pinMode(photo_r_input_pin3,          INPUT);pinMode(photo_r_input_pin4,        INPUT);gain=1;return;}
  if(gain==1){pinMode(photo_r_input_pin1,          INPUT);pinMode(photo_r_input_pin2,          INPUT);pinMode(photo_r_input_pin3,          INPUT);pinMode(photo_r_input_pin4,        INPUT);gain=0;return;}  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(photo_r_input_pin0, INPUT_PULLDOWN);
  pinMode(photo_r_input_pin1, INPUT);
  pinMode(photo_r_input_pin2, INPUT);
  pinMode(photo_r_input_pin3, INPUT);
  pinMode(photo_r_input_pin4, INPUT);
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
  oled.print(1,"Ярк: "+String(adc_max));  
  oled.print(2,String(int(freq_counter*freq_coef))+" Гц"); 
  oled.print(3,"АРУ: "+String(gain));
  oled.update();
  if(adc_max>adc_max_gain_control_top)gain_dec();
  if(adc_max<adc_max_gain_control_bot)gain_inc();
  adc_min=4096;
  adc_max=0;  
  
}
