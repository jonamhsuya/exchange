"use client";

import { useState, useEffect, useRef } from "react";
import { buildRequest } from "./lib/buildRequest";
import { decodeResponse } from "./lib/decodeResponse";
import OrderBook from "./components/OrderBook";

export default function Home() {
  const [clientId, setClientId] = useState<number | null>(null);
  const [bids, setBids] = useState<[number, number][]>([]);
  const [asks, setAsks] = useState<[number, number][]>([]);
  const [price, setPrice] = useState<number | null>(null);
  const [quantity, setQuantity] = useState<number | null>(null);
  const [side, setSide] = useState(0);

  const ws = useRef<WebSocket | null>(null);
  const SERVER_URL = "localhost:9999";

  useEffect(() => {
    ws.current = new WebSocket("ws://" + SERVER_URL);

    ws.current.onopen = () => console.log("WebSocket connected");

    ws.current.onmessage = async (event: MessageEvent<Blob>) => {
      if (typeof event.data === "string") {
        const data = JSON.parse(event.data);
        setBids(data.bids);
        setAsks(data.asks);
        return;
      }
      const response = decodeResponse(await event.data.arrayBuffer());
      if (response?.kind === "idAssignment") setClientId(response.clientId);
      else if (response?.kind === "accept")
        alert("Order accepted: " + response.orderId);
      else if (response?.kind === "cancel")
        alert("Order cancelled: " + response.orderId);
      else
        alert(
          `${response?.quantity} shares of order ${response?.orderId} executed at ${response?.price}`
        );
    };

    ws.current.onclose = () => console.log("WebSocket disconnected");
    ws.current.onerror = (error) => console.error("WebSocket error:", error);

    return () => ws.current?.close();
  }, []);

  const onSubmit = () => {
    if (price === null || quantity === null || quantity <= 0)
      return alert("Enter valid price and quantity");
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      const orderId = Math.floor(Math.random() * 0xffffffff);
      const order = buildRequest(clientId ?? 0, orderId, side, quantity, price);
      ws.current.send(order);
    }
  };

  return (
    <main className="bg-gray-900 min-h-screen text-white flex flex-col">
      <nav className="flex items-center justify-center h-[15vh] bg-gray-800 shadow-md gap-4 px-4">
        <p className="text-5xl sm:text-6xl">📈</p>
        <p className="text-4xl sm:text-5xl font-serif font-bold">Exchange</p>
        <p className="text-5xl sm:text-6xl">📉</p>
      </nav>

      <div className="flex-1 flex flex-col items-center justify-start p-6 sm:p-12 gap-8 sm:gap-12 w-full">
        <div className="w-full max-w-6xl px-2 sm:px-0">
          <OrderBook bids={bids} asks={asks} />
        </div>

        <div className="flex flex-col lg:flex-row gap-6 sm:gap-12 w-full max-w-5xl justify-center px-2 sm:px-0">
          <div className="flex flex-col items-center gap-2 w-full lg:w-auto">
            <label className="text-lg sm:text-2xl font-bold">Price</label>
            <input
              type="number"
              step="0.01"
              value={price ?? ""}
              onChange={(e) => {
                const raw = e.target.value;
                if (raw === "") setPrice(null);
                else {
                  const value = parseFloat(raw);
                  if (!isNaN(value) && value >= 0) {
                    if (raw.includes(".")) {
                      const [, decimals] = raw.split(".");
                      if (decimals.length <= 2) setPrice(value);
                    } else setPrice(value);
                  }
                }
              }}
              className="w-full lg:w-52 p-3 sm:p-4 rounded-lg border border-gray-600 bg-gray-800 text-white text-lg sm:text-xl focus:outline-none focus:ring-2 focus:ring-green-400 transition-all"
              placeholder="Price"
            />
          </div>

          <div className="flex flex-col items-center gap-2 w-full lg:w-auto">
            <label className="text-lg sm:text-2xl font-bold">Quantity</label>
            <input
              type="number"
              step="1"
              value={quantity ?? ""}
              onKeyDown={(e) => {
                if (e.key === "." || e.key === ",") e.preventDefault();
              }}
              onChange={(e) => {
                if (e.target.value === "") setQuantity(null);
                else {
                  const value = parseFloat(e.target.value);
                  if (value >= 0) setQuantity(Math.floor(value));
                }
              }}
              className="w-full lg:w-52 p-3 sm:p-4 rounded-lg border border-gray-600 bg-gray-800 text-white text-lg sm:text-xl focus:outline-none focus:ring-2 focus:ring-green-400 transition-all"
              placeholder="Quantity"
            />
          </div>

          <div className="flex flex-col items-center gap-2 w-full lg:w-auto">
            <label className="text-lg sm:text-2xl font-bold">Side</label>
            <select
              value={side}
              onChange={(e) => setSide(parseInt(e.target.value))}
              className="w-full lg:w-52 p-3 sm:p-4 rounded-lg border border-gray-600 bg-gray-800 text-white text-lg sm:text-xl focus:outline-none focus:ring-2 focus:ring-green-400 transition-all"
            >
              <option value={0}>BUY</option>
              <option value={1}>SELL</option>
            </select>
          </div>
        </div>

        <button
          onClick={onSubmit}
          className="mt-6 w-full max-w-xs lg:max-w-sm px-6 py-3 rounded-xl bg-green-500 text-black font-bold text-lg transition-all duration-300 transform hover:bg-green-600 hover:scale-105"
        >
          Submit
        </button>
      </div>
    </main>
  );
}
