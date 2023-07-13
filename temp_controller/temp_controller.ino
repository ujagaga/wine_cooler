#include "DS18B20.h"
#include "lcdgfx.h"

#define SENSPIN         (PD2)
#define RELAY_PIN       (PD3)
#define BTN_PIN         (PD5)
#define LED_PIN         (13)
#define TARGET_TEMP_W   (8)
#define TARGET_TEMP_R   (13)

#define ROW_1_Y         (0)
#define ROW_2_Y         (24)

DisplaySSD1306_128x32_I2C display(-1);
char w_target_msg[] = "WHITE";
char r_target_msg[] = "RED";
int target_temp = TARGET_TEMP_W;
int display_temp = 0;
int last_btn_state = 1;
bool change_flag = false;

void updateDisplay(void){
  char cstr[5];
  int distance;  

  if(change_flag){
    display.setFixedFont( comic_sans_font24x32_123 );    
    display.clear(); 

    itoa(display_temp, cstr, 10);
    display.printFixed(0,  ROW_1_Y, cstr, STYLE_BOLD);    

    display.setFixedFont( ssd1306xled_font6x8 );
    if(target_temp == TARGET_TEMP_W){
      display.printFixed(90,  ROW_1_Y, w_target_msg, STYLE_NORMAL); 
    }else{
      display.printFixed(90,  ROW_1_Y, r_target_msg, STYLE_NORMAL);
    }
    
    itoa(target_temp, cstr, 10);
    display.printFixed(90,  ROW_2_Y, cstr, STYLE_NORMAL);  
    distance = strlen(cstr) * 6;
    display.printFixed(90 + distance + 4,  ROW_2_Y - 4, "o", STYLE_NORMAL);
    display.printFixed(90 + distance + 10,  ROW_2_Y, "C", STYLE_NORMAL);
    change_flag = false;
  }
}

void updateCurrentTemperature(void){
    
  float currentTemp = ((float)DS_readTemp(SENSPIN))/16;      
 
  if(currentTemp < target_temp){
    /* Activate */ 
    digitalWrite(RELAY_PIN, HIGH); 
    digitalWrite(LED_PIN, LOW); 
  }else{
    /* Deactivate */ 
    digitalWrite(RELAY_PIN, LOW); 
    digitalWrite(LED_PIN, HIGH); 
  }

  int new_temp = int(currentTemp + 0.5);
  if(new_temp != display_temp){
    change_flag = true;   
    display_temp = new_temp;
    Serial.println(currentTemp);
  }
}

void readPushbutton(void){
  int read_1 = digitalRead(BTN_PIN);
  delay(1);
  int read_2 = digitalRead(BTN_PIN);
  delay(1);
  int read_3 = digitalRead(BTN_PIN);

  if((read_1 == read_2) && (read_2 == read_3) && (read_1 != last_btn_state)){
    last_btn_state = read_1;
    if(read_1 == 0){
      if(target_temp == TARGET_TEMP_W){
        target_temp = TARGET_TEMP_R;
      }else{
        target_temp = TARGET_TEMP_W;
      }
      change_flag = true;
    }    
  }
}

void setup() {
  digitalWrite(RELAY_PIN, LOW); 
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); 
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  Serial.begin(115200); 

  /* Select the font to use with menu and all font functions */
  display.setFixedFont( comic_sans_font24x32_123 );
  display.begin();   
  display.clear();
}

void loop() {
  updateCurrentTemperature();
  readPushbutton();
  updateDisplay();
}
