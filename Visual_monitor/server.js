import dgram from "dgram";
const server = dgram.createSocket("udp4");

server.on("error", (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on("message", (msg) => {
  const moves = [undefined, undefined];
  //console.log(`server got: ${msg}`);
  // qui chiamo handleMovements e poi passo i risultati
  handleMovements(moves, msg);
  console.log(moves);
});

server.on("listening", () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
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

// Listening on a port (e.g., 41234)
server.bind(41234);
