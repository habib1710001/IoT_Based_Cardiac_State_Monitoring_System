#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1015.h>
#include "WiFi.h"
#include "EEPROM.h"
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#define UpperThreshold 11000
#define LowerThreshold 10000
#define LENGTH(x) (strlen(x) + 1)   // length of char string//Macro
#define EEPROM_SIZE 200             // EEPROM size
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D

String ssid;                        //string variable to store ssid
String password;                    //string variable to store password
int httpResponseCode = 1;

Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64
Adafruit_ADS1115 ads;     // Use this for the 16-bit version 
TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;
TaskHandle_t Task3 = NULL;

int ecg_array[1500];
int j; 

static SemaphoreHandle_t mutex;


// define two tasks for Sending data & ECG data collection
void TaskSendData( void *pvParameters );
void TaskSaveData( void *pvParameters );
void TaskShowData( void *pvParameters );

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://kusumtara.com/receiveData.php";
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";
String httpRequestData;

int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;

void writeStringToFlash(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}


String readStringFromFlash(int startAddr) {
  char in[128]; // char array of size 128 for reading the stored data 
  int i = 0;
  for (; i < 128; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}

void setup()
{ 
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  ads.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.writeFillRect(0,50,128,16,BLACK);
  oled.setCursor(5,0);
  oled.print("IoT Based");
  oled.setCursor(42,30);
  oled.print("ECG");
  oled.display();
  delay(2000);
  oled.clearDisplay();

  //Init EEPROM
  if (!EEPROM.begin(EEPROM_SIZE)) 
  {
    Serial.println("failed to init EEPROM");
    delay(1000);
  }
  else
  {
    ssid = readStringFromFlash(0); // Read SSID stored at address 0
    Serial.print("SSID = ");
    Serial.println(ssid);
    password = readStringFromFlash(40); // Read Password stored at address 40
    Serial.print("passwords = ");
    Serial.println(password);
  }

  WiFi.begin(ssid.c_str(), password.c_str());

  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.writeFillRect(0,50,128,16,BLACK);
  oled.setCursor(0,0);
  oled.print("Connecting");
  oled.setCursor(0,30);
  oled.print("to web....");
  oled.display();
  delay(2000);
  oled.clearDisplay();

  delay(3500);   // Wait for a while till ESP connects to WiFi

  if (WiFi.status() != WL_CONNECTED) // if WiFi is not connected
  { 
   oled.clearDisplay();
   oled.setTextSize(2);
   oled.setTextColor(WHITE);
   oled.writeFillRect(0,50,128,16,BLACK);
   oled.setCursor(0,0);
   oled.print("Failed..");
   oled.setCursor(0,30);
   oled.print("Give Info.");
   oled.display();
   delay(2000);
   
   oled.clearDisplay();
    //Init WiFi as Station, start SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");

    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi Connected.");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // read the connected WiFi SSID and password
    ssid = WiFi.SSID();
    password = WiFi.psk();
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("Password:");
    Serial.println(password);
    Serial.println("Store SSID & Password in Flash");
    writeStringToFlash(ssid.c_str(), 0); // storing ssid at address 0
    writeStringToFlash(password.c_str(), 40); // storing password at address 40
  }
  else
  {
    Serial.println("WiFi Connected");
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(WHITE);
    oled.writeFillRect(0,50,128,16,BLACK);
    oled.setCursor(0,0);
    oled.print("Connected");
    oled.setCursor(0,30);
    oled.print("to wifi...");
    oled.display();
    delay(2000);
    oled.clearDisplay();
  }

  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
     TaskSendData //Function to be called
    ,  "Task Send Data" //Task name
    ,  40000            //Stack size in bytes
    ,  NULL             //parameter to pass into the function
    ,  1                //Task Priority (0-24)
    ,  &Task1           //Task handle
    ,  0);              //Core number(Protocol CPU:1,Application CPU:0,tskNO_AFFINITY)

  xTaskCreatePinnedToCore(
     TaskSaveData      //Function to be called
    , "Task Save Data" //Task name
    , 40000            //Stack size in bytes
    , NULL             //parameter to pass into the function
    , 1                //Task Priority (0-24)
    , &Task2           //Task handle
    , 0);              //Core number(Protocol CPU:1,Application CPU:0,tskNO_AFFINITY)

  xTaskCreatePinnedToCore(
     TaskShowData   //Function to be called
    , "TaskShowData" //Task name
    , 1024          //Stack size in bytes
    , NULL          //parameter to pass into the function
    , 1             //Task Priority (0-24)
    , &Task3        //Task handle
    , 1);           //Core number(Protocol CPU:1,Application CPU:0,tskNO_AFFINITY)
} 

void loop()
{ 
  delay(1000); 
}

void TaskSendData(void *pvParameters)
{  
  for( ; ; )
  { 
    (void)pvParameters;
    if(xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
    {
      DynamicJsonDocument doc(24576);
      doc["api_key"] = "1234"; 
      JsonArray value1 = doc.createNestedArray("value1");
      unsigned long past_time = millis();
      for(int i=0 ;i<1500 ;i++)
      {
        ads.setSPS(ADS1115_DR_860SPS);
        value1.add(ads.readADC_SingleEnded(0));
        Serial.println(ads.readADC_SingleEnded(0)); 
        ecg_array[i] = ads.readADC_SingleEnded(0);
      }
    Serial.print("Sampling time:");
    Serial.print(millis() - past_time);
    serializeJson(doc,httpRequestData);
    Serial.println(httpRequestData);
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
    (void)pvParameters;
     // Open serial communications and wait for port to open
     if(WiFi.status()== WL_CONNECTED)
     {
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);
    
      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
     
      //send HTTP POST request
      httpResponseCode = http.POST(httpRequestData);
     
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


void TaskShowData(void *pvParameters)
{  
  for( ; ; )
  { 
   (void) pvParameters;
    if(a>127)
    {
      oled.clearDisplay();
      a=0;
      lasta=a;
    }

    if (j == 1500)
    {
     j =  0 ;  
    }
    ThisTime=millis();
    
    int value = ecg_array[j];
    oled.setTextColor(WHITE);
    int b=45-(value/300);
    oled.writeLine(lasta,lastb,a,b,WHITE);
    lastb=b;
    lasta=a;
 
    if(value>UpperThreshold)
    {
    if(BeatComplete)
    {
      BPM=ThisTime-LastTime;
      BPM=int(60000/BPM);
      BPMTiming=false;
      BeatComplete=false;
    }
    if(BPMTiming==false)
    {
      LastTime=millis();
      BPMTiming=true;
    }
   }
   if((value<LowerThreshold)&&(BPMTiming))
   BeatComplete=true;
 
   oled.writeFillRect(0,50,128,16,BLACK);
   oled.setCursor(0,50);
   oled.print("BPM:");
   oled.print(BPM);
 
   oled.display();
   a++;
   j++;
  }
 }
