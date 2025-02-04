#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "YOUR_FIREBASE_DATABASE_URL"

#define TCRT5000_PIN D3
#define LED_PIN D2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
bool lastSensorState = HIGH;  // Initial sensor state (no object)
unsigned long lastUpdateTime = 0;  // Timer for limiting Firebase updates
const unsigned long updateInterval = 5000;  // Minimum interval between updates (5 seconds)
unsigned long ledTurnOffTime = 0;  // Timer for turning off the LED

void setup() {
  Serial.begin(115200);

  pinMode(TCRT5000_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially

  Serial.println("Pokretanje senzora...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Povezivanje na Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Povezano! IP adresa: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase povezivanje uspješno");
    signupOK = true;
  } else {
    Serial.printf("Firebase povezivanje neuspješno: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK) {
    int currentSensorState = digitalRead(TCRT5000_PIN);

    if (currentSensorState != lastSensorState || (millis() - lastUpdateTime > updateInterval)) {
      lastUpdateTime = millis(); 

      if (currentSensorState == LOW) {
        if (Firebase.RTDB.setString(&fbdo, "/Mailbox/status", "Pismo detektovano")) {
          Serial.println("Pismo detektovano - Poslano Firebase-u");
        } else {
          Serial.println("Greška pri slanju u Firebase:");
          Serial.println(fbdo.errorReason());
        }
      } else {
        if (Firebase.RTDB.setString(&fbdo, "/Mailbox/status", "Sanduče je prazno")) {
          Serial.println("Sanduče je prazno - Poslano Firebase-u");
        } else {
          Serial.println("Greška pri slanju u Firebase:");
          Serial.println(fbdo.errorReason());
        }
      }

      lastSensorState = currentSensorState;
    }

    if (Firebase.RTDB.getString(&fbdo, "/Mailbox/status")) {
      String mailboxStatus = fbdo.stringData();
      if (mailboxStatus == "Pismo detektovano") {
        Serial.println("Primljeno: Pismo detektovano");
        digitalWrite(LED_PIN, HIGH);  // Turn on LED
        ledTurnOffTime = millis() + 3000;
      }
    } else {
      Serial.println("Greška pri čitanju iz Firebase:");
      Serial.println(fbdo.errorReason());
    }
  }

  if (ledTurnOffTime > 0 && millis() > ledTurnOffTime) {
    digitalWrite(LED_PIN, LOW);
    ledTurnOffTime = 0;
  }
}
