import dgram from "dgram";
import express from "express";

// Server for PD communication
// const UDP_server = dgram.createSocket("udp4");
// UDP_server.bind(41234);

// const moves = [undefined, undefined];
// // gestione dei messaggi che arrivano da PD
// UDP_server.on("message", (msg) => {
//   bufferToMoves(moves, msg);
//   console.log(moves);
// });

// UDP_server.on("listening", () => {
//   const address = UDP_server.address();
//   console.log(`UDP_server listening ${address.address}:${address.port}`);
// });

// UDP_server.on("error", (err) => {
//   console.log(`UDP_server error:\n${err.stack}`);
//   UDP_server.close();
// });

// Server for client communication
const express_server = express();

express_server.listen(3000, (msg) => {
  console.log("express_server running on port 3000");
});

express_server.on("message", (msg) => {
  console.log(msg);
});

express_server.get("/moves", (req, res) => {
  res.json({ move: lastMove });
});

function bufferToMoves(moves, buffer) {
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
