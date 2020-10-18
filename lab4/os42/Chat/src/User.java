import java.net.Socket;


public class User {

	Socket socket;
	String name;

	public User(Socket socket, String name) {

		this.socket = socket;
		this.name = name;
	}

	public Socket getSocket() {

		return socket;
	}

	public void setSocket(Socket socket) {

		this.socket = socket;
	}

	public String getName() {

		return name;
	}

	public void setName(String name) {

		this.name = name;
	}

	@Override
	public int hashCode() {

		final int prime = 31;
		int result = 1;
		result = prime * result + ((name == null) ? 0 : name.hashCode());
		result = prime * result + ((socket == null) ? 0 : socket.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {

		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		User other = (User) obj;
		if (name == null){
			if (other.name != null)
				return false;
		}
		else if (!name.equals(other.name))
			return false;
		if (socket == null){
			if (other.socket != null)
				return false;
		}
		else if (!socket.equals(other.socket))
			return false;
		return true;
	}

	@Override
	public String toString() {

		return "User [socket=" + socket + ", name=" + name + "]";
	}

}
