export type OrderStatus =
  | "PENDING"
  | "ACCEPTED"
  | "CANCELLING"
  | "PARTIALLY_FILLED";

export type OrderData = {
  price: number;
  quantity: number;
  side: number;
  status: OrderStatus;
  filled: number;
};

interface OrderTableProps {
  orders: Map<number, OrderData>;
  onCancel?: (orderId: number) => void;
}

export default function OrdersTable({ orders, onCancel }: OrderTableProps) {
  const orderArray = Array.from(orders.entries());

  return (
    <div className="w-full max-w-6xl mx-auto p-4 bg-gray-900 rounded-2xl shadow-lg text-white">
      <h2 className="text-2xl font-bold text-center mb-4">My Orders</h2>
      <table className="w-full table-auto text-center border-collapse">
        <thead>
          <tr className="border-b border-gray-600">
            <th className="px-2 py-2">Order ID</th>
            <th className="px-2 py-2">Price</th>
            <th className="px-2 py-2">Quantity</th>
            <th className="px-2 py-2">Side</th>
            <th className="px-2 py-2">Status</th>
            <th className="px-2 py-2">Filled</th>
            <th className="px-2 py-2">Actions</th>
          </tr>
        </thead>
        <tbody>
          {orderArray?.map(([orderId, data]) => (
            <tr key={orderId} className="border-b border-gray-700">
              <td className="px-2 py-1">{orderId}</td>
              <td className="px-2 py-1 font-bold">{data.price?.toFixed(2)}</td>
              <td className="px-2 py-1">{data.quantity}</td>
              <td className="px-2 py-1">{data.side === 0 ? "BUY" : "SELL"}</td>
              <td className="px-2 py-1">{data.status}</td>
              <td className="px-2 py-1">{data.filled}</td>
              <td className="px-2 py-1">
                <button
                  onClick={() => onCancel && onCancel(orderId)}
                  disabled={
                    data.status === "CANCELLING" ||
                    data.status === "PARTIALLY_FILLED"
                  }
                  className="px-3 py-1 rounded-lg bg-red-500 hover:bg-red-600 disabled:bg-gray-600 transition-colors"
                >
                  Cancel
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
