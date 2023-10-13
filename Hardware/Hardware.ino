#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>


// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "IoT Lab"
#define WIFI_PASSWORD "IoT@123456"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 3. Define the RTDB URL */
#define DATABASE_URL "dht11-1ca81-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app



// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#define MQ2_A A0 // define MQ2 analog pin
#define MQ2_D D0 // define MQ2 digital pin
#define LED D1
#define DHTPIN 14    // Chân dữ liệu của DHT 11 , với NodeMCU chân D5 GPIO là 14
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int A_value, D_value;

bool lamp1 = false;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{

  Serial.begin(115200);
  pinMode(MQ2_A, INPUT);
  pinMode(MQ2_D, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  dht.begin();
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  config.signer.test_mode = true;
  /* Assign the callback function for the long running token generation task */

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);

}

void loop()
{
  A_value = analogRead(MQ2_A);
  D_value = digitalRead(MQ2_D);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (millis() - sendDataPrevMillis > 2000 )
  {
    sendDataPrevMillis = millis();

    // Serial.print("Nhiet do: ");
    // Serial.print(t);
    // Serial.print("*C "); 
    Serial.printf("Nhiet do: %f°C \n", t);
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, "/Nhiet do", t) ? "ok" : fbdo.errorReason().c_str());
    // Serial.print("Do am: ");
    // Serial.print(h);
    // Serial.print("% ");
    Serial.printf("Do am: %f% \n", h);
    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, "/Do am", h) ? "ok" : fbdo.errorReason().c_str());


    Serial.printf("Gas: %d\n", A_value);
    Serial.printf("Digital Value: %d\n", D_value);
    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/Khi gas", A_value) ? "ok" : fbdo.errorReason().c_str());
  }

    Serial.printf("Set bool... %s\n", Firebase.getBool(fbdo, F("/device/lamp1"), &lamp1) ? "ok" : fbdo.errorReason().c_str());

  if(lamp1){
      digitalWrite(LED, HIGH);
      Serial.printf("Lamp: ON\n");
  }
  else {
      digitalWrite(LED, LOW);
      Serial.printf("Lamp: OFF\n");
  }

}

