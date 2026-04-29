import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class CalculatorClient {
    public static void main(String[] args) {
        try {
            // Get the registry on the server's host (localhost in this case)
            Registry registry = LocateRegistry.getRegistry("localhost", 2099);

            // Look up the remote object by name
            CalculatorInterface calculator = (CalculatorInterface) registry.lookup("CalculatorService");

            Scanner scanner = new Scanner(System.in);

            while (true) {
                System.out.println("\nSimple RMI Calculator");
                System.out.println("1. Add");
                System.out.println("2. Subtract");
                System.out.println("3. Multiply");
                System.out.println("4. Divide");
                System.out.println("5. Exit");
                System.out.print("Choose operation (1-5): ");

                int choice = scanner.nextInt();
                if (choice == 5)
                    break;

                System.out.print("Enter first number: ");
                double a = scanner.nextDouble();
                System.out.print("Enter second number: ");
                double b = scanner.nextDouble();

                double result = 0;
                switch (choice) {
                    case 1:
                        result = calculator.add(a, b);
                        break;
                    case 2:
                        result = calculator.subtract(a, b);
                        break;
                    case 3:
                        result = calculator.multiply(a, b);
                        break;
                    case 4:
                        result = calculator.divide(a, b);
                        break;
                    default:
                        System.out.println("Invalid choice!");
                        continue;
                }

                System.out.println("Result: " + result);
            }

            scanner.close();
            System.out.println("Calculator Client exiting...");
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}