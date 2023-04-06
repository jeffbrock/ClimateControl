#include <Ethernet.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT1_PIN        14
#define DHT2_PIN        12
#define HEAT_PIN        13
#define HUM_PIN         0

#define DHTTYPE         DHT22   
#define LOCAL_SSID      "soup"
#define PASSWORD        "mrpants4848"
#define HOST            "192.168.1.22"
#define PORT            9000

DHT dhtInternal_1 = DHT(DHT1_PIN, DHTTYPE);
DHT dhtInternal_2 = DHT(DHT2_PIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;

bool useWifi = true;
int setTemp = 76;
int setHumidity = 76;

struct reading {
   int humidityPercent;
   int tempFar;
};

void setup()
{
  Serial.begin(9600);
  pinMode(HEAT_PIN, OUTPUT);
  pinMode(HUM_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  dhtInternal_1.begin();
  dhtInternal_2.begin();
  initWifi(); 
}

void loop()
{  
  writeLcd("Reading...", "");
  reading loopRead = ReadEnvironment();
  delay(1000);    
  
  char environmentReading[200];
  sprintf(environmentReading, "TemperatureFahrenheit=%s&HumidityPercent=%s", (String)loopRead.tempFar, (String)loopRead.humidityPercent);   

  if (loopRead.tempFar <= setTemp ) digitalWrite(HEAT_PIN, HIGH);
  else digitalWrite(HEAT_PIN, LOW);
  
  if (loopRead.humidityPercent <= setHumidity ) digitalWrite(HUM_PIN, HIGH);
  else digitalWrite(HUM_PIN, LOW);
  
  String heatState = "OFF";
  if (loopRead.tempFar <= setTemp) heatState = "ON";
  String humState = "OFF";
  if (loopRead.humidityPercent <= setHumidity) humState = "ON";
  String lcdLine1 = (String)"T: " + loopRead.tempFar + (char)223 + " (" + (String)setTemp + (char)223 +  ") " + heatState;
  String lcdLine2 = (String)"H: " + loopRead.humidityPercent +  "% (" + (String)setHumidity + "%) " + humState;
  writeLcd(lcdLine1, lcdLine2);
 
  delay(55000);

  if (useWifi) 
  {
    writeLcd("Posting reading", (String)HOST + ":" + PORT);
    postReading(environmentReading); 
    delay(4000);
  }
}

struct reading ReadEnvironment()
{
  struct reading thisReading;
  float humidityPercent1 = dhtInternal_1.readHumidity();
  float tempC1 = dhtInternal_1.readTemperature();
  float humidityPercent2 = dhtInternal_2.readHumidity();
  float tempC2 = dhtInternal_2.readTemperature();  
  thisReading.humidityPercent = (int)((humidityPercent1 + humidityPercent2) / 2 + 0.5);
  thisReading.tempFar = (int)((((tempC1 + tempC2) / 2) * 1.8 + 32) + 0.5);
  return thisReading;
}

void initLcd() 
{
  Wire.begin();
  lcd.init();  
  lcd.backlight(); 
}

void writeLcd(String line1, String line2)
{
  Serial.println(line1);    
  Serial.println(line2);    
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print(line1);
  lcd.setCursor(0, 1); 
  lcd.print(line2);
}

void initWifi() 
{
  if (useWifi)
  {
    writeLcd("Connecting wifi...", LOCAL_SSID);
    WiFi.begin(LOCAL_SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) delay(500);  
    writeLcd("Connected", LOCAL_SSID);
    delay(2000);
  }
}

void postReading(char* reading)
{    
  if(client.connect(HOST, PORT)) 
  { 
    int bodyLength = strlen(reading);
    client.println("POST /api HTTP/1.1");
    client.print("Host: "); 
    client.println(HOST);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(bodyLength);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println();
    client.print(reading);
    delay(100);
    while(client.read() != -1);
  }
}