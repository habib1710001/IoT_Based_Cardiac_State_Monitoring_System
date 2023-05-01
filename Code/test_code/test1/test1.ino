#include <Wire.h>
#include <Adafruit_ADS1015.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <ArduinoJson.h>

Adafruit_ADS1115 ads;     // Use this for the 16-bit version 
TaskHandle_t Task1, Task2;
SemaphoreHandle_t baton;

// Replace with your network credentials
const char* ssid     = "Hoque menson";
const char* password = "longwarrior";

// REPLACE with your Domain name and URL path or IP address with path
//const char* serverName = "http://iotbasedecg.info/ecg_project.php";
const char* serverName = "http://ecg-real-time.000webhostapp.com//ecg_project1.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";

String sensorName = "AD8232";
String sensorLocation = "Hospital";
float time_lap=0;
int ecgarray[10];

int16_t adc0;

// define two tasks for Blink & AnalogRead
void TaskSendData( void *pvParameters );
void TaskEcgRead( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
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

  baton = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    TaskEcgRead
    ,  "Task Ecg Read"
    ,  10000  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  &Task1 
    ,  1);

  xTaskCreatePinnedToCore(
     TaskSendData
    ,  "Task Send Data"
    ,  20000
    ,  NULL
    ,  1
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
  (void) pvParameters;
   xSemaphoreTake( baton, portMAX_DELAY);
   for (int j=0;j<10;j++)
   {
    ads.setSPS(ADS1115_DR_860SPS);
    adc0 = ads.readADC_SingleEnded(0);
    ecgarray[j] = adc0;
    Serial.println(adc0);
    delay(10);
   }
   xSemaphoreGive( baton );
   delay(50);
   vTaskDelete(NULL);
}

void TaskSendData(void *pvParameters)
{
     (void) pvParameters;
     xSemaphoreTake( baton, portMAX_DELAY );
     
     StaticJsonDocument<256> doc;
     doc["api_key"] = "1234";
     JsonArray value1 = doc.createNestedArray("value1");
     for (int i=0;i<10;i++)
     {
      value1.add(ecgarray[i]);
     }
     if(WiFi.status()== WL_CONNECTED){
     HTTPClient http;
    
     // Your Domain name with URL path or IP address with path
     http.begin(serverName);
    
     // Specify content-type header
     http.addHeader("Content-Type", "application/json");

     String httpRequestData;
 
     serializeJson(doc,httpRequestData);
     serializeJson(doc,Serial);
    
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
   else {
    Serial.println("WiFi Disconnected");
   }
    xSemaphoreGive( baton );
    delay(50);
 }
