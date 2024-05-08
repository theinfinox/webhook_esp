#include <ESP8266WiFi.h>

const char* ssid = "pi";
const char* password = "raspberry";

const char* Event_Name = "fingerprint";
const char* Key = "/db_test/index.php";
const char* server = "192.168.0.88";
const int serverPort = 80;

const int buttonPin = 2; // Pin connected to the button
bool buttonState = false; // Variable to store the button state

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial.println("\n\nAdafruit finger detect test");
  
  connectToWiFi();
}

void loop() {
  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // If button is pressed, simulate fingerprint detection and make webhook request
  if (buttonState == LOW) {
    Serial.println("Button pressed!");
    simulateFingerprintDetection();
  }

  delay(100); // Delay for debouncing
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void simulateFingerprintDetection() {
  Serial.println("Simulating fingerprint detection...");
  const char* NAME = "Sachin";
  const char* ID = "1";
  Serial.print("Attendance Marked for "); 
  Serial.println(NAME);
  makewebhookRequest(NAME, ID);
}

void makewebhookRequest(const char* NAME, const char* ID) {
  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print("...");

  WiFiClient client;
  if (!client.connect(server, serverPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("connected");

  String url = String(Key);
  String data = String("{\"FingerID\":\"123456\"}");

  // Calculate Content-Length
  int contentLength = data.length();

  // Print the request
  Serial.println("Sending HTTP request:");
  Serial.print("POST ");
  Serial.print(url);
  Serial.println(" HTTP/1.1");
  Serial.print("Host: ");
  Serial.println(server);
  Serial.println("Connection: close");
  Serial.println("Content-Type: application/json");
  Serial.print("Content-Length: ");
  Serial.println(contentLength);
  Serial.println();
  Serial.println(data);

  // Send the request
  client.print(String("POST ") + url + " HTTP/1.1\r\n");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(contentLength);
  client.println();
  client.println(data);

  // Wait for server to respond
  delay(500);

  // Read response
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}
