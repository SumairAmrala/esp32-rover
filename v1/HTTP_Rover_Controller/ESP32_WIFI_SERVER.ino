//ESP32S3

#include <WiFi.h>

#define RXD2 16
#define TXD2 17

const char* ssid = "SSID";
const char* password = "PASSWORD";

const int ESP32_TX_TO_ARDUINO = 17;
const int ESP32_RX_FROM_ARDUINO = 16;

float roverBatteryV = 0;
float roverBatteryPercent = 0;
char roverGear = 'P';

WiFiServer server(80);

char command = 'S';


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, ESP32_RX_FROM_ARDUINO, ESP32_TX_TO_ARDUINO);
  Serial1.setTimeout(10);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");

}

void loop() {
  readArduinoTelemetry();
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println();
  Serial.println("New client connected");


  String requestLine = client.readStringUntil('\n');
  requestLine.trim();

  Serial.print("Request line: ");
  Serial.println(requestLine);

  if (requestLine == "GET /F HTTP/1.1") {
    command = 'F';
    Serial.println(command);
    Serial1.write(command);
  }
  else if (requestLine == "GET /R HTTP/1.1") {
    command = 'R';
    Serial.println(command);
    Serial1.write(command);
  }
  else if (requestLine == "GET /S HTTP/1.1") {
    command = 'S';
    Serial.println(command);
    Serial1.write(command);
  }

  while (client.available()) {
    client.read();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<html>");
  client.println("<body>");

  client.println("<head>");
  client.println("<meta http-equiv='refresh' content='1'>");
  client.println("</head>");

  client.println("<h1>Rover Control</h1>");

  client.print("<p>Command: ");
  client.print(command);
  client.println("</p>");

  client.print("<p>Gear: ");
  client.print(roverGear);
  client.println("</p>");

  client.print("<p>Battery Voltage: ");
  client.print(roverBatteryV, 2);
  client.println("V</p>");

  client.print("<p>Battery Percent: ");
  client.print(roverBatteryPercent, 1);
  client.println("%</p>");

  client.println("<br>");

  client.println("<a href='/F'>Forward</a><br>");
  client.println("<a href='/R'>Reverse</a><br>");
  client.println("<a href='/S'>Stop</a><br>");

  client.println("</body>");
  client.println("</html>");

  delay(10);
  client.stop();

  Serial.println("Client disconnected");

}

void readArduinoTelemetry(){
  if (Serial1.available()){
    String line = Serial1.readStringUntil('\n');
    line.trim();

    if (line.startsWith("T,")){
      int firstComma = line.indexOf(',');
      int secondComma = line.indexOf(',', firstComma + 1);
      int thirdComma = line.indexOf(',', secondComma + 1);

      String voltageText = line.substring(firstComma + 1, secondComma);
      String percentText = line.substring(secondComma + 1, thirdComma);
      String gearText = line.substring(thirdComma + 1);

      roverBatteryV = voltageText.toFloat();
      roverBatteryPercent = percentText.toFloat();
      roverGear = gearText.charAt(0);
    }
    Serial.print("Telemetry received: ");
    Serial.println(line);
  }
}