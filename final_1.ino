/* Comment this out to disable prints and save space */

#define BLYNK_TEMPLATE_ID "TMPL661XgC0Ff"
#define BLYNK_TEMPLATE_NAME "IoT Project"
#define BLYNK_AUTH_TOKEN "KxSUf8efpqNSZWjB62Pgi90kxW3gxvCE"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <DHT.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ADMIN 2640";
char pass[] = "12345678";


// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(4, 5); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

#define DHTPIN 2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

#define gas A0
#define SENSOR_PIN 7

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int gasLevel = analogRead(gas);

  int sensorValue = digitalRead(SENSOR_PIN);

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V4, gasLevel);

  if (sensorValue == HIGH && t<27.00)
  {
    Blynk.virtualWrite(V3, "Turn on heater!");
    
    //Relay is low level triggered relay so we need to write LOW to switch on the light
  }
  else
  {
    Blynk.virtualWrite(V3, "Turn off the heater!");
  }

}

void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(1000);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  pinMode(gas,INPUT);
  pinMode(SENSOR_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(5000L, sendSensor);
}


void loop()
{
  Blynk.run();
  timer.run();
}
