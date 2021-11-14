#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <NTPClient.h>
#include <TM1637Display.h>
//Wifi credentials
const char *ssid = "TOPNET_Karim_Ext";
const char *password = "ksmk@050703";

//Managemnt LED
const int led = 2;
const int capteurLuminosite = 34;
int valDelayLed = 1000;
bool etatLed = 0;
bool etatLedVoulu = 0;
int previousMillis = 0;

// Define the connections pins to TM1637Display:
#define CLK 22                     
#define DIO 23

 // Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);            

//Define Time Zone
int valUserTZ = 3600; //GMT+1 = 1*60*60 = 3600 seconds difference
const long utcOffsetInSeconds = valUserTZ;  

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS mounted successfully");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    Serial.print("File: ");
    Serial.println(file.name());
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");
  initWiFi();
  initSPIFFS();
  //----------------------------------------------------GPIO
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(capteurLuminosite, INPUT);

  //----------------------------------------------------WIFI
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\n");
  Serial.println("Connexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/jquery-3.4.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/jquery-3.4.1.min.js", "text/javascript");
  });

  server.on("/lireLuminosite", HTTP_GET, [](AsyncWebServerRequest *request) {
    int val = analogRead(capteurLuminosite);
    String luminosite = String(val);
    request->send(200, "text/plain", luminosite);
    //Serial.println(luminosite);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    etatLedVoulu = 1;
    request->send(204);
    Serial.println("LED ON");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    etatLedVoulu = 0;
    digitalWrite(led, LOW);
    etatLed = 0;
    request->send(204);
    Serial.println("LED OFF");
  });

  server.on("/delayLed", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasParam("valDelayLed", true))
    {
      String msgDelayLED;
      msgDelayLED = request->getParam("valDelayLed", true)->value();
      valDelayLed = msgDelayLED.toInt();
    }
    request->send(204);
    Serial.print("Time Blinking: ");
    Serial.println(valDelayLed);
  });

  server.on("/timezone", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasParam("valUserTZ", true))
    {
      String msgTimeZone;
      msgTimeZone = request->getParam("valUserTZ", true)->value();
      valUserTZ = msgTimeZone.toInt();
    }
    request->send(204);
    Serial.print("Time Zone: ");
    Serial.println(valUserTZ);
  });

  server.begin();
  Serial.println("Serveur actif!");
  // Clear the display:
  display.clear();
  timeClient.begin();
}

void loop(){
  //Time 
  int A,B;
  
  timeClient.update();
  display.setBrightness(7);                   // Set the brightness:
  
  A = timeClient.getHours() * 100 + timeClient.getMinutes();
  B = timeClient.getSeconds();
  
  if((B % 2) == 0)
  {
    display.showNumberDecEx(A, 0b01000000 , false, 4, 0); 
  }
  else
  {
    display.showNumberDecEx(A, 0b00000000 , false, 4, 0); 
  }
  //Blinking speed
  
  if(etatLedVoulu)
  {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= valDelayLed)
    {
      previousMillis = currentMillis;

      etatLed = !etatLed;
      digitalWrite(led, etatLed);
    }
  }
}