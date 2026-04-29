# Lab 03 - Remote Method Invocation (Java RMI)

**Module:** Systèmes Répartis - ESI 2CS  
**Language:** Java 11+  
**Topics:** Java RMI, remote interfaces, RMI registry, stubs

---

## Overview

Two self-contained examples demonstrate Java RMI:

| Sub-project | Description |
|-------------|-------------|
| `hello/`      | Minimal "Hello" service - calls `getInformation()` remotely |
| `calculator/` | Interactive calculator - add, subtract, multiply, divide over RMI |

---

## Directory Structure

```
lab03-rmi/
├── Makefile
├── ma_policy.policy          ← security policy file (optional)
├── hello/
│   ├── Information.java      ← remote interface
│   ├── InformationImpl.java  ← server-side implementation
│   ├── LanceServeur.java     ← server launcher (registry on port 1099)
│   └── LanceClient.java      ← client
└── calculator/
    ├── CalculatorInterface.java  ← remote interface
    ├── CalculatorImpl.java       ← server-side implementation
    ├── CalculatorServer.java     ← server launcher (registry on port 2099)
    └── CalculatorClient.java     ← interactive client (menu-driven)
```

---

## Building & Running

### Hello Example

```bash
# compile
make hello-compile

# Terminal 1 - start server
make hello-server

# Terminal 2 - run client
make hello-client
```

### Calculator Example

```bash
# compile
make calc-compile

# Terminal 1 - start server
make calc-server

# Terminal 2 - run client (interactive menu)
make calc-client
```

Or manually:

```bash
javac -d out hello/*.java calculator/*.java
java -cp out hello.LanceServeur     # server
java -cp out hello.LanceClient      # client
java -cp out CalculatorServer       # calc server
java -cp out CalculatorClient       # calc client
```

---

## Architecture

```
Client JVM                              Server JVM
─────────────────                       ──────────────────────────
LanceClient                             LanceServeur
  │                                       │ LocateRegistry.createRegistry(1099)
  │  Naming.lookup("rmi://.../TestRMI")   │ Naming.rebind(url, obj)
  │ ─────────────────────────────────► Registry
  │ ◄──────────── stub ─────────────────  │
  │                                       │
  │  stub.getInformation()                │
  │ ─────────────────────────────────► InformationImpl.getInformation()
  │ ◄──────────── "bonjour" ────────────  │
```

---

## Key Concepts

| Concept | Description |
|---------|-------------|
| `Remote` interface | Marks methods as remotely callable |
| `UnicastRemoteObject` | Base class for remote objects; handles networking |
| `LocateRegistry` | Creates or locates the RMI registry |
| `Naming.rebind()` | Registers a remote object under a URL-style name |
| `Naming.lookup()` | Client retrieves a stub by name |
| Stub | Auto-generated proxy; marshals method calls into network messages |

---

## Notes

- The deprecated `RMISecurityManager` has been removed; the registry is created programmatically inside the server process, so no separate `rmiregistry` process is needed.
- The `hello` package uses port **1099** (RMI default); `calculator` uses port **2099** to avoid conflicts when running both simultaneously.
- Java 17+ removed `rmic` - stub generation is no longer needed; the JVM handles it dynamically via `UnicastRemoteObject`.
