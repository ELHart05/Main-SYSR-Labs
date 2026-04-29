/*
 * SYSR - Lab 03 RMI - Calculator example
 * Server-side implementation of CalculatorInterface.
 */
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class CalculatorImpl extends UnicastRemoteObject implements CalculatorInterface {

    private static final long serialVersionUID = 1L;

    protected CalculatorImpl() throws RemoteException {
        super();
    }

    @Override
    public double add(double a, double b) throws RemoteException {
        System.out.printf("[Server] add(%.2f, %.2f)%n", a, b);
        return a + b;
    }

    @Override
    public double subtract(double a, double b) throws RemoteException {
        System.out.printf("[Server] subtract(%.2f, %.2f)%n", a, b);
        return a - b;
    }

    @Override
    public double multiply(double a, double b) throws RemoteException {
        System.out.printf("[Server] multiply(%.2f, %.2f)%n", a, b);
        return a * b;
    }

    @Override
    public double divide(double a, double b) throws RemoteException {
        System.out.printf("[Server] divide(%.2f, %.2f)%n", a, b);
        if (b == 0) throw new RemoteException("Division by zero");
        return a / b;
    }
}
