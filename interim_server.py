import socket
import json
from enum import IntEnum

class MsgType(IntEnum):
    Comms_ReqMessages = 0x4d  # 'M'
    Comms_AllMessages = 0x6d  # 'm'
    Comms_NewMessage = 0x4e   # 'N'

mock_messages = [
    {
        "msg_id": 1,
        "author": "Angry Andrew",
        "contents": "Hey, where are you? I've been waiting here 18 hours 43 minutes and 22 seconds!",
    },
    {
        "msg_id": 2,
        "author": "Patient Patricia",
        "contents": "I'm en route now. I tripped over a Walrus and spilt coffee on his book. Sorry for the delay!",
    },
]

def send_all_messages(conn):
    all_messages = {
        "messages": mock_messages
    }
    json_bytes = json.dumps(all_messages).encode('utf-8')
    response = MsgType.Comms_AllMessages.value.to_bytes(4, byteorder='little') + len(json_bytes).to_bytes(4, byteorder='little') + json_bytes
    print(response)
    conn.sendall(response)

def handle_client(conn, addr):
    print(f"Client connected: {addr}")
    while True:
        try:
            msg_type_bytes = conn.recv(4) # Receive the msg type
            if not msg_type_bytes:
                break
            msg_type = MsgType(int.from_bytes(msg_type_bytes, byteorder='little'))

            length_bytes = conn.recv(4)  # Receive the msg length
            length = int.from_bytes(length_bytes, byteorder='little')

            if msg_type == MsgType.Comms_ReqMessages:
                print("Received ReqMessages request")
                send_all_messages(conn)
            else:
                print(f"Received unimplemented message type: {msg_type}")
        except ValueError as e:
            print(f"Invalid message type received: {e}")
        except Exception as e:
            print(f"Error handling client: {e}")
            break
    print(f"Client disconnected: {addr}")
    conn.close()

def main():
    PORT = 3000
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('', PORT))
    server.listen(1)
    print(f"Server listening on port {PORT}")

    while True:
        conn, addr = server.accept()
        handle_client(conn, addr)

if __name__ == "__main__":
    main()
