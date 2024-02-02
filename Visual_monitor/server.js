import dgram from "dgram";
import cors from "cors";
import express from "express";
import fs from "fs";

// Server for PD communication
const UDP_server = dgram.createSocket("udp4");
UDP_server.bind(41234);

// Initial moves array
const moves = [undefined, undefined];

// variables for CSV file writing
let counterMessages = 0;

// Server for PD communication (PD sends messages to this server)
UDP_server.on("message", (msg) => {
  handleMessages(moves, msg);

  counterMessages++;
  if (moves[0] !== undefined && moves[1] !== undefined) {
    if (counterMessages === 2) {
      let newRecord = {
        timestamp: new Date().toISOString(),
        move1: getMove(moves[0]),
        move2: getMove(moves[1]),
        winner: declareWinner(moves),
      };

      fs.appendFile(
        "games.csv",
        `${newRecord.timestamp},${newRecord.move1},${newRecord.move2},${newRecord.winner}\n`,
        (err) => {
          if (err) throw err;
          console.log("Writing on CSV done");
        }
      );
      counterMessages = 0;
    }
  }
});

UDP_server.on("listening", () => {
  const address = UDP_server.address();
  console.log(`UDP_server running on ${address.address}:${address.port}`);

  if (!fs.existsSync("games.csv"))
    fs.writeFile("games.csv", "timestamp,move1,move2,winner\n", (err) => {
      if (err) throw err;
      console.log("Writing on CSV done");
    });
});

UDP_server.on("error", (err) => {
  console.log(`UDP_server error:\n${err.stack}`);
  UDP_server.close();
});

// Server for client communication (main.js)
const express_server = express();
express_server.use(cors());

express_server.listen(3000, () => {
  console.log("express_server running on port 3000");
});

express_server.get("/last-moves", (req, res) => {
  res.json({ move: moves });
});

/**
 * Function that receive a Buffer object from a UDP message and returns an array of integers that represent the hand movements from p1 and p2 in the given position
 * @param {number[]} moves
 * @param {Buffer} buffer
 * @example handleMessages([undefined, undefined], Buffer.from("p1-0")) // [0, undefined]
 * @example handleMessages([undefined, undefined], Buffer.from("p2-1")) // [undefined, 1]
 * @returns
 */
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

/**
 * Function that given an array of moves, encoded as numbers 0,1,2,3 , return who's the winner or if the set is invalid
 * @param {number[]} moves
 * @returns {"p1" | "p2" | "Invalid move" | "Draw"}
 */
function declareWinner(moves) {
  const p1 = moves[0];
  const p2 = moves[1];
  if (moves.includes(3)) return "Invalid move";
  if (p1 === undefined || p2 === undefined) return "Invalid move";
  if (p1 === p2) return "Draw";
  if (p1 === 0 && p2 === 1) return "p2";
  if (p1 === 0 && p2 === 2) return "p1";
  if (p1 === 1 && p2 === 0) return "p1";
  if (p1 === 1 && p2 === 2) return "p2";
  if (p1 === 2 && p2 === 0) return "p2";
  if (p1 === 2 && p2 === 1) return "p1";
  return "p2";
}

/**
 * Function that given a number from 0 to 3, return the corresponding hand movement
 * @param {number} number
 * @returns {"rock" | "paper" | "scissor" | "invalid"}
 * @example getMove(0) // "rock"
 * @example getMove(1) // "paper"
 * @example getMove(2) // "scissor"
 * @example getMove(3) // "invalid"
 */
function getMove(number) {
  if (number === 0) return "rock";
  if (number === 1) return "paper";
  if (number === 2) return "scissors";
  return "invalid";
}
