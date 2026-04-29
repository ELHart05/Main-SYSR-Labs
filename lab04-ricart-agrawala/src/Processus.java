import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class Processus {
    public final int id;
    private final int port;
    private final int numProcesses = 10;
    private final GUI gui;

    private final Queue<Integer> demandeFile = new LinkedList<>();
    private boolean jetonPresent = false;
    private boolean dedans = false;
    private int horloge = 0;
    private volatile boolean isAlive = true;

    private static final Map<Integer, Processus> allProcesses = new ConcurrentHashMap<>();

    public static void registerProcessus(int id, Processus p) {
        allProcesses.put(id, p);
    }

    public static boolean isAlive(int id) {
        Processus p = allProcesses.get(id);
        return p != null && p.isAlive;
    }

    public void kill() {
        isAlive = false;
        System.out.println("P" + id + ": KILLED");

        synchronized (this) {
            if (jetonPresent) {
                while (!demandeFile.isEmpty()) {
                    int next = demandeFile.poll();
                    gui.updateQueue(id, new ArrayList<>(demandeFile));
                    if (isAlive(next)) {
                        sendToken(next);
                        jetonPresent = false;
                        gui.setToken(id, false);
                        break;
                    }
                }
            }
        }
    }


    public void resume() {
        isAlive = true;
        System.out.println("P" + id + ": RESUMED");
    }

    private final ExecutorService executor = Executors.newCachedThreadPool();
    private List<Integer> otherIds = new ArrayList<>();

    public Processus(int id, int basePort, GUI gui) {
        this.id = id;
        this.port = basePort + id;
        this.gui = gui;

        if (id == 1) {
            jetonPresent = true;
        }

        executor.submit(this::startServer);
        executor.submit(this::loopRequestCS);
    }

    public void start(List<Integer> otherIds) {
        this.otherIds = otherIds;
    }

    private void startServer() {
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            while (true) {
                try (Socket socket = serverSocket.accept();
                     ObjectInputStream in = new ObjectInputStream(socket.getInputStream())) {
                    Message msg = (Message) in.readObject();
                    handleMessage(msg);
                } catch (Exception e) {
                    System.err.println("P" + id + ": error receiving message: " + e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("P" + id + ": failed to open server socket.");
        }
    }

    private synchronized void handleMessage(Message msg) {
        if (!isAlive) return;

        horloge = Math.max(horloge, msg.timestamp) + 1;

        if (msg.type == Message.Type.REQUEST) {
            if (!demandeFile.contains(msg.senderId)) {
                demandeFile.offer(msg.senderId);
                gui.updateQueue(id, new ArrayList<>(demandeFile));
            }

            if (jetonPresent && !dedans) {
                int next = demandeFile.poll();
                if (next != id && isAlive(next)) {
                    sendToken(next);
                    jetonPresent = false;
                    gui.setToken(id, false);
                }
            }

        } else if (msg.type == Message.Type.TOKEN) {
            jetonPresent = true;
            gui.setToken(id, true);
        }
    }

    private void sendToken(int destId) {
        sendMessage(destId, new Message(Message.Type.TOKEN, id, horloge, null));
    }

    private void sendMessage(int destId, Message msg) {
        int destPort = 5000 + destId;
        try (Socket socket = new Socket("localhost", destPort);
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream())) {
            out.writeObject(msg);
        } catch (IOException e) {
            System.err.println("P" + id + ": error sending message to P" + destId);
        }
    }

    private void loopRequestCS() {
        Random rand = new Random();

        while (true) {
            try {
                Thread.sleep((1 + rand.nextInt(5)) * 1000);
                dedans = true;
                if (isAlive) {
                    requestCriticalSection();
                }
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void requestCriticalSection() {
        synchronized (this) {
            horloge++;
            if (!demandeFile.contains(id)) {
                demandeFile.offer(id);
                gui.updateQueue(id, new ArrayList<>(demandeFile));
            }
            gui.setRequesting(id, true);
            if (!jetonPresent) {
                for (int otherId : otherIds) {
                    sendMessage(otherId, new Message(Message.Type.REQUEST, id, horloge, null));
                }
                System.out.println("P" + id + ": waiting for token...");
                return;
            }

            enterCriticalSection();
        }
    }

    private void enterCriticalSection() {
        try {
            dedans = true;
            gui.setCS(id, true);
            System.out.println("P" + id + ": ENTER CS");
            Thread.sleep((1 + new Random().nextInt(2)) * 1000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            exitCriticalSection();
        }
    }

    private void exitCriticalSection() {
        synchronized (this) {
            dedans = false;
            gui.setCS(id, false);
            System.out.println("P" + id + ": EXIT CS");

            while (!demandeFile.isEmpty()) {
                int next = demandeFile.poll();
                gui.updateQueue(id, new ArrayList<>(demandeFile));
                if (next != id && isAlive(next)) {
                    sendToken(next);
                    jetonPresent = false;
                    gui.setToken(id, false);
                    break;
                }
            }
        }
    }
}
