#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <Wire.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D

Adafruit_ADS1115 ads;     // Use this for the 16-bit version 
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64



TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;
static SemaphoreHandle_t mutex;

int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 12000
#define LowerThreshold 9000


// Replace with your network credentials
const char* ssid     = "3 Musketees";
const char* password = "ECE1710001";

// define two tasks for Sending data & ECG data collection
void TaskSendData( void *pvParameters );
void TaskSaveData( void *pvParameters );

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://sansisenseplus.com/ecg_project1.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";

String httpRequestData;


void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  ads.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(25,10);
  oled.print("Welcome");
  oled.display();
  delay(2000);
  oled.clearDisplay();
  
  
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
     TaskSendData //Function to be called
    ,  "Task Send Data" //Task name
    ,  40000            //Stack size in bytes
    ,  NULL             //parameter to pass into the function
    ,  1             //Task Priority (0-24)
    ,  &Task1           //Task handle
    ,  1);              //Core number(Protocol CPU:1,Application CPU:0,tskNO_AFFINITY)

  xTaskCreatePinnedToCore(
     TaskSaveData  //Function to be called
    , "Task Save Data" //Task name
    , 40000          //Stack size in bytes
    , NULL           //parameter to pass into the function
    , 1              //Task Priority (0-24)
    , &Task2         //Task handle
    , 1);            //Core number(Protocol CPU:1,Application CPU:0,tskNO_AFFINITY)

} 



void loop()
{ 
  delay(1000); 
}

void TaskSendData(void *pvParameters)
{  
  for( ; ; )
  { 
    (void) pvParameters;
    if(xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
    {
    DynamicJsonDocument doc(24576);
    doc["api_key"] = "1234"; 
    JsonArray value1 = doc.createNestedArray("value1");
    for(int i=0 ;i<1500 ;i++)
    {
     ads.setSPS(ADS1115_DR_860SPS);
     value1.add(ads.readADC_SingleEnded(0));
    }
    serializeJson(doc,httpRequestData);
    xSemaphoreGive(mutex);
    }
  }
}

void TaskSaveData(void *pvParameters)
{
   for( ; ; )
   {
    if(xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
    {
    (void) pvParameters;
     // Open serial communications and wait for port to open
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
    httpRequestData = "";
    xSemaphoreGive(mutex);
  }
 }
}
