package chave;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

import console.Console;

public class Chave {

	private String serverAddress;
	private int serverPort;

	/**
	 * Construtor padrao.
	 *
	 * @param serverAddress
	 *            endereco do servidor do gerenciador.
	 * @param serverPort
	 *            porta do servidor do gerenciador.
	 */
	public Chave(String serverAddress, int serverPort)
			throws IOException, IllegalArgumentException {
		String message;
		Message m;

		this.serverAddress = serverAddress;
		this.serverPort = serverPort;

		/* Tentar conectar ao gerenciador, mandando minha porta de servidor */
		message = new Message("CHAVE", "CONNECT", "").send(
				serverAddress, serverPort);
		m = new Message(message);
		if (!m.getBody().equals("1")) {
			throw new IOException("O gerenciador nao permitiu a minha conexao!");
		}

	}

	public void execute() {

		Message saida;

		Console console = Console.getInstance();

		while (true) {

			int on = console
					.readInt("Deseja alterar o estado da chave?\n\t0 - Desligado\n\t1 - Ligado");

			saida = new Message("CHAVE", "", "");

			if (on == 1) {
				saida.setAction("ON");
			} else if (on == 0) {
				saida.setAction("OFF");
			} else {
				System.out.println("Valor incorreto");
				continue;
			}

			try {
				saida.send(this.serverAddress, this.serverPort);
			} catch (IOException ex) {
				System.err
						.println("Nao deu certo. :(");
				System.err.println(ex);
				ex.printStackTrace();
			}

		}
	}

}
