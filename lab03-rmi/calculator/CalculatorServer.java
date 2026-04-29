import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.RemoteException;

public class CalculatorServer {
    public static void main(String[] args) {
        try {
            // Create an instance of the remote object
            CalculatorImpl calculator = new CalculatorImpl();

            // Create and get the RMI registry on port 1099
            Registry registry = LocateRegistry.createRegistry(2099);

            // Bind the remote object to the registry with a name
            registry.rebind("CalculatorService", calculator);

            System.out.println("Calculator Server is ready.");
        } catch (RemoteException e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}