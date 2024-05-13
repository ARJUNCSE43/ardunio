#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "11223344"
#define API_KEY "AIzaSyBFctmwmvsr8djXgUPD6PWnj42x5kdp6Yw"
#define DATABASE_URL "iot-3976a-default-rtdb.firebaseio.com" 

FirebaseData fbdo,fbdo2;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

String receivedData;

float value_V=0;
int value_A=0;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){



  if (Serial.available() > 0) {
    receivedData = Serial.readStringUntil('\n'); // Read the data until newline character
    // Split the data into two variables
    String variable1 = receivedData.substring(0, receivedData.indexOf(':'));
    String variable2 = receivedData.substring(receivedData.indexOf(':') + 1);
    
    // Print the variables
    Serial.print("Variable 1: ");
    Serial.println(variable1);
    Serial.print("Variable 2: ");
    Serial.println(variable2);
    //Firebase.RTDB.setString(&fbdo, "Data", receivedData);
    //delay(100);
    
    if(Firebase.RTDB.setFloat(&fbdo, "Value/V", variable1.toFloat())){
      delay(50);
      Firebase.RTDB.setString(&fbdo2, "Value/A", variable2.toInt());
    }
      
  }


//  if (Serial.available() > 0) {
//    char receivedChar = Serial.read();
//    if (receivedChar == 'V' && value_V!=Serial.parseFloat() && value_V<10) {
//      value_V = Serial.parseFloat();
//      Firebase.RTDB.setFloat(&fbdo, "Value/V", value_V );
//    }
//    
//    if (receivedChar == 'A' && value_A!=Serial.parseFloat()) {
//      value_A = Serial.parseInt();
//      
//      Firebase.RTDB.setInt(&fbdo, "Value/A", value_A );
//    }
//    
//      Serial.println("A: "+String(value_A) +"   V: "+String(value_V));
//  }

}
