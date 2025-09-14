export function buildRequest(
  clientId: number,
  orderId: number,
  side: number,
  quantity: number,
  price: number
): Buffer {
  const MESSAGE_LENGTH = 18;
  const buffer = Buffer.alloc(MESSAGE_LENGTH);

  buffer.writeUInt32LE(clientId, 0);
  buffer.writeUInt32LE(orderId, 4);
  buffer.writeUInt32LE(quantity, 8);
  buffer.writeUInt32LE(Math.floor(price * 100), 12);
  buffer.writeUInt8(side, 16);

  let checksum = 0;
  for (let i = 0; i < MESSAGE_LENGTH; i++) {
    checksum ^= buffer[i];
  }
  buffer.writeUInt8(checksum, MESSAGE_LENGTH - 1);

  return buffer;
}
