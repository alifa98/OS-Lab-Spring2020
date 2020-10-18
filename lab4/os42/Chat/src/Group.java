import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class Group {

	private List<User> users;
	private Long id;

	public Group(Long id) {

		this.id = id;
	}

	public List<User> getUsers() {

		return users;
	}

	public void setUsers(List<User> users) {

		this.users = users;
	}

	public Long getGroupId() {

		return id;
	}

	public void setGroupId(Long groupId) {

		this.id = groupId;
	}

	public void addUser(User user) {

		if (this.users == null)
			users = new ArrayList<>();

		users.add(user);
	}

	public void removeUser(User user) {

		if (this.users != null)
			users.remove(user);
	}

	public void sendToAllSubscribers(User userProfile, String text) {

		System.out.println("Sendig Message from " + userProfile.getName() + " to  Group " + this.id);
		for (User user : users){
			if (userProfile.equals(user))
				continue;

			try{
				DataOutputStream out = new DataOutputStream(new BufferedOutputStream(user.getSocket().getOutputStream()));
				out.writeUTF(userProfile.getName() + " says: " + text + "\n");
				out.flush();
				System.out.println("Message Sent to " + user.name);
			}
			catch (IOException e){
				System.err.println("Error in BroadCast.");
				e.printStackTrace();
			}
		}

	}

	@Override
	public String toString() {

		return "Group [users=" + users + ", id=" + id + "]";
	}

}
