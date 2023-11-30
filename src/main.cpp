/*
  Rui Santos
  Complete project details
   - Arduino IDE: https://RandomNerdTutorials.com/esp32-ota-over-the-air-arduino/
   - VS Code: https://RandomNerdTutorials.com/esp32-ota-over-the-air-vs-code/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include "time.h"

// Replace with your network credentials
const char *ssid = "General_Yang";
const char *password = "8587056243";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Set number of outputs
#define NUM_OUTPUTS 4

// Assign each GPIO to an output
int outputGPIOs[NUM_OUTPUTS] = {2, 4, 12, 14};

// Initialize LittleFS
void initLittleFS()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");

  File file = LittleFS.open("/test_example.txt");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// String getOutputStates(){
//   JSONVar myArray;
//   for (int i =0; i<NUM_OUTPUTS; i++){
//     myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
//     myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
//   }
//   String jsonString = JSON.stringify(myArray);
//   return jsonString;
// }

void notifyClients(String state)
{
  ws.textAll(state);
}

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
//   AwsFrameInfo *info = (AwsFrameInfo*)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
//     data[len] = 0;
//     if (strcmp((char*)data, "states") == 0) {
//       notifyClients(getOutputStates());
//     }
//     else{
//       int gpio = atoi((char*)data);
//       digitalWrite(gpio, !digitalRead(gpio));
//       notifyClients(getOutputStates());
//     }
//   }
// }

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    //      handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String jsondata;

String processor(const String &var)
{
  if (var == "jsondata")
  {
    return jsondata;
  }
  return String();
}

DynamicJsonDocument doc(1000);

void onRequest(AsyncWebServerRequest *request) {}
void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {}
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DeserializationError error = deserializeJson(doc, (char*)data);
  if(!error) {
    String jsondata;
    serializeJson(doc, jsondata);

    Serial.println("post");
    Serial.println(jsondata);

    File file = LittleFS.open("/params6.json", "w");
    if(!file) {
      Serial.println("Failed to open file for reading");
      request->send(500, "application/json", "{\"message\" : \"Error writing file\"}");
    }
    if(serializeJson(doc, file)==0){
      Serial.println(F("Failed to write to file"));
    };
    file.close();
    request->send(200, "application/json", "{\"message\":\"notes successfully posted\"}");
  }
}

void onBody2(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DeserializationError error = deserializeJson(doc, (char*)data);
  if(!error) {
    String jsondata;
    serializeJson(doc, jsondata);

    Serial.println("post");
    Serial.println(jsondata);

    File file = LittleFS.open("/params7.json", "w");
    if(!file) {
      Serial.println("Failed to open file for reading");
      request->send(500, "application/json", "{\"message\" : \"Error writing file\"}");
    }
    if(serializeJson(doc, file)==0){
      Serial.println(F("Failed to write to file"));
    };
    file.close();
    request->send(200, "application/json", "{\"message\":\"notes successfully posted\"}");
  }
}

void onBody3(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DeserializationError error = deserializeJson(doc, (char*)data);
  // Serial.println("post1");
  
  if(!error) {
    String jsondata;
    serializeJson(doc, jsondata);

    // Serial.println("post2: no error");
    // Serial.println(jsondata);

    File file = LittleFS.open("/params5.json", "w");
    if(!file) {
      Serial.println("Failed to open file for reading");
      request->send(500, "application/json", "{\"message\" : \"Error writing file\"}");
    }
    if(serializeJson(doc, file)==0){
      Serial.println(F("Failed to write to file"));
    };
    file.close();

    // Read JSON file
    file = LittleFS.open("/params5.json", "r");
    if (!file) {
      Serial.println("Failed to open data.json file");
      request->send(500, "application/json", "{\"message\" : \"Error writing file\"}");
      return;
    }

    // Read the content of the file
    jsondata = file.readString();
    file.close();
    // Serial.println(jsondata);

    // request->send(LittleFS, "/index.html", String(), false, processor);
    // request->send(200, "application/json", "{\"message\":\"notes successfully posted\"}");
      // request->send(200, "application/json", "{\"timerSetData\":\"1234\"}");
      request->send(200, "application/json", jsondata);

  }
  request->send(500, "application/json", "{\"message\" : \"Error writing file\"}");

}
// ref: https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const char* ntpServer = "pool.ntp.org";

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

String getNtpTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return String();
  }
  char formattedTime[100];
  strftime(formattedTime, sizeof(formattedTime), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  return String(formattedTime);
}

unsigned long Get_Epoch_Time() {
  time_t now;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void initNtpTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Set GPIOs as outputs
  for (int i = 0; i < NUM_OUTPUTS; i++)
  {
    pinMode(outputGPIOs[i], OUTPUT);
  }
  initLittleFS();
  initWiFi();
  initWebSocket();

  initNtpTime();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {

    // Read JSON file
    File file = LittleFS.open("/params5.json", "r");
    if (!file) {
      Serial.println("Failed to open data.json file");
      request->send(500, "text/plain", "Internal Server Error");
      return;
    }

    // Read the content of the file
    jsondata = file.readString();
    file.close();
    Serial.println(jsondata);

    request->send(LittleFS, "/index.html", String(), false, processor); });

  server.on("/timerset", HTTP_POST, onRequest, onFileUpload, onBody3);


  //  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //      request->send(LittleFS, "/index.html", "text/html",false);
  //  });

  server.on("/getdata", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    File file = LittleFS.open("/params5.json");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    String fileContent = file.readString();

    file.close();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, fileContent);

    if (error) {
      Serial.println("Failed to parse JSON");
      return;
    }

    const char* value = doc["timerSetData"]["c1d0p0o0"];
    Serial.print("Value from JSON: ");
    Serial.println(value);
    String json;
    serializeJson(doc, json);

    request->send(200, "application/json", json); });

  server.on("/description_notes", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("get");

    File file = LittleFS.open("/params6.json","r");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    String fileContent = file.readString();
    file.close();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, fileContent);

    if(error) {
      Serial.println("Failed to parse JSON");
      return;
    }
    Serial.println(fileContent);

    String jsondata;
    serializeJson(doc, jsondata);
    Serial.println(jsondata);
    // String jsondata = file.readString();
    // file.close();
    request->send(200, "application/json", jsondata); });

  server.on("/description_notes", HTTP_POST, onRequest, onFileUpload, onBody);

  server.on("/savehelptext", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    File file = LittleFS.open("/params7.json");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    String fileContent = file.readString();
    file.close();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, fileContent);

    if(error) {
      Serial.println("Failed to parse JSON");
      return;
    }

    String jsondata;
    serializeJson(doc, jsondata);
    Serial.println(jsondata);

    request->send(200, "application/json", jsondata); });

  server.on("/savehelptext", HTTP_POST, onRequest, onFileUpload, onBody2);

  // Serve HTML page with NTP time
  server.on("/get_time2", HTTP_GET, [](AsyncWebServerRequest *request){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char formattedTime[30];
    strftime(formattedTime, sizeof(formattedTime), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    
    String html = "<html><body>";
    html += "<h1>NTP Time</h1>";
    html += "<p>" + String(formattedTime) + "</p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });


  server.on("/get_time", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("get_time");
    DynamicJsonDocument doc(20);
    String jsondata;

    doc["time"]=Get_Epoch_Time();
    serializeJson(doc, jsondata);

    Serial.println(jsondata);
    request->send(200, "application/json", jsondata); 

    // request->send(200, "application/json", "{\"time\":\"1701359641\"}"); 
  });


  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

  server.serveStatic("/", LittleFS, "/");

  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);

  // Start server
  server.begin();
}

void loop()
{
  ws.cleanupClients();
}
