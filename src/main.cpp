#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <creds.h>
#include <DHT.h>

char SSID[] = secret_SSID;
char PASS[] = secret_pass;

const char *fb_host = FB_HOST;
const char *fb_auth = FB_AUTH;
int led = 5;
int dhtpin = 2;
String fireStatus = "";

DHT dht(dhtpin, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(led, OUTPUT);
  pinMode(dhtpin, INPUT);
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Attempting to connect");
    WiFi.begin(SSID,PASS);
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.print(".");
    }
    Serial.print("Connected to IP: "); Serial.println(WiFi.localIP());
  }
  Firebase.begin(fb_host, fb_auth);
  Firebase.setString("LED_stats", "OFF");
  if(Firebase.success())
  {
    Serial.println("LED Turned OFF Successfully.");
  }
  else
  {
    Serial.println("Error while writing to FireBase RTDB");
  }
}

void loop() {
  // Get status of LED from the Firebase Database on the cloud.
  fireStatus = Firebase.getString("LED_stats");
  // Perform appropriate actions.
  if (fireStatus == "ON")
  {
    Serial.println("LED ON");
    digitalWrite(led, HIGH);
  }
  else if (fireStatus == "OFF")
  {
    Serial.println("LED OFF");
    digitalWrite(led, LOW);
  }
  else
  {
    Serial.println("Command Error!! Please send \"ON\" or \"OFF\"");
  }
  //read data from the dht sensor.
  float t = dht.readTemperature();
  //Check if sensor data is NAN, if not sensor isn't working, print appropriate text.
  if(isnan(t))
  {
    Serial.println("Failed to read from the DHT sensor");
    return;
  }

  //Send data to Firebase Database on the cloud.
  Firebase.setFloat("Temperature", t);
  //Check if it was successfully sent, send appropriate feedback text.
  if (Firebase.success())
  {
    Serial.println("Successfully uploaded Temperature data.");
  }
  else
  {
    Serial.println("Error in writing Temperature Data to Firebase RTDB.");
  }
  
}