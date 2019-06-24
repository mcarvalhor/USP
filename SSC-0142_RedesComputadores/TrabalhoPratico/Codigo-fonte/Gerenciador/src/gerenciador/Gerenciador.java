package gerenciador;


import java.io.IOException;
import java.net.*;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.concurrent.Semaphore;


public class Gerenciador {

	private ServerSocket socket;
	private InetAddress addr;
	private int port;

	public Dispositivo
		alimentadorLuzes,
		alimentadorAr,
		alimentadorProjetor;
	public Semaphore mutex;

	public Uspiano professor;
	public ArrayList<Uspiano> listaPresenca;
	public Timestamp ultimaPresenca;

	public Gerenciador() throws IOException {
		this(9000);
	}

	public Gerenciador(int port) throws IOException {
		this.port = port;
		this.socket = new ServerSocket(this.port);
		this.addr = Inet4Address.getLocalHost();
		if(this.addr == null) {
			this.addr = Inet6Address.getLocalHost();
		}
		if(this.addr == null) {
			this.addr = InetAddress.getLocalHost();
		}
		if(this.addr == null) {
			throw new UnknownHostException();
		}
		this.mutex = new Semaphore(1);

		this.alimentadorLuzes = this.alimentadorAr = this.alimentadorProjetor = null;
		this.ultimaPresenca = null;
		this.listaPresenca = null;
		this.professor = null;
	}

	public String getHostAddress() {
		return addr.getHostAddress();
	}

	public int getPort() {
		return this.port;
	}

	public void execute() {
		GerenciadorThread t;
		Socket s;

		new TimerThread(this).start();

		while (true) {
			try {
				s = socket.accept();
				if(s != null) {
					t = new GerenciadorThread(s, this);
					System.out.println("Requisicao: " + t.getHostAddress() + ":" + t.getPort());
					t.start();
				}
			} catch(IOException ex) {
				System.err.println("Alguem tentou conectar aqui no servidor, mas nao deu certo. :(");
				System.err.println(ex);
				ex.printStackTrace();
			}
		}
	}

}
