#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "YourWiFiNetworkName";
const char* password = "YourWiFiPassword";

const char* host = "www.googleapis.com";
const int httpsPort = 443;

const char* channelId = "YourYouTubeChannelID";
const char* apiKey = "YourYouTubeAPIKey";

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Fetch the subscriber count when a command is received
  Serial.println("Ready to receive commands");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "get_subscriber_count") {
      String subscriberCount = fetchSubscriberCount();
      Serial.println("Subscriber count: " + subscriberCount);
    }
  }
}

String fetchSubscriberCount() {
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return "";
  }

  String url = "/youtube/v3/channels?part=statistics&id=" + String(channelId) + "&key=" + String(apiKey);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();

    if (line == "\"subscriberCount\": {") {
      String subscriberCountLine = client.readStringUntil('\n');
      subscriberCountLine.trim();

      int colonIndex = subscriberCountLine.indexOf(':');
      int commaIndex = subscriberCountLine.indexOf(',');

      if (colonIndex >= 0 && commaIndex >= 0) {
        String subscriberCount = subscriberCountLine.substring(colonIndex + 1, commaIndex);
        return subscriberCount;
      }
    }
  }

  Serial.println("Failed to fetch subscriber count");
  return "";
}
