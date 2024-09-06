const net = require("net");

const MsgType = {
  Comms_ReqMessages: 0x4d, // 'M'
  Comms_AllMessages: 0x6d, // 'm'
  Comms_NewMessage: 0x4e, // 'N'
};

const mockMessages = [
  {
    msg_id: 1,
    author: "Angry Andrew",
    contents:
      "Hey, where are you? I've been waiting here 18 hours 43 minutes and 22 seconds!",
  },
  {
    msg_id: 2,
    author: "Patient Patricia",
    contents:
      "I'm en route now. I tripped over a Walrus and spilt coffee on his book. Sorry for the delay!",
  },
];

const server = net.createServer((socket) => {
  console.log("Client connected");

  socket.on("data", (data) => {
    console.log(data);
    const msgType = data.readUInt32LE(0);
    const length = data.readUInt32LE(4);

    if (msgType === MsgType.Comms_ReqMessages) {
      console.log("Received ReqMessages request");
      sendAllMessages(socket);
    } else {
      console.log(`Received unknown message type: ${msgType}`);
    }
  });

  socket.on("end", () => {
    console.log("Client disconnected");
  });
});

function sendAllMessages(socket) {
  const allMessages = {
    messages: mockMessages,
  };

  const jsonData = JSON.stringify(allMessages);
  const jsonBuffer = Buffer.from(jsonData);

  const responseBuffer = Buffer.alloc(8 + jsonBuffer.length);
  responseBuffer.writeUInt32LE(MsgType.Comms_AllMessages, 0);
  responseBuffer.writeUInt32LE(jsonBuffer.length, 4);
  jsonBuffer.copy(responseBuffer, 8);
  console.log(responseBuffer);
  socket.write(responseBuffer);
}

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
