// GUI.java with integrated terminal output per process and FIFO queue visualization
import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.time.LocalTime;
import java.util.*;

public class GUI {
    private final JLabel[] csLabels = new JLabel[11];
    private final JLabel[] tokenLabels = new JLabel[11];
    private final JLabel[] requestLabels = new JLabel[11];
    private final JLabel[] queueLabels = new JLabel[11];
    private final JButton[] killButtons = new JButton[11];
    private final JButton[] resumeButtons = new JButton[11];
    private final JTextArea[] logAreas = new JTextArea[11];
    private final Map<Integer, Processus> processes = new HashMap<>();
    private final Map<Integer, PrintWriter> logWriters = new HashMap<>();

    public GUI() {
        JFrame frame = new JFrame("RA83 Simulation");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new GridLayout(11, 1));

        for (int i = 1; i <= 10; i++) {
            JPanel panel = new JPanel();
            panel.setLayout(new BorderLayout());

            JPanel topPanel = new JPanel(new FlowLayout());
            JLabel pidLabel = new JLabel("Processus " + i);
            csLabels[i] = new JLabel("pas dans CS");
            tokenLabels[i] = new JLabel("No jeton");
            requestLabels[i] = new JLabel("Idle");
            queueLabels[i] = new JLabel("file: []");

            killButtons[i] = new JButton("Kill");
            resumeButtons[i] = new JButton("Reprendre");
            resumeButtons[i].setEnabled(false);

            int pid = i;
            killButtons[i].addActionListener(e -> {
                processes.get(pid).kill();
                killButtons[pid].setEnabled(false);
                resumeButtons[pid].setEnabled(true);
                setRequesting(pid, false);
                log(pid, "KILLED");
            });

            resumeButtons[i].addActionListener(e -> {
                processes.get(pid).resume();
                killButtons[pid].setEnabled(true);
                resumeButtons[pid].setEnabled(false);
                log(pid, "RESUMED");
            });

            topPanel.add(pidLabel);
            topPanel.add(csLabels[i]);
            topPanel.add(tokenLabels[i]);
            topPanel.add(requestLabels[i]);
            topPanel.add(queueLabels[i]);
            topPanel.add(killButtons[i]);
            topPanel.add(resumeButtons[i]);

            logAreas[i] = new JTextArea(5, 50);
            logAreas[i].setEditable(false);
            JScrollPane scrollPane = new JScrollPane(logAreas[i]);

            panel.add(topPanel, BorderLayout.NORTH);
            panel.add(scrollPane, BorderLayout.CENTER);
            frame.add(panel);

            try {
                new File("logs").mkdirs();
                PrintWriter writer = new PrintWriter(new FileWriter("logs/P" + i + ".log", false));
                logWriters.put(i, writer);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }

        frame.setSize(800, 900);
        frame.setVisible(true);
    }

    public void registerProcess(int id, Processus p) {
        processes.put(id, p);
    }

    public void setCS(int pid, boolean inCS) {
        if (pid >= 1 && pid <= 10) {
            SwingUtilities.invokeLater(() -> {
                csLabels[pid].setText(inCS ? "dans CS" : "pas dans CS");
                csLabels[pid].setForeground(inCS ? Color.GREEN.darker() : Color.BLACK);
                log(pid, inCS ? "ENTERED CS" : "EXITED CS");
                if (!inCS) setRequesting(pid, false);
            });
        }
    }

    public void setToken(int pid, boolean hasToken) {
        if (pid >= 1 && pid <= 10) {
            SwingUtilities.invokeLater(() -> {
                tokenLabels[pid].setText(hasToken ? "jeton" : "No jeton");
                tokenLabels[pid].setForeground(hasToken ? Color.BLUE.darker() : Color.BLACK);
                log(pid, hasToken ? "RECEIVED TOKEN" : "SENT TOKEN");
            });
        }
    }

    public void setRequesting(int pid, boolean isRequesting) {
        if (pid >= 1 && pid <= 10) {
            SwingUtilities.invokeLater(() -> {
                requestLabels[pid].setText(isRequesting ? "DEMANDE CS" : "Idle");
                requestLabels[pid].setForeground(isRequesting ? Color.ORANGE.darker() : Color.GRAY);
                if (isRequesting) log(pid, "REQUESTED CS");
            });
        }
    }

    public void updateQueue(int pid, java.util.List<Integer> queue) {
        if (pid >= 1 && pid <= 10) {
            SwingUtilities.invokeLater(() -> {
                queueLabels[pid].setText("File: " + queue.toString());
            });
        }
    }

    private void log(int pid, String msg) {
        String line = "[" + LocalTime.now() + "] P" + pid + ": " + msg;
        System.out.println(line);

        PrintWriter writer = logWriters.get(pid);
        if (writer != null) {
            writer.println(line);
            writer.flush();
        }

        JTextArea area = logAreas[pid];
        if (area != null) {
            SwingUtilities.invokeLater(() -> {
                area.append(line + "\n");
                area.setCaretPosition(area.getDocument().getLength());
            });
        }
    }
}
