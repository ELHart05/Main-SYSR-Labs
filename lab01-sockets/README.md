# Lab 01 - Socket Programming (TCP Chat)

**Module:** Systèmes Répartis - ESI 2CS  
**Language:** C  
**Topics:** BSD Sockets, TCP, `pthreads`

---

## Overview

Two sub-projects implement a basic **tac-o-tac** (turn-by-turn) chat over TCP sockets:

| Sub-project | Description |
|-------------|-------------|
| `basic/`    | Minimal client–server: one message each, no threads |
| `threaded/` | Concurrent client–server using `pthreads`; messages read from text files |

---

## Directory Structure

```
lab01-sockets/
├── basic/
│   ├── client.c          ← TCP client (interactive)
│   ├── server.c          ← TCP server (interactive)
│   └── Makefile
└── threaded/
    ├── client.c          ← pthread client (reads from file)
    ├── server.c          ← pthread server (reads from file)
    ├── client-messages.txt
    ├── server-messages.txt
    └── Makefile
```

---

## Building

```bash
# Basic variant
cd basic && make

# Threaded variant
cd threaded && make
```

---

## Running

### Basic (terminal 1 → server, terminal 2 → client)

```bash
# Terminal 1
./server

# Terminal 2
./client
```

Type a message and press Enter. The other side must reply before you can send again.  
Type `EXIT` to close the session.

### Threaded (file-driven)

```bash
# Terminal 1
./server server-messages.txt

# Terminal 2
./client 127.0.0.1 client-messages.txt
```

Each side reads its messages from the supplied file, sending one line every 2 seconds.  
When all messages are sent, `EXIT` is sent automatically.

---

## Key API Calls

| Call | Description |
|------|-------------|
| `socket(AF_INET, SOCK_STREAM, 0)` | Create a TCP socket |
| `bind()` | Assign address/port to server socket |
| `listen()` | Mark socket as passive (server) |
| `accept()` | Block until a client connects |
| `connect()` | Initiate connection to server (client) |
| `send()` / `recv()` | Send / receive data |
| `shutdown()` | Close the connection |
| `pthread_create()` | Spawn send/receive threads |

---

## Notes

- Port `6666` is used by the basic variant; port `6666` is also the default for the threaded variant.
- The server sets `SO_REUSEADDR` to allow immediate restart without "Address already in use" errors.
- In the basic variant, both sides use `scanf(" %s", ...)` which does not support spaces in messages. For multi-word messages, replace with `fgets`.
