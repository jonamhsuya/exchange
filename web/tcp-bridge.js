import express from "express";
import bodyParser from "body-parser";
import net from "net";
import cors from "cors";

const app = express();
app.use(bodyParser.json());
app.use(
  cors({
    origin: "http://localhost:3000", // your React app origin
  })
);

const HOST = "127.0.0.1";
const PORT = 9999;

// Keep a single TCP connection open
const client = new net.Socket();
client.connect(PORT, HOST, () => {
  console.log("Connected to C++ TCP server");
});

client.on("error", (err) => {
  console.error("TCP Error:", err);
});

// Endpoint to receive orders from React
app.post("/send-order", (req, res) => {
  const { side, quantity, price } = req.body;

  const buffer = Buffer.alloc(16);

  // Header
  buffer.writeUInt16LE(16, 0);
  buffer.writeUInt8(side, 2);

  // Payload
  buffer.writeUInt32LE(quantity, 4);
  buffer.writeUInt32LE(Math.floor(price * 100), 8);

  // Checksum
  let checksum = 0;
  for (let i = 0; i < 15; i++) checksum ^= buffer[i];
  buffer.writeUInt8(checksum, 15);

  client.write(buffer);
  res.json({ status: "Order sent" });
});

app.listen(3001, () => {
  console.log("React → TCP bridge running on http://localhost:3001");
});
