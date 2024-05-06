#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

const char *ssid = "Rujul";
const char *password = "12345678Rujul";

WebServer server(80);
DHT dht(26, DHT11);

void handleRoot() {
  char msg[2000]; // Increased buffer size for HTML content

  // Read temperature from DHT11 sensor
  float temperatureC = readDHTTemperature();

  // Calculate Respiratory Health Index
  float respiratoryHealthIndex = 100 - temperatureC;

  int heartRate = random(70, 85);

  snprintf(msg, 2000,
           "<!DOCTYPE html>\
<html lang='en'>\
<head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>ESP32 DHT Server</title>\
    <link href='https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap' rel='stylesheet'>\
    <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css'>\
    <style>\
        body { font-family: 'Roboto', sans-serif; background-color: #f5f5f5; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }\
        .container { max-width: 500px; background-color: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }\
        h1 { text-align: center; margin-top: 0; color: #333; }\
        .data { display: flex; flex-direction: column; align-items: center; margin-bottom: 15px; }\
        .icon { font-size: 24px; }\
        .value { font-size: 24px; font-weight: bold; color: #555; }\
        .unit { font-size: 16px; color: #777; }\
        .label { font-size: 18px; color: #888; margin-bottom: 5px; }\
    </style>\
</head>\
<body>\
    <div class='container'>\
        <h1>ESP32 DHT Server</h1>\
        <div class='data'>\
            <span class='label'>Body Temperature</span>\
            <div>\
                <i class='fas fa-thermometer-half icon' style='color:#ca3517;'></i>\
                <span class='value'>%.2f</span>\
                <span class='unit'>&deg;C</span>\
            </div>\
        </div>\
        <div class='data'>\
            <span class='label'>Heart Rate</span>\
            <div>\
                <i class='fas fa-heartbeat icon' style='color:#f7347a;'></i>\
                <span class='value'>%d</span>\
                <span class='unit'>BPM</span>\
            </div>\
        </div>\
        <div class='data'>\
            <span class='label'>Respiratory Health Index</span>\
            <div>\
                <i class='fas fa-heartbeat icon' style='color:#f7347a;'></i>\
                <span class='value'>%.2f</span>\
            </div>\
        </div>\
    </div>\
</body>\
</html>",
           temperatureC, heartRate, respiratoryHealthIndex
          );
  server.send(200, "text/html", msg);
}

void setup(void) {

  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2); // Allow the CPU to switch to other tasks
}

float readDHTTemperature() {
  // Sensor readings may also take up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}
