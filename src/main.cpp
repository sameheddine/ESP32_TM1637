

#include <NTPClient.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <TM1637Display.h>

// Define the connections pins:
#define CLK 22                     
#define DIO 23
// Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);              

// Network credentials
const char *ssid     = "TOPNET_Karim_Ext";
const char *password = "ksmk@050703";

// Define LED et capteur 
const int led = 2;
const int capteurLuminosite = 34;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

AsyncWebServer server(80);


void setup(){
  // Initialize Serial Monitor
  Serial.begin(115200);

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

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("Conntect to ");
  Serial.println(ssid);
  Serial.println("ESP IP: ");
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

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  //Tunisia time zone is GMT+1 = 1*60*60 = 3600seconds difference
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);
  display.clear();
}

void loop() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  int A,B;
    
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
