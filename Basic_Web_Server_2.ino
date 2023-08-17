

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "SHT21.h"

ESP8266WebServer server(80);          
SHT21 SHT21;                            

const char* ssid     = "soheil";         
const char* password = "10691069";

#define     ledPin     D3
#define     relayPin   D5

String      htmlPage;                  

void setup() {
  Serial.begin(9600);

  Wire.begin();

  digitalWrite(ledPin, LOW);            
  pinMode(ledPin, OUTPUT);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);         
 

 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                  
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  
  server.on("/", handleRoot);     
  server.on("/LEDTOGGLE", handleLEDToggle);  
  server.on("/LEDON", handleLEDOn);
  server.on("/LEDOFF", handleLEDOff);
  server.on("/RELAYTOGGLE", handleRelayToggle);  
  server.on("/RELAYON", handleRelayOn);
  server.on("/RELAYOFF", handleRelayOff);
  server.onNotFound(handleNotFound);        

  server.begin();                          
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                   
}



void handleRoot() {
  buildHtmlPage();
  server.send(200, "text/html", htmlPage);
}


void handleLEDToggle() {                          
  digitalWrite(ledPin, !digitalRead(ledPin));     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}


void handleLEDOn() {                          
  digitalWrite(ledPin, HIGH);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}


void handleLEDOff() {                          
  digitalWrite(ledPin, LOW);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}


void handleRelayToggle() {                         
  digitalWrite(relayPin, !digitalRead(relayPin));     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}


void handleRelayOn() {                          
  digitalWrite(relayPin, LOW);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}

void handleRelayOff() {                         
  digitalWrite(relayPin, HIGH);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}


void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); 
}


void buildHtmlPage() {
  htmlPage = "<!DOCTYPE html>";
  htmlPage += "<html>";
  htmlPage += "<head>";                                    
  htmlPage += "<title>ESP8266 Web Server</title>";             
  htmlPage += "<meta http-equiv=\"refresh\" content=\"10\">";  
  htmlPage += "<meta charset=\"UTF-8\">";                    
  htmlPage += "</head>";
  
  htmlPage += "<BODY bgcolor='#E0E0D0'>";                     


  htmlPage += "<br>Temperature °C: " + String(SHT21.getTemperature());
  htmlPage += "<br>Humidity   %RH: " + String(SHT21.getHumidity());
  htmlPage += "<br>";


  String ledState = ((digitalRead(ledPin)) ? "on" : "off");
  htmlPage += "<br>LED: " + ledState;
  htmlPage += "<form action=\"/LEDON\" method=\"POST\"><input type=\"submit\" value=\"LED On\"></form>";
  htmlPage += "<form action=\"/LEDOFF\" method=\"POST\"><input type=\"submit\" value=\"LED Off\"></form>";
  htmlPage += "<form action=\"/LEDTOGGLE\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>";

 
  String relayState = ((!digitalRead(relayPin)) ? "on" : "off");
  htmlPage += "<br>Relay: " + relayState;
  htmlPage += "<form action=\"/RELAYON\" method=\"POST\"><input type=\"submit\" value=\"Relay On\"></form>";
  htmlPage += "<form action=\"/RELAYOFF\" method=\"POST\"><input type=\"submit\" value=\"Relay Off\"></form>";
  htmlPage += "<form action=\"/RELAYTOGGLE\" method=\"POST\"><input type=\"submit\" value=\"Toggle Relay\"></form>";

  htmlPage += "</body>";
  htmlPage += "</html>";
}
