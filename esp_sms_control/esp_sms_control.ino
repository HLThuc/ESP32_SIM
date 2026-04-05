#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP32

#include <WiFi.h>
#include <WebSocketsClient_Generic.h>
#include <ArduinoJson.h>

//////////////////////////////////////
// WiFi
//////////////////////////////////////

const char* ssid = "36 Thuong Duc";
const char* password = "0792065626";

//////////////////////////////////////
// WebSocket server
//////////////////////////////////////

const char* ws_host = "192.168.101.187";
const uint16_t ws_port = 3000;
const char* ws_path = "/";

WebSocketsClient webSocket;

//////////////////////////////////////
// LED
//////////////////////////////////////

#define LED 2

//////////////////////////////////////
// SIM A7680C UART
//////////////////////////////////////

#define MODEM_RX 16   // RX2
#define MODEM_TX 17   // TX2

HardwareSerial sim(2);

//////////////////////////////////////
// SEND SMS FUNCTION
//////////////////////////////////////

void sendSMS(String phone, String text)
{
  Serial.println("Sending SMS...");

  sim.println("AT+CMGF=1");
  delay(500);

  sim.print("AT+CMGS=\"");
  sim.print(phone);
  sim.println("\"");

  delay(500);

  sim.print(text);

  delay(200);

  sim.write(26); // Ctrl+Z

  Serial.println("SMS command sent");
}

//////////////////////////////////////
// WebSocket Event
//////////////////////////////////////

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch(type)
  {

    case WStype_CONNECTED:
      Serial.println("Connected to server");
      webSocket.sendTXT("esp32");
      break;

    case WStype_TEXT:
    {
      String msg = String((char*)payload);

      Serial.println("Received:");
      Serial.println(msg);

      //////////////////////////////////
      // LED control
      //////////////////////////////////

      if (msg == "ON")
      {
        digitalWrite(LED, HIGH);
        Serial.println("LED ON");
        return;
      }

      if (msg == "OFF")
      {
        digitalWrite(LED, LOW);
        Serial.println("LED OFF");
        return;
      }

      //////////////////////////////////
      // Parse JSON
      //////////////////////////////////

      StaticJsonDocument<256> doc;

      DeserializationError err =
        deserializeJson(doc, msg);

      if (err)
      {
        Serial.println("JSON parse failed");
        return;
      }

      String type = doc["type"];

      //////////////////////////////////
      // SMS command
      //////////////////////////////////

      if (type == "sms")
      {
        String phone = doc["phone"];
        String text  = doc["msg"];

        Serial.println("Phone:");
        Serial.println(phone);

        Serial.println("Message:");
        Serial.println(text);

        sendSMS(phone, text);
      }

      break;
    }

    case WStype_DISCONNECTED:
      Serial.println("Disconnected");
      break;

  }
}

//////////////////////////////////////
// SETUP
//////////////////////////////////////

void setup()
{
  Serial.begin(115200);

  //////////////////////////////////
  // LED
  //////////////////////////////////

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //////////////////////////////////
  // SIM UART
  //////////////////////////////////

  sim.begin(115200, SERIAL_8N1,
            MODEM_RX,
            MODEM_TX);

  delay(2000);

  Serial.println("SIM ready");

  //////////////////////////////////
  // WiFi
  //////////////////////////////////

  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP());

  //////////////////////////////////
  // WebSocket
  //////////////////////////////////

  webSocket.begin(
    ws_host,
    ws_port,
    ws_path
  );

  webSocket.onEvent(webSocketEvent);

}

//////////////////////////////////////
// LOOP
//////////////////////////////////////

void loop()
{
  webSocket.loop();
}