package alimentador;

import java.io.*;
import java.net.*;

public class Alimentador {

	private String serverAddress;
	private int serverPort, port;
	private ServerSocket socket;
	private InetAddress addr;

	private boolean ligado; // O projetor ta ligado?
	private int screen; // tela atual

	/**
	 * Construtor padrao.
	 *
	 * @param serverAddress
	 *            endereco do servidor do gerenciador.
	 * @param serverPort
	 *            porta do servidor do gerenciador.
	 * @param port
	 *            porta local.
	 */
	public Alimentador(String serverAddress, int serverPort, int port)
			throws IOException, IllegalArgumentException {
		String message;
		Socket socket;
		Message m;

		this.serverAddress = serverAddress;
		this.serverPort = serverPort;
		this.port = port;

		this.ligado = false;
		this.screen = 1;

		/* Tentar conectar ao gerenciador, mandando minha porta de servidor */
		message = new Message("PROJETOR", "CONNECT", Integer.toString(port))
				.send(serverAddress, serverPort);
		m = new Message(message);
		if (!m.getBody().equals("1")) {
			throw new IOException("O gerenciador nao permitiu a minha conexao!");
		}

		this.socket = new ServerSocket(this.port);
		this.addr = Inet4Address.getLocalHost();
		if (this.addr == null) {
			this.addr = Inet6Address.getLocalHost();
		}
		if (this.addr == null) {
			this.addr = InetAddress.getLocalHost();
		}
		if (this.addr == null) {
			throw new UnknownHostException();
		}
	}

	public String getHostAddress() {
		return addr.getHostAddress();
	}

	public int getPort() {
		return this.port;
	}

	public void execute() {
		BufferedWriter outputStream;
		Message entrada, saida;
		Socket s;

		while (true) {
			try {
				s = socket.accept();
				System.out.println("Requisicao de: " + s.getInetAddress()
						+ ":" + s.getPort());
				entrada = Message.getFromSocket(s);
				saida = new Message("PROJETOR", "", "");
				switch (entrada.getAction()) {
				case "ON":
					this.ligado = true;
					this.screen = 1;
					saida.setAction("ON");
					saida.setBody("1");
					break;
				case "OFF":
					this.ligado = false;
					saida.setAction("OFF");
					saida.setBody("1");
					break;
				case "GET_SCREEN":
					saida.setAction("GET_SCREEN");
					if(this.ligado) {
						saida.setBody("Tela atual: " + Integer.toString(this.screen));
					} else {
						saida.setBody("Tela atual: (ligue o projetor primeiro)");
					}
					break;
				case "SET_SCREEN":
					saida.setAction("SET_SCREEN");
					if (this.ligado) {
						try {
							this.screen = Integer.parseInt(entrada.getBody());
							saida.setBody("1");
						} catch (NumberFormatException ex) {
							// Erro: O cara me manda mudar a tela mas nao manda
							// um int valido de tela!
							saida.setBody("0");
						}
					} else {
						saida.setBody("0"); // Erro: nao to ligado, entao nao
											// tem como alterar a tela!
					}
					break;
				}
				outputStream = new BufferedWriter(new OutputStreamWriter(
						s.getOutputStream(), "UTF-8"));
				outputStream.write(saida.toString());
				outputStream.flush();
				s.close();
			} catch (IOException ex) {
				System.err
						.println("Alguem tentou conectar aqui no servidor, mas nao deu certo. :(");
				System.err.println(ex);
				ex.printStackTrace();
			}
		}
	}

}
