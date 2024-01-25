import dgram from "dgram";
import cors from "cors";
import express from "express";

// Server for PD communication
const UDP_server = dgram.createSocket("udp4");
UDP_server.bind(41234);

const moves = [undefined, undefined];
// gestione dei messaggi che arrivano da PD
UDP_server.on("message", (msg) => {
  handleMessages(moves, msg);
  if (moves[0] !== undefined && moves[1] !== undefined) {
    moves[0] = undefined;
    moves[1] = undefined;
    console.log("write on excel file", declareWinner(moves));
  }
});

UDP_server.on("listening", () => {
  const address = UDP_server.address();
  console.log(`UDP_server running on ${address.address}:${address.port}`);
});

UDP_server.on("error", (err) => {
  console.log(`UDP_server error:\n${err.stack}`);
  UDP_server.close();
});

// Server for client communication
const express_server = express();
express_server.use(cors());

express_server.listen(3000, () => {
  console.log("express_server running on port 3000");
});

express_server.get("/last-moves", (req, res) => {
  res.json({ move: moves });
});

function handleMessages(moves, buffer) {
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

function declareWinner(moves) {
  if (moves[0] === moves[1]) {
    return "Draw";
  } else if (moves[0] === 1 && moves[1] === 3) {
    return "Player 1 wins";
  } else if (moves[0] === 2 && moves[1] === 1) {
    return "Player 1 wins";
  } else if (moves[0] === 3 && moves[1] === 2) {
    return "Player 1 wins";
  } else {
    return "Player 2 wins";
  }
}
