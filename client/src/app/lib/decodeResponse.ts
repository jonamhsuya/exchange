export type ExchangeResponse =
  | { kind: "idAssignment"; clientId: number; }
  | { kind: "accept"; orderId: number }
  | { kind: "cancel"; orderId: number }
  | {
      kind: "trade";
      orderId: number;
      quantity: number;
      price: number;
    };

export function decodeResponse(buffer: ArrayBuffer): ExchangeResponse | null {
  const view = new DataView(buffer);
  const type = view.getUint8(buffer.byteLength - 2);

  let checksum = 0;
  for (let i = 0; i < buffer.byteLength - 1; i++) {
    checksum ^= view.getUint8(i);
  }
  const expected = view.getUint8(buffer.byteLength - 1);
  if (checksum !== expected) {
    console.error("Checksum mismatch in response");
    return null;
  }

  if (type === 0) {
    return {
      kind: "idAssignment",
      clientId: view.getUint32(0, true),
    };
  }

  if (type === 1) {
    return {
      kind: "accept",
      orderId: view.getUint32(0, true),
    };
  }

  if (type === 2) {
    return {
      kind: "cancel",
      orderId: view.getUint32(0, true),
    };
  }

  if (type === 3) {
    return {
      kind: "trade",
      orderId: view.getUint32(0, true),
      quantity: view.getUint32(4, true),
      price: view.getUint32(8, true) / 100,
    };
  }

  return null;
}
