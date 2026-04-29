# Lab 04 - Distributed Mutual Exclusion: Ricart & Agrawala 83

**Module:** Systèmes Répartis - ESI 2CS  
**Language:** Java 11+  
**Topics:** Distributed mutual exclusion, logical clocks, sockets, pthreads, Swing GUI

---

## Overview

Simulation of the **Ricart & Agrawala (1983)** token-based distributed mutual exclusion algorithm with:

- **10 processes** numbered 1–10, each running in its own thread
- **Socket-based communication** - each process is simultaneously a peer (server + client)
- **Logical clock** (Lamport) for ordering requests
- **Fault tolerance** - processes can be killed and resumed via the GUI
- **Swing GUI** - real-time visualisation of each process state, token holder, and request queue
- **Per-process log files** written to `logs/`

---

## Directory Structure

```
lab04-ricart-agrawala/
├── Makefile
├── src/
│   ├── Main.java        ← entry point: creates and wires all 10 processes
│   ├── Processus.java   ← process logic (CS request, token management, socket peer)
│   ├── Message.java     ← serialisable message (REQUEST | TOKEN)
│   └── GUI.java         ← Swing dashboard
└── logs/                ← per-process log files generated at runtime
```

---

## Building & Running

```bash
make run
```

Or manually:

```bash
javac -d out src/Message.java src/GUI.java src/Processus.java src/Main.java
java -cp out Main
```

---

## Algorithm Description

Ricart & Agrawala 83 is a **token-based** mutual exclusion algorithm:

1. **Only the process holding the token** may enter the critical section (CS).
2. A process that wants to enter the CS broadcasts a **REQUEST** message with its logical timestamp to all other processes.
3. The holder of the token, after exiting its CS, checks its request queue. If there are pending requests, it forwards the token to the oldest requester.
4. A process that receives a REQUEST while not holding the token adds the requester to its local FIFO queue; if it holds the token and is not in the CS, it forwards it immediately.

**Safety:** at most one process is in the CS at any time (only one token exists).  
**Liveness:** every request is eventually granted (absent process failures).

---

## Port Assignment

Each process `i` listens on port `5000 + i`:

| Process | Port |
|---------|------|
| P1 | 5001 |
| P2 | 5002 |
| … | … |
| P10 | 5010 |

---

## GUI Controls

| Element | Meaning |
|---------|---------|
| `dans CS` (green) | Process is inside critical section |
| `jeton` (blue) | Process currently holds the token |
| `DEMANDE CS` (orange) | Process is waiting to enter CS |
| `Idle` (grey) | Process is in its non-critical section |
| `File: [...]` | FIFO queue of pending requests at this process |
| **Kill** button | Simulate process crash |
| **Reprendre** button | Revive a killed process |

---

## Log Files

Each run creates `logs/P1.log` … `logs/P10.log` (previous logs are overwritten).  
Each log line has the format:

```
[HH:MM:SS.nnn] P<id>: <event>
```

---

## Key Classes

| Class | Responsibility |
|-------|---------------|
| `Main` | Bootstraps processes, shared GUI, peer registration |
| `Processus` | Encapsulates one distributed process: server socket, CS loop, message handling |
| `Message` | `Serializable` DTO carrying `type`, `senderId`, `timestamp` |
| `GUI` | Swing panel; thread-safe updates via `SwingUtilities.invokeLater()` |
