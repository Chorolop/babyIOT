/**
 * Lev 2020.8.24
 * 通过WIFI控制台灯亮灭 来源: https://zhuanlan.zhihu.com/p/42317263
 * 使用 ESP8266-CH340-nodemcu(A) / DC转3v3电源模块 / 电源 / 低电平继电器(B) / 台灯(C)
 * A-D3 -> B-IN
 * C-Neg -> B-COM
 * C-Pos -> B-NC
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "1603";
const char* password = "82480869";

ESP8266WebServer server(80);

String html = "<!DOCTYPE HTML><html lang=\"en\"><head> <title>Control Interface</title>  <meta charset=\"UTF-8\"></head><body> <a href=\"./pin?light=on\"><input type=\"button\" value=\"&#79;&#80;&#69;&#78;\" /></a> <a href=\"./pin?light=off\"><input type=\"button\" value=\"&#67;&#76;&#79;&#83;&#69;\" /></a></body></html>";

const int led = D3;

void pin(){
    if(server.arg("light") == "on"){
        digitalWrite(led, LOW);
        server.send(200, "text/html", html);
    }else if(server.arg("light") == "off"){
        digitalWrite(led, HIGH);
        server.send(200, "text/html", html);
    }
}

void handleRoot() {
    server.send(200, "text/html", html);
}

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void) {
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/pin", HTTP_GET, pin);
    server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
    server.handleClient();
}