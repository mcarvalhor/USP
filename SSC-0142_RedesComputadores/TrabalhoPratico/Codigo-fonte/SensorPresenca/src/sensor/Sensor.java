package sensor;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

import console.Console;

public class Sensor {

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
	public Sensor(String serverAddress, int serverPort)
			throws IOException, IllegalArgumentException {
		String message;
		Message m;

		this.serverAddress = serverAddress;
		this.serverPort = serverPort;

		/* Tentar conectar ao gerenciador, mandando minha porta de servidor */
		message = new Message("SENSOR_PRESENCA", "CONNECT", "").send(serverAddress, serverPort);
		m = new Message(message);
		if (!m.getBody().equals("1")) {
			throw new IOException("O gerenciador nao permitiu a minha conexao!");
		}

	}

	public void execute() {

		Message saida;

		Console console = Console.getInstance();

		while (true) {

			boolean detected = console
					.readBoolean("Foi detectada presenca de pessoas?\n\t1 - Sim");
			
			if (!detected) continue;
			
			saida = new Message("SENSOR_PRESENCA", "DETECTED", "");

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