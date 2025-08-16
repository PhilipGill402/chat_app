# Chat Application (C, TCP, Pthreads)

This project is a simple **multi-client chat application** written in C. It demonstrates how to use **TCP sockets** and **POSIX threads** to allow multiple clients to communicate with a server in real time.

---

## Features
- Multi-client support via threads on the server.  
- Each client provides a username at startup.  
- Messages are broadcasted to all connected clients in the format:  
  ```
  [username]: [message]
  ```
- Clients can type messages using a prompt (`>>`) while receiving incoming messages simultaneously.  
- Graceful handling of client disconnects and `Ctrl+C` interrupts on the server.  

---

## File Structure
- **server.c** – Implements the server that listens for clients, spawns threads, and relays messages.  
- **client.c** – Implements the client that connects to the server, sends user messages, and displays incoming ones.  
- **client.h / server.h** – Contain required includes and shared constants.  

---

## Requirements
- GCC or Clang (C compiler).  
- POSIX environment (Linux, BSD, macOS, WSL).  
- `pthread` library for threading.  

---

## Build Instructions
Compile both server and client using `gcc`:

```bash
# Build server
make

# Build client
gcc client.c -o client
```

---

## Usage

### 1. Start the Server
```bash
./server
```
By default, the server listens on:
- IP: `127.0.0.1`
- Port: `8080`

### 2. Start Clients
On separate terminals:
```bash
./client
```
Enter a username when prompted. Then you can start chatting.

### 3. Example Chat
Terminal A:
```
Enter your username: Alice
>> Hello!
```

Terminal B:
```
Enter your username: Bob
Alice: Hello!
>> Hey Alice!
```

Terminal A sees:
```
>> Hello!
Bob: Hey Alice!
```

---

## Known Issues
- When server messages arrive at the same time as user input, they may overwrite the prompt (`>>`). This can be refined with libraries like **ncurses** to manage terminal output more cleanly.  
- No authentication or encryption — this is a basic educational example only.  

---

## Future Improvements
- Add ncurses UI for better message/prompt separation.  
- Implement private messages and command handling.  
- Add persistent logging of chats.  
- Add configuration for IP/port at runtime.  
