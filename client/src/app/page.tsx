"use client";

import { useState, useEffect, useRef } from "react";
import { buildRequest } from "./lib/buildRequest";
import { decodeResponse } from "./lib/decodeResponse";

export default function Home() {
  const [clientId, setClientId] = useState<number | null>(null);
  const [price, setPrice] = useState(0.0);
  const [quantity, setQuantity] = useState(100);
  const [side, setSide] = useState(0);

  const ws = useRef<WebSocket | null>(null);

  const SERVER_URL = "localhost:9999";

  useEffect(() => {
    ws.current = new WebSocket("ws://" + SERVER_URL);

    ws.current.onopen = () => {
      console.log("WebSocket connected");
    };

    ws.current.onmessage = async (event: MessageEvent<Blob>) => {
      const response = decodeResponse(await event.data.arrayBuffer());
      console.log("Message from server:", response);
      if (response?.kind === "idAssignment") {
        setClientId(response.clientId);
      }
      console.log(Date.now());
    };

    ws.current.onclose = () => {
      console.log("WebSocket disconnected");
    };

    ws.current.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    return () => {
      ws.current?.close();
    };
  }, []);

  const onSubmit = () => {
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      const orderId = Math.floor(Math.random() * 0xffffffff);
      console.log("Generated orderId:", orderId);
      const order = buildRequest(clientId ?? 0, orderId, side, quantity, price);
      ws.current.send(order);
      console.log("Sent order:", order);
      console.log(Date.now());
    } else {
      console.error("WebSocket is not connected");
    }
  };

  return (
    <main className="p-10">
      <input
        type="number"
        value={price}
        onChange={(e) => setPrice(parseFloat(e.target.value))}
        className="border-black border-1 w-40 m-10 p-2"
        placeholder="Price"
      />
      <input
        type="number"
        value={quantity}
        onChange={(e) => setQuantity(parseInt(e.target.value))}
        className="border-black border-1 w-40 m-10 p-2"
        placeholder="Quantity"
      />
      <select
        value={side}
        onChange={(e) => setSide(parseInt(e.target.value))}
        className="border-black border-1 w-40 m-10 p-2"
      >
        <option value={0}>BUY</option>
        <option value={1}>SELL</option>
      </select>
      <button
        className="border-black border-1 w-40 m-10 p-2 hover:bg-gray-200"
        onClick={onSubmit}
      >
        Submit
      </button>
    </main>
  );
}
