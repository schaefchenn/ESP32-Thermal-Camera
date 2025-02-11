#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Thermal-Camera";  
const char* password = "toHotToHandle";  

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void handleDataRequest() {
  server.send(200, "application/json", getJSON());
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setupWifi() {
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
  
    server.on("/data", handleDataRequest);
    server.onNotFound(handle_NotFound);
  
    server.begin();
    Serial.println("HTTP server started");

}