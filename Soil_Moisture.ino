#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int sensorPin = A0;
int sensorPinRed = D2;
int sensorPinGreen = D3;

const char* ssid = "kku-wifi@true";
const char* pass = NULL;
String sensor_id = "0";
const char *host = "nrdev.kku.ac.th";
const int httpsPort = 443;
String Link = "/soil0";
const char fingerprint[] PROGMEM = "60 7F F1 02 28 6C 22 61 6A F0 8B 50 66 43 06 C3 D5 B3 67 BC";
String payload;

void setup() {

  Serial.begin(9600); // set up serial port for 9600 baud (speed)
  delay(500); // wait for display to boot up
  pinMode(sensorPinRed, OUTPUT);
  pinMode(sensorPinGreen, OUTPUT);

  Blynk.begin(auth, "vivo 1801", "12345678");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  

}

void loop() {

 int sensorValue = 0;
  String Humidity = "";
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue, 0, 1023, 0, 100);
  Humidity += sensorValue ;  
  Serial.print("Soil moisture: ");
  Serial.print(Humidity);
  Serial.println(" %");

  if (sensorValue < 50 ){
    digitalWrite(sensorPinGreen, LOW);
    digitalWrite(sensorPinRed, HIGH);  
  }else {
    digitalWrite(sensorPinRed, LOW);
    digitalWrite(sensorPinGreen, HIGH);
  }
  
  delay(1000); 

  
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  httpsClient.connect(host, httpsPort);

  payload = "[{\"id\":"+sensor_id+",\"soilnull\":"+Humidity+",\"soil\":"+Humidity+"}]";

  if ((httpsClient.print(String("POST ")
                         + Link + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Content-Type: application/json" + "\r\n" +
                         "Content-Length: 36" + "\r\n\r\n" +
                         payload + "\r\n" +
                         "Connection: close\r\n\r\n")))
  {
    Serial.println("POST-OK!!!");
      //Serial.println(Humidity);
  }
  else
  {
    Serial.println("POST-Failed!!!");
  }
  delay(1000);
    //Serial.println(Humidity);
  Blynk.run();
}
