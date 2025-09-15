export function buildCancelRequest(clientId: number, orderId: number): Buffer {
  const MESSAGE_LENGTH = 9;
  const buffer = Buffer.alloc(MESSAGE_LENGTH);

  buffer.writeUInt32LE(clientId, 0);
  buffer.writeUInt32LE(orderId, 4);

  let checksum = 0;
  for (let i = 0; i < MESSAGE_LENGTH; i++) {
    checksum ^= buffer[i];
  }
  buffer.writeUInt8(checksum, MESSAGE_LENGTH - 1);

  return buffer;
}
