# Distributed Systems - ESI 4th year Labs

Practical labs for the **Distributed Systems** (Systèmes Répartis) module at ESI, 4th-year Computer Science.  
All labs build toward a full distributed application, progressing from raw socket I/O up to a token-based mutual exclusion simulation.

---

## Labs at a Glance

| Lab | Topic | Language | Key Concepts |
|-----|-------|----------|--------------|
| [lab01-sockets](lab01-sockets/) | TCP Socket Programming | C | BSD sockets, `pthreads`, tac-o-tac chat |
| [lab02-rpc](lab02-rpc/) | Remote Procedure Call (ONC RPC) | C | `rpcgen`, IDL, XDR serialisation |
| [lab03-rmi](lab03-rmi/) | Java RMI | Java | Remote interfaces, registry, stubs |
| [lab04-ricart-agrawala](lab04-ricart-agrawala/) | Distributed Mutual Exclusion | Java | RA83 algorithm, logical clocks, Swing GUI |

---

## Prerequisites

| Lab | Requirements |
|-----|-------------|
| lab01 | `gcc`, `make`, `glibc` (pthreads) |
| lab02 | `gcc`, `make`, `libnsl-dev`, `rpcbind` |
| lab03 | Java 11+, `javac` / `java` |
| lab04 | Java 11+, Swing (included in standard JDK) |

### Quick install (Debian / Ubuntu)

```bash
sudo apt update
sudo apt install gcc make libnsl-dev rpcbind default-jdk
```

---

## Getting Started

Clone the repository and navigate to any lab:

```bash
git clone <repo-url>
cd repo

# example: build and run lab01 basic chat
cd lab01-sockets/basic
make
# Terminal 1: ./server
# Terminal 2: ./client
```

Each lab has its own `README.md` with detailed build instructions, usage examples, and concept explanations.

---

## Repository Structure

```
Main-SYSR-Labs/
├── README.md                        ← you are here
├── .gitignore
├── lab01-sockets/
│   ├── README.md
│   ├── basic/          ← interactive tac-o-tac (C, no threads)
│   └── threaded/       ← concurrent chat with pthreads, file-driven
├── lab02-rpc/
│   ├── README.md
│   └── calcul.*        ← ONC RPC calculator (IDL + generated stubs + impl)
├── lab03-rmi/
│   ├── README.md
│   ├── hello/          ← minimal RMI "Hello" example
│   └── calculator/     ← interactive RMI calculator
└── lab04-ricart-agrawala/
    ├── README.md
    └── src/            ← 10-process RA83 simulation with Swing dashboard
```

---

## Lab Summaries

### Lab 01 - Sockets

Implements a simple TCP chat between a client and a server.  
The **basic** variant is turn-by-turn (one message each, alternating).  
The **threaded** variant uses `pthreads` to decouple send and receive and drives the conversation from text files - useful for automated testing.

### Lab 02 - RPC

Implements a remote calculator using **ONC/Sun RPC**.  
The interface is defined in the IDL file `calcul.x`; `rpcgen` generates all stub code and XDR serialisers.  
The student only writes the service implementation (`calcul_server.c`) and the client test (`calcul_client.c`).

### Lab 03 - RMI

Two Java RMI examples:
- **hello/** - a minimal remote object that returns a greeting string.
- **calculator/** - a full interactive calculator with add, subtract, multiply, divide.

Demonstrates the complete RMI lifecycle: define a `Remote` interface, implement it with `UnicastRemoteObject`, register in the RMI registry, look up from a client.

### Lab 04 - Ricart & Agrawala 83

Simulates the **RA83 token-based distributed mutual exclusion** algorithm across 10 concurrent processes communicating over TCP sockets.  
A **Swing GUI** shows each process's state (idle / requesting / in critical section), who holds the token, and the pending request queues in real time.  
Fault injection (kill/resume) can be triggered interactively through the GUI.
