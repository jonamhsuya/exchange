"use client";

import { useState } from "react";

export default function Home() {
  const [price, setPrice] = useState(0.0);
  const [quantity, setQuantity] = useState(100);
  const [side, setSide] = useState(0);

  const onSubmit = async () => {
    try {
      const res = await fetch("/api/submit", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ side, quantity, price }),
      });
      const data = await res.json();
      console.log(data);
    } catch (err) {
      console.error("Error sending order:", err);
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
