# zena
explore ideas related to messenging applications

----

major components:
1. protocol
2. server
3. client (core)
4. GUIs

### protocol

what kind of back and forth would there need to be between server and client?

- fetch new messages
- send message
  - should support rich text - formatting, emojis
- file transfer (images, video)
- check new messages (lightweight - returns a) are there new msgs b) number of new msgs c) which channels they are in

message types? (basically RPCs)

one-to-one
one-to-many (send files as steaming chunks for example)

command / response 

broadcast
