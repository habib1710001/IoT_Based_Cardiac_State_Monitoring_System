/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64
 
int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 12000
#define LowerThreshold 11000
 
void setup() {
  Serial.begin(115200);
  
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(2);
}
 
void loop()
{
oled.setTextColor(WHITE);
oled.writeFillRect(0,50,128,16,BLACK);
oled.setCursor(5,10);//column, row
oled.print("Welcome to");
oled.setCursor(5,50);
oled.print("GUDAM GHOR");
oled.display();
}
