#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Server-Pre-Setup
ESP8266WebServer server(80);
void handlePulse();
void handleColour();
void handle404();

void setup() {
  //WiFi-Setup
  Serial.begin(38400);
  WiFi.begin("SSID", "PASSWORD");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //Server-Setup
  server.on("/pulse", HTTP_POST, handlePulse);
  server.on("/colour", HTTP_POST, handleColour);
  server.onNotFound(handle404);
  
  server.begin();

  //LED-Setup
  #define REDPIN 4
  #define GREENPIN 5
  #define BLUEPIN 0

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  //Turn off internal LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void pulse(int cycle){
  analogWrite(REDPIN, cycle);
  analogWrite(GREENPIN, cycle);
  analogWrite(BLUEPIN, cycle);
}

void handlePulse(){
  if (!server.hasArg("pulse") || server.arg("pulse") == NULL){
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }
  pulse(server.arg("pulse").toInt());
  server.send(200);
}

void setColour(int r, int g, int b){
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, b);
}

void handleColour(){
  if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") ||
      server.arg("r") == NULL || server.arg("g") == NULL || server.arg("b") == NULL){
        server.send(400, "text/plain", "400: Invalid Request");
        return;
      }
  setColour(
    server.arg("r").toInt(),
    server.arg("g").toInt(),
    server.arg("b").toInt()
  );
  server.send(200);
}

void handle404(){
  server.send(404, "text/plain", "404: Not found");
}

void loop(){
  server.handleClient();
}
