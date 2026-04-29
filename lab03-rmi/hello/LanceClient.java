/*
 * SYSR - Lab 03 RMI - Hello example
 * Client: looks up the remote object and calls getInformation().
 *
 * Usage: java hello.LanceClient
 */
package hello;

import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;

public class LanceClient {

    public static void main(String[] args) {
        System.out.println("Starting RMI client...");
        try {
            String url = "rmi://" + InetAddress.getLocalHost().getHostAddress() + "/TestRMI";
            Remote r = Naming.lookup(url);
            if (r instanceof Information info) {
                String s = info.getInformation();
                System.out.println("Response from server: " + s);
            }
        } catch (MalformedURLException | RemoteException |
                 NotBoundException | UnknownHostException e) {
            e.printStackTrace();
        }
        System.out.println("Client done.");
    }
}