

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>     //
// Define the connections pins:
#define CLK 22                     
#define DIO 23
// Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);              

const char *ssid     = "TOPNET_Karim_Ext";
const char *password = "ksmk@050703";


const long utcOffsetInSeconds = 3600;  //Tunisia time zone is GMT+1 = 1*60*60 = 3600seconds difference


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

uint32_t chipId = 0;



void setup(){ 
  Serial.begin(115200);
   // Clear the display:
  display.clear();
  
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    Serial.print("Conntect to ");
    Serial.println(ssid);
    Serial.println("ESP IP");
    Serial.println(WiFi.localIP());
    Serial.println("MAC Address");
    Serial.println(WiFi.macAddress());
    Serial.println(timeClient.getHours());
    Serial.println(timeClient.getMinutes());

  }
  
  
  
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
