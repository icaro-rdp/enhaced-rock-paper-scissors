import dgram from "dgram";

const UDP_server = dgram.createSocket("udp4");
// Listening on a port (e.g., 41234)
UDP_server.bind(41234);

import express from "express";
const express_server = express();

UDP_server.on("error", (err) => {
  console.log(`UDP_server error:\n${err.stack}`);
  UDP_server.close();
});

const moves = [undefined, undefined];
UDP_server.on("message", (msg) => {
  // qui chiamo handleMovements e poi passo i risultati
  handleMovements(moves, msg);
  console.log(moves);
});

UDP_server.on("listening", () => {
  const address = UDP_server.address();
  console.log(`UDP_server listening ${address.address}:${address.port}`);
});

express_server.listen(3000, () => {
  console.log("express_server running on port 3000");
});

function handleMovements(moves, buffer) {
  const message = buffer.toString();
  const splitted = message.split("-");
  const player = splitted[0];

  if (typeof message !== "string") return [3, 3];
  if (player === "p1") {
    moves[0] = parseInt(splitted[1]);
  } else {
    moves[1] = parseInt(splitted[1]);
  }
  return moves;
}
