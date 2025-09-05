import net from "net";
import { NextResponse } from "next/server";

function buildOrderMessage(
  side: number,
  quantity: number,
  price: number
): Buffer {
  const buffer = Buffer.alloc(16);

  // Header
  buffer.writeUInt16LE(16, 0);
  buffer.writeUInt8(side, 2);

  // Payload
  buffer.writeUInt32LE(quantity, 4);
  buffer.writeUInt32LE(Math.floor(price * 100), 8);

  // Checksum
  let checksum = 0;
  for (let i = 0; i < 15; i++) {
    checksum ^= buffer[i];
  }
  buffer.writeUInt8(checksum, 15);

  return buffer;
}

export async function POST(req: Request) {
  try {
    const { side, quantity, price } = await req.json();

    const orderMsg = buildOrderMessage(side, quantity, price);

    await new Promise<void>((resolve, reject) => {
      const client = new net.Socket();
      client.connect(9999, "18.188.92.59", () => {
        client.write(orderMsg);
        client.end();
        resolve();
      });
      client.on("error", reject);
    });

    return NextResponse.json({ success: true });
  } catch (err: any) {
    console.error("Error sending order:", err);
    return NextResponse.json(
      { success: false, error: err.message },
      { status: 500 }
    );
  }
}
