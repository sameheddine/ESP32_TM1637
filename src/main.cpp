#include <NTPClient.h>
//#include <WiFi.h>
//#include <WiFiUdp.h>
#include <TM1637Display.h>     //
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
// Define the connections pins to TM1637Display:
#define CLK 22                     
#define DIO 23

TM1637Display display = TM1637Display(CLK, DIO);              // Create display object of type TM1637Display:

const char *ssid     = "TOPNET_Karim_Ext";
const char *password = "ksmk@050703";
#define WIFI_TIMEOUT_MS 2000

//Define Time Zone
int valeurTimeZone = 0;
const long utcOffsetInSeconds = valeurTimeZone;  //Tunisia time zone is GMT+1 = 1*60*60 = 3600seconds difference
    


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

uint32_t chipId = 0;
// Define LED et capteur 
const int led = 2;
const int capteurLuminosite = 34;
bool etatLed = 0;
bool etatLedVoulu = 0;

AsyncWebServer server(80);

void setup(){ 
  //Serial
  Serial.begin(115200);
  Serial.println("\n");
  
  //GPIO
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(capteurLuminosite, INPUT);
  
  //SPIFFS
   if (!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while (file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }
  
  //WIFI
  WiFi.begin(ssid, password);

  Serial.println("Connection attempt ...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Conntect to ");
    Serial.println(ssid);
    Serial.println("ESP IP");
    Serial.println(WiFi.localIP());
    Serial.println("MAC Address");
    Serial.println(WiFi.macAddress());
  
  //SERVER
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
    
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(led, HIGH);
    request->send(204);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(led, LOW);
    request->send(204);
  });
 
  server.on("/timeZone", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    String message;
    if(request->hasParam("valeurTimeZone", true))
    {
      message = request ->getParam("valeurTimeZone", true)->value();
      valeurTimeZone = message.toInt();
    }
    request->send(204);
    Serial.println(valeurTimeZone);

    
  });
  server.begin();
  Serial.println("Serveur actif!");

   // Clear the display:
  display.clear();
  timeClient.begin();
  
  
}

void loop() {
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
 
}
