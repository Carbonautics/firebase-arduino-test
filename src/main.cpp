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
int dpin = 2;
String fireStatus = "";

DHT dht(dpin, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(led, OUTPUT);
  pinMode(dpin, INPUT);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  fireStatus = Firebase.getString("LED_stats");
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

  float t = dht.readTemperature();

  if(isnan(t))
  {
    Serial.println("Failed to read from the DHT sensor");
    return;
  }
  
}