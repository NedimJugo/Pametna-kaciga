#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>       
#include <ESP8266WiFi.h>      
#include <Firebase_ESP_Client.h>


#define DHTPIN 4
#define DHTTYPE DHT11
#define RED_PIN 14
#define GREEN_PIN 13
#define BLUE_PIN 12
#define CONTROL_LED_PIN 5
#define MQ135_PIN A0
#define BUZZER_PIN 15

DHT dht(DHTPIN, DHTTYPE);

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "WiFi-SSID"
#define WIFI_PASSWORD "WiFi Password"

// Insert Firebase project API Key
#define API_KEY "Firebase API Key"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "Firebase database url" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false; 

void setup(){

  dht.begin();
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(CONTROL_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int airQualityValue = analogRead(MQ135_PIN);

  String ledColor;
    if (temperature < 23) {
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(RED_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        ledColor = "Zelena";
    } else if (temperature >= 23 && temperature < 25) {
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW); 
        ledColor = "Zuta";
    } else {
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
        ledColor = "Crvena";
    }
    
    // Write the LED color to Firebase
    if (Firebase.RTDB.setString(&fbdo, "DHT_11/LED_Color", ledColor.c_str())) {
        Serial.print("LED Color: ");
        Serial.println(ledColor);
    }

  
 
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Enter Temperature in to the DHT_11 Table
    if (Firebase.RTDB.setInt(&fbdo, "DHT_11/Temperature", temperature)){
      Serial.print("Temperature : ");
      Serial.println(temperature);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    
    if (Firebase.RTDB.setFloat(&fbdo, "DHT_11/Humidity", humidity)){
      Serial.print("Humidity : ");
      Serial.print(humidity);
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "DHT_11/Air_Quality", airQualityValue)) {
      Serial.print("Air Quality: ");
      Serial.println(airQualityValue);
    }
    
    if (Firebase.RTDB.getString(&fbdo, "DHT_11/LED_Control")) {
        String ledControlState = fbdo.stringData();
        
        if (ledControlState == "ON") {
            digitalWrite(CONTROL_LED_PIN, HIGH);  // Turn the LED on
        } else {
            digitalWrite(CONTROL_LED_PIN, LOW);   // Turn the LED off
        }
    } else {
        Serial.println("Failed to get LED control state");
        Serial.println(fbdo.errorReason());
    }
  }
  
  if (airQualityValue > 350) {
    tone(BUZZER_PIN, 2000);
  } else if (airQualityValue > 300){
    tone(BUZZER_PIN, 1000, 500);  
    delay(400);                   
  } else {
    noTone(BUZZER_PIN);           
  }
}
