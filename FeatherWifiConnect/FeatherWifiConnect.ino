/*
 *  Simple HTTP get webclient test
 */
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

const char* ssid     = "WIFI NAME";
const char* password = "WIFI PASS";

const char* host = "wifitest.adafruit.com";

void setup() {
  ArduinoOTA();
  ArduinoOTA.begin();
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {

    Serial.println("Connection Failed! Rebooting...");

    delay(5000);

    ESP.restart();

  }
  ArduinoOTA.setHostname("Unicorn");
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
  ArduinoOTA.onStart([]() {

    String type;

    if (ArduinoOTA.getCommand() == U_FLASH)

      type = "sketch";

    else // U_SPIFFS

      type = "filesystem";



    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()

    Serial.println("Start updating " + type);
  });
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

 ArduinoOTA.onEnd([]() {

    Serial.println("\nEnd");

  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));

  });

  ArduinoOTA.onError([](ota_error_t error) {

    Serial.printf("Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");

    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");

    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");

    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");

    else if (error == OTA_END_ERROR) Serial.println("End Failed");

  });


  Serial.println("Ready");

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());

}

int value = 0;

void loop() {

  ArduinoOTA.handle();
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
