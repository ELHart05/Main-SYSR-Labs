/*
 * SYSR - Lab 03 RMI - Hello example
 * Server-side implementation of the remote object.
 */
package hello;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class InformationImpl extends UnicastRemoteObject implements Information {

    private static final long serialVersionUID = 2674880711467464646L;

    protected InformationImpl() throws RemoteException {
        super();
    }

    @Override
    public String getInformation() throws RemoteException {
        System.out.println("[Server] getInformation() invoked");
        return "bonjour";
    }
}
