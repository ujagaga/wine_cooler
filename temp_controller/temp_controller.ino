#include "DS18B20.h"
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define SENSPIN     (2)
#define RELAY_PIN   (3)
#define TARGET_TEMP (10)

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiAvrI2c oled;

void updateCurrentTemperature(void){
    
  float currentTemp = ((float)DS_readTemp(SENSPIN))/16; 
 
  if(currentTemp < TARGET_TEMP){
    /* Activate */ 
    digitalWrite(RELAY_PIN, HIGH); 
  }else{
    /* Deactivate */ 
    digitalWrite(RELAY_PIN, LOW); 
  }

  oled.clear(); 
  oled.println(currentTemp);

}


void setup() {
  digitalWrite(RELAY_PIN, LOW); 
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600); 

#if RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.
  oled.setFont(fixednums15x31);  
}

void loop() {
  // updateCurrentTemperature();
  for(int i = 0; i < 10; i++){
    oled.clear(); 
    oled.print(i);
    oled.print(" WHITE");
    delay(1000);
  }
  delay(2000);
}
