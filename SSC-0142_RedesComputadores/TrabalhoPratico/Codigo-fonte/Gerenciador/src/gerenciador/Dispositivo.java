package gerenciador;

public class Dispositivo {

	private String hostAddress;
	private int hostPort;

	public Dispositivo(String address, int port) {
		this.hostAddress = address;
		this.hostPort = port;
	}

	public String getAddress() {
		return this.hostAddress;
	}

	public int getPort() {
		return this.hostPort;
	}

	public void setAddress(String address) {
		this.hostAddress = address;
	}

	public void setPort(int port) {
		this.hostPort = port;
	}

}
