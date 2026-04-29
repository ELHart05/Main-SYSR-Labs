import java.util.*;
public class Main {
    public static void main(String[] args) {
        GUI gui = new GUI();
        int basePort = 5000;
        List<Processus> processusList = new ArrayList<>();

        for (int i = 1; i <= 10; i++) {
            Processus p = new Processus(i, basePort, gui);
            Processus.registerProcessus(i, p);
            gui.registerProcess(i, p);
            processusList.add(p);
        }

        for (Processus p : processusList) {
            List<Integer> others = new ArrayList<>();
            for (int i = 1; i <= 10; i++) {
                if (i != p.id) others.add(i);
            }
            p.start(others);
        }
    }
}