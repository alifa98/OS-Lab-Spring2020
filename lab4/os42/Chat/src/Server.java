
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class Server {

	// initialize socket and input stream
	private ServerSocket server;
	private ExecutorService ex;

	// constructor with port
	public Server(int port) throws IOException {

		server = new ServerSocket(port);
		System.out.println("LOG : Server Created on Port " + port);

		ex = Executors.newCachedThreadPool();

		createAndAddDefaultGroups();
	}

	private void createAndAddDefaultGroups() {

		InMemDataBase.groups.add(new Group(1L));
		InMemDataBase.groups.add(new Group(2L));
		InMemDataBase.groups.add(new Group(3L));
		InMemDataBase.groups.add(new Group(4L));

	}

	public void starter() throws IOException {

		System.out.println("Waiting for first connection.\n");

		while (true){
			ex.submit(new Handeler(server.accept()));
			System.out.println("Waiting for a client...\n");
		}
	}

	//this inner class prepare response for each client in seperate thread
	private class Handeler implements Runnable {

		private Socket socket;
		private User userProfile;

		private DataInputStream in;
		private DataOutputStream out;

		public Handeler(Socket client) {

			this.socket = client;
		}

		public void run() {

			try{
				System.out.println(" >> Client accepted.");
				out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
				in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
				out.writeUTF("Hi, Welcome to Chat Server.\n");
				out.flush();

				String name = in.readUTF();

				this.userProfile = new User(socket, name);
				InMemDataBase.users.add(userProfile);

				while (true){
					String entredMessage = in.readUTF();

					System.out.println(String.format("Received from %s: %s", name, entredMessage));

					if (entredMessage.equals("quit"))
						break;

					String parameters[] = entredMessage.split("\\s");

					System.out.println(String.format("Parsed Command: (%s) %s", name, Arrays.toString(parameters)));

					switch (parameters[0]) {
						case "join":
							if (parameters.length > 1){
								for (Group group : InMemDataBase.groups){
									if (group.getGroupId().equals(Long.parseLong(parameters[1]))){
										group.addUser(this.userProfile);
										try{
											out.writeUTF("You joined to group " + group.getGroupId() + ".\n");
											System.out.println(name + " joined to group " + group.getGroupId() + ".\n");
											out.flush();
										}
										catch (IOException e){
											e.printStackTrace();
										}
									}
								}
								System.err.println(String.format("DataBase State: \nUsers: %s\nGroups: %s\n", InMemDataBase.users, InMemDataBase.groups));
							}
							else{
								System.err.println("Invalid command format");
							}
							break;

						case "send":
							if (parameters.length > 2){
								for (Group group : InMemDataBase.groups){
									if (group.getGroupId().equals(Long.parseLong(parameters[1]))){
										if (group.getUsers().contains(this.userProfile)){

											String text = "";
											for (int index = 2; index < parameters.length; ++index){
												text += parameters[index] + " ";
											}
											group.sendToAllSubscribers(this.userProfile, text);
										}
										else{
											out.writeUTF("You are not in group " + group.getGroupId() + ".\n");
											out.flush();
										}
									}
								}
							}
							else{
								System.err.println("Invalid command format");
							}
							break;
						case "leave":
							if (parameters.length > 1){
								for (Group group : InMemDataBase.groups){
									if (group.getGroupId().equals(Long.parseLong(parameters[1]))){
										group.removeUser(this.userProfile);
										try{
											out.writeUTF("You left the group " + group.getGroupId() + ".\n");
											System.out.println(name + " left group " + group.getGroupId() + ".\n");
											out.flush();
										}
										catch (IOException e){
											e.printStackTrace();
										}
									}
								}
								System.err.println(String.format("DataBase State: \nUsers: %s\nGroups: %s\n", InMemDataBase.users, InMemDataBase.groups));
							}
							else{
								System.err.println("Invalid command format");
							}
							break;
						default:
							System.err.println(String.format("Illegal Command: (%s) %s", name, Arrays.toString(parameters)));
							out.writeUTF("IllegalCommand.\n");
							break;
					}
					out.flush();
				}

				InMemDataBase.users.remove(userProfile);
				in.close();
				out.close();
				socket.close();
				System.out.println("I/O Stremas Closed For Client.");
			}
			catch (Exception e){}
		}

	}

	public static void main(String args[]) {

		try{
			Server s = new Server(Integer.parseInt(args[0]));
			s.starter();
		}
		catch (Exception e){
			System.exit(-1);
		}
	}
}
