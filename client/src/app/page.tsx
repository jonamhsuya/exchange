"use client";

import { useState, useEffect, useRef } from "react";
import { buildRequest } from "./lib/buildRequest";
import { decodeResponse } from "./lib/decodeResponse";

export default function Home() {
  const [clientId, setClientId] = useState<number | null>(null);
  const [price, setPrice] = useState<number | null>(null);
  const [quantity, setQuantity] = useState<number | null>(null);
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
        alert("Client ID Assigned: " + response.clientId);
      } else if (response?.kind == "accept") {
        alert("Order accepted: " + response.orderId);
      } else if (response?.kind == "cancel") {
        alert("Order cancelled: " + response.orderId);
      } else {
        alert(
          response?.quantity +
            " shares of order " +
            response?.orderId +
            " executed at " +
            response?.price
        );
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
    if (price === null) {
      alert("Please enter a valid price.");
      return;
    } else if (quantity === null || quantity <= 0) {
      alert("Please enter a valid quantity.");
      return;
    }
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
    <main>
      <nav className="flex flex-row items-center justify-center h-[15vh] bg-gradient-to-b from-green-300 to-green-400 shadow-lg">
        <p className="text-6xl">📈&nbsp;&nbsp;</p>
        <p className="text-5xl font-serif font-bold">Exchange</p>
        <p className="text-6xl">&nbsp;&nbsp;📉</p>
      </nav>
      <div className="flex flex-col items-center justify-center bg-gradient-to-b from-green-500 to-yellow-300 p-10 w-screen h-[85vh]">
        <div className="flex flex-row items-center justify-center gap-20">
          <div className="flex flex-col items-center justify-center">
            <p className="text-white text-4xl font-bold drop-shadow-[0_2px_2px_rgba(0,0,0,1)]">
              Price
            </p>
            <input
              type="number"
              step="0.01"
              value={price ?? ""}
              onChange={(e) => {
                const raw = e.target.value;
                if (raw === "") {
                  setPrice(null);
                  return;
                }
                const value = parseFloat(raw);
                if (isNaN(value) || value < 0) {
                  setPrice(0);
                  return;
                }
                if (raw.includes(".")) {
                  const [, decimals] = raw.split(".");
                  if (decimals.length > 2) {
                    return;
                  }
                }
                setPrice(value);
              }}
              className="border-black border-1 shadow-lg w-40 m-10 p-2 bg-white rounded-lg hover:w-50 hover:p-3 hover:duration-200 not-hover:duration-200"
              placeholder="Price"
            />
          </div>
          <div className="flex flex-col items-center justify-center">
            <p className="text-white text-outline text-4xl font-bold drop-shadow-[0_2px_2px_rgba(0,0,0,1)]">
              Quantity
            </p>
            <input
              type="number"
              step="1"
              value={quantity ?? ""}
              onKeyDown={(e) => {
                if (e.key === "." || e.key === ",") {
                  e.preventDefault();
                }
              }}
              onChange={(e) => {
                if (e.target.value === "") {
                  setQuantity(null);
                  return;
                }
                const value = parseFloat(e.target.value);
                if (value < 0) {
                  setQuantity(0);
                  return;
                }
                if (Math.floor(value * 100) === value * 100) {
                  setQuantity(parseFloat(e.target.value));
                }
              }}
              className="border-black border-1 shadow-lg w-40 m-10 p-2 bg-white rounded-lg hover:w-50 hover:p-3 hover:duration-200 not-hover:duration-200"
              placeholder="Quantity"
            />
          </div>
          <div className="flex flex-col items-center justify-center">
            <p className="text-white text-4xl font-bold drop-shadow-[0_2px_2px_rgba(0,0,0,1)]">
              Side
            </p>
            <select
              value={side}
              onChange={(e) => setSide(parseInt(e.target.value))}
              className="border-black border-1 shadow-lg w-40 m-10 p-2 bg-white rounded-lg hover:w-50 hover:p-3 hover:duration-200 not-hover:duration-200"
            >
              <option value={0}>BUY</option>
              <option value={1}>SELL</option>
            </select>
          </div>
        </div>
        <div className="flex flex-row items-center justify-center">
          <button
            className="border-black border-1 shadow-lg w-60 m-20 p-4 bg-gradient-to-b from-lime-100 to-yellow-100 rounded-xl hover:w-75 hover:p-5 hover:duration-200 not-hover:duration-200"
            onClick={onSubmit}
          >
            <p className="text-3xl font-bold">Submit</p>
          </button>
        </div>
      </div>
    </main>
  );
}
