#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;     // Use this for the 16-bit version 
TaskHandle_t Task1 = NULL;

DynamicJsonDocument doc(24576);
JsonArray value1 = doc.createNestedArray("value1");


// Replace with your network credentials
const char* ssid     = "3 Musketees";
const char* password = "ECE1710001";

// define two tasks for Sending data & ECG data collection
void TaskSendData( void *pvParameters );


// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://sansisenseplus.com/ecg_project1.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";

int i = 0;
String httpRequestData;


void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  ads.begin();
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
  // Now set up two tasks to run independently.
    xTaskCreatePinnedToCore(
     TaskSendData
    ,  "Task Send Data"
    ,  40000
    ,  NULL
    ,  1  //Priority
    ,  &Task1
    ,  1);  

         
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
    ads.setSPS(ADS1115_DR_860SPS);
    if( i == 0)
    {
     doc["api_key"] = "1234"; 
    }
    value1.add(ads.readADC_SingleEnded(0));
    Serial.println(ads.readADC_SingleEnded(0)); 
    Serial.println(i);
    i++ ;
    //Serial.println(ads.readADC_SingleEnded(0));
    if( i == 1500)
    {
     serializeJson(doc,httpRequestData);
     serializeJson(doc,Serial);  
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
      Serial.print("HTTP Response code: ");
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
    i=0;
   }
  }
 }
