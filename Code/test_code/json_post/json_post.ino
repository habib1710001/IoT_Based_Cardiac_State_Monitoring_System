#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <ArduinoJson.h>
// Replace with your network credentials
const char* ssid     = "3 Musketees";
const char* password = "ECE1710001";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://sansisenseplus.com/ecg_project1.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "1234";
int i;

void setup()
{ 
  Serial.begin(115200);
  // initialize serial communication at 115200 bits per second:
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(xPortGetFreeHeapSize());
  
  
}


void loop(){

     DynamicJsonDocument doc(24576);
     //StaticJsonDocument<6144> doc;
     doc["api_key"] = "1234";
     JsonArray value1 = doc.createNestedArray("value1");
     for (i=0;i<1500;i++)
     {
      value1.add(i);
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
}
