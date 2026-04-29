import java.io.Serializable;
import java.util.Map;

public class Message implements Serializable {
    public enum Type { REQUEST, TOKEN }

    public Type type;
    public int senderId;
    public int timestamp;
    public Map<Integer, Integer> tokenState; // Only for TOKEN

    public Message(Type type, int senderId, int timestamp, Map<Integer, Integer> tokenState) {
        this.type = type;
        this.senderId = senderId;
        this.timestamp = timestamp;
        this.tokenState = tokenState;
    }
}