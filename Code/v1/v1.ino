#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>
#include <ArduinoJson.h>
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D
#define UpperThreshold 14000
#define LowerThreshold 10000


#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif


Adafruit_ADS1115 ads;     // Use this for the 16-bit version 
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64


TaskHandle_t Task1, Task2;

DynamicJsonDocument doc(24576);
JsonArray value1 = doc.createNestedArray("value1");


// Replace with your network credentials
const char* ssid     = "3 Musketees";
const char* password = "ECE1710001";


// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://sansisenseplus.com/ecg_project1.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";

int i = 0;
String httpRequestData;
 


int16_t adc0;

int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;


// define two tasks for Sending data & ECG data collection
void TaskSendData( void *pvParameters );
void TaskEcgRead( void *pvParameters );


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(2);
  ads.begin();
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // Now set up two tasks to run independently.

  xTaskCreatePinnedToCore(
    TaskEcgRead
    ,  "Task Ecg Read"
    ,  40000  // Stack size
    ,  NULL
    ,  2 // Priority
    ,  &Task1 
    ,  1);
    
  xTaskCreatePinnedToCore(
     TaskSendData
    ,  "Task Send Data"
    ,  40000
    ,  NULL
    ,  1  //Priority
    ,  &Task2
    ,  0);
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
  delay(1000);
}


void TaskEcgRead(void *pvParameters)  // This is a task.
{
 for( ; ; )
 {
    (void) pvParameters;
    if(a>600)
    {
      oled.clearDisplay();
      a=0;
      lasta=a;
    }
    if(a<0)
    {
     oled.clearDisplay();
     a=127;
     lasta=a;
    }
 
    ThisTime=millis();
    ads.setSPS(ADS1115_DR_860SPS);
    int value=ads.readADC_SingleEnded(0);
    oled.setTextColor(WHITE);
    int b=37-(value/400);
    oled.writeLine(lasta,lastb,a,b,WHITE);
    lastb=b;
    lasta=a;
  
    if(value>UpperThreshold)
    {
     if(BeatComplete)
      {
       BPM=ThisTime-LastTime;
       BPM=int(60/(float(BPM)/1000));
       BPMTiming=false;
       BeatComplete=false;
      }
     if(BPMTiming==false)
      {
       LastTime=millis();
       BPMTiming=true;
      }
   }
   if((value<LowerThreshold)&(BPMTiming))
   BeatComplete=true;
 
    oled.writeFillRect(0,50,128,16,BLACK);
    oled.setCursor(0,50);
    oled.print("BPM:");
    oled.print(BPM);
    oled.display();
    a--;

    //*********************************************************************************************************//

    if( i == 0)
    {
     doc["api_key"] = "1234"; 
    }
    value1.add(ads.readADC_SingleEnded(0));
    i++ ;
    //Serial.println(ads.readADC_SingleEnded(0));
    if( i == 1500)
    {
     serializeJson(doc,httpRequestData); 
     serializeJson(doc,Serial); 
     i=0;
    }
 }
}


void TaskSendData(void *pvParameters)
{    
  for( ; ; )
  {
   (void) pvParameters;
   if(i >= 1500)
   {   
    if(WiFi.status()== WL_CONNECTED)
    {
     HTTPClient http;
    
     // Your Domain name with URL path or IP address with path
     http.begin(serverName);
    
     // Specify content-type header
     http.addHeader("Content-Type", "application/json");
    
     //send HTTP POST request
     int httpResponseCode = http.POST(httpRequestData);
     
     if (httpResponseCode>0) 
     {
      Serial.println("HTTP Response code: ");
      Serial.println(httpResponseCode);
      i=0;
     }
     else 
     {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
     }
     // Free resources
     http.end();
    }
    else 
    {
     Serial.println("WiFi Disconnected");
    }
   }
   vTaskDelay(2);
 }
}
