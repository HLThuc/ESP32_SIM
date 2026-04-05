const express = require("express");
const http = require("http");
const WebSocket = require("ws");

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

app.use(express.static("public"));

let espClient = null;

// WebSocket connection
wss.on("connection", (ws) => {
  console.log("Client connected");

  ws.on("message", (message) => {
    const msg = message.toString();
    console.log("Received:", msg);

    // ESP32 identify
    if (msg === "esp32") {
      espClient = ws;
      console.log("ESP32 registered");
      return;
    }

    // from UI
    if (espClient) {
      espClient.send(msg);
      console.log("Forward to ESP:", msg);
    }
  });

  ws.on("close", () => {
    console.log("Client disconnected");
    if (ws === espClient) {
      espClient = null;
    }
  });
});

server.listen(3000, "0.0.0.0", () => {
  console.log("Server running on:");
  console.log("http://localhost:3000");
});