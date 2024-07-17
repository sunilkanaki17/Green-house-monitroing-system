//Include the library files
//#include <LiquidCrystal_I2C.h>
#define BLYNK_TEMPLATE_ID "TMPL33D7wrmar"
#define BLYNK_TEMPLATE_NAME "Green House"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


#define DHTPIN 2 //Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

//Initialize the LCD display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "z0UFKsg4_Wut4Ukec1BWB_rRSlfIz4TS";//Enter your Auth token
char ssid[] ="Pixel";//Enter your WIFI name
char pass[] = "Sunil@17";//Enter your WIFI password

BlynkTimer timer;
bool mode = 0;

//Define component pins
const int LDR = D0;
const int LED = D5;
#define sensor A0
#define waterPump D3
#define fan D7
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V2, t);
    //Blynk.virtualWrite(V1, h);
    Serial.print("Temperature : ");
    Serial.println(t);
    if(t > 33)
   {
    digitalWrite(fan, LOW);
    Blynk.virtualWrite(V3, 1);
    delay(1000);
  }
  else 
  {
    digitalWrite(fan, HIGH);
    Blynk.virtualWrite(V3, 0);
    delay(1000);
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(LDR,  INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(waterPump, HIGH);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  //Call the function
  timer.setInterval(500L, soilMoistureSensor);
  timer.setInterval(500L, sendSensor);
  timer.setInterval(500L, LDRsensor);
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);

 if(value > 22)
  {
    digitalWrite(waterPump, HIGH);
    Blynk.virtualWrite(V1, 0);
    delay(1000);
  }
  else 
  {
    digitalWrite(waterPump, LOW);
    Blynk.virtualWrite(V1, 1);
    delay(1000);
  }
  Serial.print("Moisture :");
  Serial.print(value);
  Serial.print(" ");

}

void LDRsensor() {

  // put your main code here, to run repeatedly:
  if(digitalRead(LDR)==0) //NIGHT
  {
    digitalWrite(LED,HIGH);
    Blynk.virtualWrite(V4, 1);
    delay(2000);
    //igitalWrite(LED,LOW);
  }
  else
  {
    digitalWrite(LED,LOW);
    Blynk.virtualWrite(V4, 0);
    delay(2000);
  }
  Serial.println(digitalRead(LDR));
  delay(1000);
}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}



