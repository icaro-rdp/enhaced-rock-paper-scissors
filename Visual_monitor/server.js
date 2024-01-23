import dgram from "dgram";
const server = dgram.createSocket("udp4");

server.on("error", (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on("message", (msg, rinfo) => {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
});

server.on("listening", () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
  // qui chiamo handleMovements e poi passo i risultati
});

const handleMovements(message:string){
  // qui arriva il messaggio delle mani singole
  // il messaggio che arriva è del tipo : p1-0 / p2-3
  // ritorno un array con le due mosse fatte da p1 e p2
  // [0,3]
}

// Listening on a port (e.g., 41234)
server.bind(41234);
