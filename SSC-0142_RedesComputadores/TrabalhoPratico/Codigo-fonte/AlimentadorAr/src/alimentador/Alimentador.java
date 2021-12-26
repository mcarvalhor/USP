package alimentador;


import java.io.*;
import java.net.*;

public class Alimentador {

	@SuppressWarnings("unused")
	private String serverAddress;
	@SuppressWarnings("unused")
	private int serverPort, port;
	private ServerSocket socket;
	private InetAddress addr;

	private boolean ligado; // O ar-condicionado ta ligado?
	private double temperatura; // Temperatura (graus C)

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
		@SuppressWarnings("unused")
		Socket socket;
		Message m;

		this.serverAddress = serverAddress;
		this.serverPort = serverPort;
		this.port = port;

		this.ligado = false;
		this.temperatura = 20;

		/* Tentar conectar ao gerenciador, mandando minha porta de servidor */
		message = new Message("AR_CONDICIONADO", "CONNECT",
				Integer.toString(port)).send(serverAddress, serverPort);
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
				saida = new Message("AR_CONDICIONADO", "", "");
				switch (entrada.getAction()) {
				case "ON":
					this.ligado = true;
					this.temperatura = 20;
					saida.setAction("ON");
					saida.setBody("1");
					break;
				case "OFF":
					this.ligado = false;
					saida.setAction("OFF");
					saida.setBody("1");
					break;
				case "GET_TEMP":
					saida.setAction("GET_TEMP");
					if(this.ligado) {
						saida.setBody("Temperatura: " + Double.toString(this.temperatura) + " graus");
					} else {
						saida.setBody("Temperatura: (ligue o ar-condicionado primeiro)");
					}
					break;
				case "SET_TEMP":
					saida.setAction("SET_TEMP");
					if (this.ligado) {
						try {
							this.temperatura = Double.parseDouble(entrada
									.getBody());
							saida.setBody("1");
						} catch (NumberFormatException ex) {
							// Erro: O cara me manda mudar a temperatura mas nao
							// manda um double valido de temperatura!
							saida.setBody("0");
						}
					} else {
						saida.setBody("0"); // Erro: nao to ligado, entao nao
											// tem como alterar a temperatura!
					}
					break;
				}
				outputStream = new BufferedWriter(new OutputStreamWriter(
						s.getOutputStream(), "UTF-8"));
				outputStream.write(saida.toString());
				outputStream.flush();
				s.close();
			} catch (Exception ex) {
				System.err
						.println("Alguem tentou conectar aqui no servidor, mas nao deu certo. :(");
				System.err.println(ex);
				ex.printStackTrace();
			}
		}
	}

}
