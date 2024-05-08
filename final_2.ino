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


#define trig 12
#define echo 11


BlynkTimer timer;


void sendSensor()
{
  long t = 0, h = 0, hp = 0;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  t = pulseIn(echo, HIGH);

  // Calculating distance 
  h = t / 58;
 
  h = h - 2;  // offset correction

  h = 15 - h;
  hp = 6.67 * h;  // water height
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V7, hp);

}

void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(1000);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 


  // Setup a function to be called every second
  timer.setInterval(5000L, sendSensor);
}


void loop()
{
  Blynk.run();
  timer.run();
}
