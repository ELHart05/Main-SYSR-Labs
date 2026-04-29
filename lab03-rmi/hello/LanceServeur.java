/*
 * SYSR - Lab 03 RMI - Hello example
 * Server launcher: starts the RMI registry and registers the remote object.
 *
 * Usage: java hello.LanceServeur
 */
package hello;

import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;

public class LanceServeur {

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(1099);

            InformationImpl informationImpl = new InformationImpl();

            String url = "rmi://" + InetAddress.getLocalHost().getHostAddress() + "/TestRMI";
            System.out.println("Registering object at: " + url);
            Naming.rebind(url, informationImpl);
            System.out.println("Server ready.");

        } catch (RemoteException | MalformedURLException | UnknownHostException e) {
            e.printStackTrace();
        }
    }
}