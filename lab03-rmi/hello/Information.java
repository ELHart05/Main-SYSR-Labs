/*
 * SYSR - Lab 03 RMI - Hello example
 * Remote interface: declares methods callable from a remote client.
 */
package hello;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Information extends Remote {
    String getInformation() throws RemoteException;
}
