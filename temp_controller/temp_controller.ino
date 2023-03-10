#include "DS18B20.h"
#include "lcdgfx.h"

#define SENSPIN        (2)
#define RELAY_PIN      (3)
#define TARGET_TEMP_W  (10)
#define TARGET_TEMP_R  (15)

DisplaySSD1306_128x32_I2C display(-1);

void updateCurrentTemperature(void){
    
  float currentTemp = ((float)DS_readTemp(SENSPIN))/16; 
 
  if(currentTemp < TARGET_TEMP_W){
    /* Activate */ 
    digitalWrite(RELAY_PIN, HIGH); 
  }else{
    /* Deactivate */ 
    digitalWrite(RELAY_PIN, LOW); 
  }

  // oled.clear(); 
  // oled.println(currentTemp);

}


void setup() {
  digitalWrite(RELAY_PIN, LOW); 
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600); 

  /* Select the font to use with menu and all font functions */
  display.setFixedFont( comic_sans_font24x32_123 );
  display.begin();   
  display.clear();
}

void loop() {
  // updateCurrentTemperature();
  char cstr[5];
  for(int i = 0; i < 10; i++){
    // display.begin();
    display.setFixedFont( comic_sans_font24x32_123 );    
    display.clear(); 
    itoa(i, cstr, 10);
    display.printFixed(0,  0, cstr, STYLE_NORMAL);    

    display.setFixedFont( ssd1306xled_font6x8 );
    display.printFixed(50,  5, "o", STYLE_NORMAL);
    display.printFixed(54,  10, "C", STYLE_NORMAL);

    display.printFixed(90,  23, "WHITE", STYLE_NORMAL);    
    lcd_delay(1000);
  }
  delay(2000);
}
