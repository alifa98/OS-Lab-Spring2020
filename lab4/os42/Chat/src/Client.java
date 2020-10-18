import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;


public class Client {

	// initialize socket and input output streams
	private Socket socket;
	private BufferedReader input;
	private DataOutputStream out;
	private DataInputStream in;

	public Client(String address, int port, String name) throws Exception {

		socket = new Socket(address, port);
		System.out.println("Connected");
		input = new BufferedReader(new InputStreamReader(System.in));
		out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
		in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));

		out.writeUTF(name);

		while (true){
			if (in.available() > 0)
				System.out.println(">>" + in.readUTF() + "\n");

			if (input.ready()){
				String msg = input.readLine();

				if (msg.equals("quit")){
					out.writeUTF(msg);
					out.flush();
					break;
				}
				out.writeUTF(msg);
				out.flush();
			}
		}

		out.close();
		input.close();
		socket.close();
	}

	public static void main(String args[]) {

		try{
			new Client(args[0], Integer.parseInt(args[1]), args[2]);
		}
		catch (Exception e){
			e.printStackTrace();
			System.err.println("Error in stat up");
		}
	}
}
