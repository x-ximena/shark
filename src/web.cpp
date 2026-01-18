#include "web.h"
#include "config.h"
#include "secrets.h"

#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket(81);

static void handleRoot()
{
    File f = LittleFS.open("/index.html", "r");
    if (!f)
    {
        server.send(500, "text/plain", "Missing /index.html");
        return;
    }
    server.streamFile(f, "text/html");
    f.close();
}

static void sendLedStateToAll()
{
    webSocket.broadcastTXT(digitalRead(ledPin) == HIGH ? "ON" : "OFF");
}

// WebSocket event handler
static void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        Serial.printf("WS client %u connected\n", num);
        sendLedStateToAll();
        break;

    case WStype_TEXT:
    {
        String msg = String((char *)payload).substring(0, length);
        Serial.printf("WS recv: %s\n", msg.c_str());

        if (msg == "H")
        {
            on = true;
            stop0 = stop1 = stop2 = false;
            webSocket.broadcastTXT("RUNNING");
            digitalWrite(ledPin, HIGH);
            sendLedStateToAll();
        }
        else if (msg == "L")
        {
            on = false;
            webSocket.broadcastTXT("STOPPING");
            digitalWrite(ledPin, LOW);
            sendLedStateToAll();
        }
        else if (msg == "S")
        {
            webSocket.broadcastTXT(on ? "RUNNING" : "STOPPED");
            sendLedStateToAll();
        }
        break;
    }

    case WStype_DISCONNECTED:
        Serial.printf("WS client %u disconnected\n", num);
        break;

    default:
        break;
    }
}

// Initialize web server and WebSocket
void webInit()
{
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Open: http://");
    Serial.println(WiFi.localIP());

    // Start LittleFS
    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS mount failed");
    }
    else
    {
        Serial.println("LittleFS mounted");
    }

    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        Serial.print("FS: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
    root.close();

    // Setup HTTP server and WebSocket
    server.on("/", handleRoot);
    server.begin();

    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);

    Serial.println("HTTP (80) + WebSocket (81) started.");
}

// Main web loop
void webLoop()
{
    server.handleClient();
    webSocket.loop();
}
