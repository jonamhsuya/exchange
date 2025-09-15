interface OrderBookProps {
  bids: [number, number][];
  asks: [number, number][];
}

export default function OrderBook({ bids, asks }: OrderBookProps) {
  const maxQuantity = Math.max(
    ...bids.map(([_, qty]) => qty),
    ...asks.map(([_, qty]) => qty),
    1
  );

  const bestBid = bids.length ? Math.max(...bids.map(([price]) => price)) : 0;
  const bestAsk = asks.length ? Math.min(...asks.map(([price]) => price)) : 0;

  return (
    <div className="flex gap-8 p-6 bg-gray-900 text-white rounded-2xl shadow-lg w-full max-w-6xl mx-auto">
      <div className="flex-1">
        <h2 className="text-2xl font-bold text-green-400 text-center mb-4">Bids</h2>
        <table className="w-full table-auto border-collapse text-center">
          <thead>
            <tr className="text-green-300 border-b border-green-500">
              <th className="px-2 py-2">Price</th>
              <th className="px-2 py-2">Quantity</th>
            </tr>
          </thead>
          <tbody>
            {bids
              .slice()
              .sort((a, b) => b[0] - a[0])
              .map(([price, qty], idx) => {
                const width = (qty / maxQuantity) * 100;
                const isBest = price === bestBid;
                return (
                  <tr key={idx} className="relative">
                    <td className="px-2 py-2 relative font-bold">
                      <div
                        className={`absolute top-0 left-0 h-full rounded ${
                          isBest ? "bg-green-500/50" : "bg-green-500/20"
                        }`}
                        style={{ width: `${width}%` }}
                      ></div>
                      <span className="relative">{price.toFixed(2)}</span>
                    </td>
                    <td className="px-2 py-2">{Math.round(qty)}</td>
                  </tr>
                );
              })}
          </tbody>
        </table>
      </div>

      <div className="flex-1">
        <h2 className="text-2xl font-bold text-red-400 text-center mb-4">Asks</h2>
        <table className="w-full table-auto border-collapse text-center">
          <thead>
            <tr className="text-red-300 border-b border-red-500">
              <th className="px-2 py-2">Price</th>
              <th className="px-2 py-2">Quantity</th>
            </tr>
          </thead>
          <tbody>
            {asks
              .slice()
              .sort((a, b) => a[0] - b[0])
              .map(([price, qty], idx) => {
                const width = (qty / maxQuantity) * 100;
                const isBest = price === bestAsk;
                return (
                  <tr key={idx} className="relative">
                    <td className="px-2 py-2 relative font-bold">
                      <div
                        className={`absolute top-0 left-0 h-full rounded ${
                          isBest ? "bg-red-500/50" : "bg-red-500/20"
                        }`}
                        style={{ width: `${width}%` }}
                      ></div>
                      <span className="relative">{price.toFixed(2)}</span>
                    </td>
                    <td className="px-2 py-2">{Math.round(qty)}</td>
                  </tr>
                );
              })}
          </tbody>
        </table>
      </div>
    </div>
  );
}
