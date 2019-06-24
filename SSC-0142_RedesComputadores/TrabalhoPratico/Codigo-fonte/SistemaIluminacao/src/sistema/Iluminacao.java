package sistema;

import java.io.*;
import java.net.*;

public class Iluminacao {

	@SuppressWarnings("unused")
	private String serverAddress;
	@SuppressWarnings("unused")
	private int serverPort, port;
	private ServerSocket socket;
	private InetAddress addr;

	private boolean ligado[]; // fileiras ligadas

	/**
	 * Construtor padrao.
	 *
	 * @param serverAddress
	 *            endereco do servidor do gerenciador.
	 * @param serverPort
	 *            porta do servidor do gerenciador.
	 * @param port
	 *            porta local.
	 * @param fileiras
	 *            do sistema de iluminacao
	 */
	public Iluminacao(String serverAddress, int serverPort, int port,
			int fileiras) throws IOException, IllegalArgumentException {
		String message;
		@SuppressWarnings("unused")
		Socket socket;
		Message m;

		this.serverAddress = serverAddress;
		this.serverPort = serverPort;
		this.port = port;

		this.ligado = new boolean[fileiras];

		/* Tentar conectar ao gerenciador, mandando minha porta de servidor */
		message = new Message("SISTEMA_ILUMINACAO", "CONNECT",
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
				saida = new Message("SISTEMA_ILUMINACAO", "", "");
				switch (entrada.getAction()) {
				case "ON":
					try {
						int i = Integer.parseInt(entrada.getBody());
						if (i >= 0 && i < this.ligado.length) {
							this.ligado[i] = true;
							saida.setBody("1");
						} else {
							saida.setBody("0");
						}
					} catch (NumberFormatException ex) {
						// Erro: O cara me manda acender uma fileira invalida!
						saida.setBody("0");
					}
					saida.setAction("ON");
					break;
				case "OFF":
					try {
						int i = Integer.parseInt(entrada.getBody());
						if (i >= 0 && i < this.ligado.length) {
							this.ligado[i] = false;
							saida.setBody("1");
						} else {
							saida.setBody("0");
						}
					} catch (NumberFormatException ex) {
						// Erro: O cara me manda apagar uma fileira invalida!
						saida.setBody("0");
					}
					saida.setAction("ON");
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
