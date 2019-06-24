package sistema;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class Message {

	private String type;
	private int id;
	private int len;
	private String action;
	private String body;

	private static final String MSG_REGEX = "^\\s*SALA_INTEL\\s*[\\r\\n]+\\s*TYPE:\\s*([^\\r\\n]+)\\s*[\\r\\n]+\\s*"
			+ "ID:\\s*([^\\r\\n]+)\\s*[\\r\\n]+\\s*LEN:\\s*([^\\r\\n]+)\\s*[\\r\\n]+\\s*ACTION:\\s*([^\\r\\n]+)\\s*([\\r\\n]+\\s*.*)?$";
	private static final Pattern MSG_PATTERN = Pattern.compile(MSG_REGEX,
			Pattern.DOTALL | Pattern.CASE_INSENSITIVE);

	/* Construtores */

	/**
	 * Cria uma mensagem a partir de um TYPE, ID, LEN, ACTION e BODY.
	 *
	 * @param type
	 * @param id
	 * @param len
	 * @param action
	 * @param body
	 */
	public Message(String type, int id, int len, String action, String body) {
		this.type = type;
		this.id = id;
		this.len = len;
		this.action = action;
		this.body = body;
	}

	/**
	 * Cria uma mensagem a partir de um TYPE, ID, ACTION e BODY. O LEN eh
	 * calculado a partir de BODY.
	 *
	 * @param type
	 * @param id
	 * @param action
	 * @param body
	 */
	public Message(String type, int id, String action, String body) {
		this(type, id, body.length(), action, body);
	}

	/**
	 * Cria uma mensagem a partir de um TYPE, ACTION e BODY. O LEN eh calculado
	 * a partir de BODY e o ID eh definido aleatoriamente.
	 *
	 * @param type
	 * @param action
	 * @param body
	 */
	public Message(String type, String action, String body) {
		this(type, (int) (Math.random() * Integer.MAX_VALUE), action, body);
	}

	/**
	 * Cria uma mensagem a partir de uma string formatada da maneira de nosso
	 * relatorio.
	 *
	 * @param fullMessage
	 *            a string
	 */
	public Message(String fullMessage) {
		Matcher m;

		m = MSG_PATTERN.matcher(fullMessage);
		if (!m.matches()) {
			throw new IllegalArgumentException("Invalid message.");
		}

		this.type = m.group(1);
		this.id = Integer.parseInt(m.group(2));
		this.len = Integer.parseInt(m.group(3));
		this.action = m.group(4);
		this.body = m.group(5);

		if (this.body == null || this.body.trim().isEmpty()) {
			this.body = "";
		}
		this.body = this.body.trim();
	}

	/* Methods */

	/**
	 * Verifica se uma string pode ser usada para instanciar uma nova mensagem.
	 * (eh valida de acordo com o nosso relatorio?)
	 *
	 * @param fullMessage
	 *            a string que sera validada
	 * @return
	 */
	public static boolean valid(String fullMessage) {
		Matcher m;

		m = MSG_PATTERN.matcher(fullMessage);
		return m.matches();
	}

	/**
	 * Cria uma nova Mensagem a partir da resposta de um socket.
	 *
	 * @param receivingSocket
	 *            o socket
	 * @return a nova mensagem
	 * @throws IOException
	 */
	public static Message getFromSocket(Socket receivingSocket)
			throws IOException {
		BufferedReader socketStream;
		String l, input;

		socketStream = new BufferedReader(new InputStreamReader(
				receivingSocket.getInputStream(), "UTF-8"));

		input = "";
		while ((l = socketStream.readLine()) != null) {
			l += System.lineSeparator();
			input += l;
		}

		return new Message(input);
	}

	/**
	 * Envia esta instancia de Mensagem como socket para um servidor e retorna
	 * uma string que eh toda a resposta dele pra essa mensagem. Idealmente,
	 * essa string de resposta tambem eh uma mensagem valida e pode ser
	 * instanciada nessa classe.
	 *
	 * @param hostAddr
	 *            o endereco do servidor
	 * @param hostPort
	 *            a porta do servidor
	 * @return a string da mensagem de resposta do servidor
	 * @throws UnknownHostException
	 * @throws IOException
	 */
	public String send(String hostAddr, int hostPort)
			throws UnknownHostException, IOException {
		Socket sendingSocket;
		BufferedWriter s;
		BufferedReader r;
		String l, input;

		sendingSocket = new Socket(hostAddr, hostPort);

		s = new BufferedWriter(new OutputStreamWriter(
				sendingSocket.getOutputStream(), "UTF-8"));
		s.write(this.toString());
		s.flush();
		sendingSocket.shutdownOutput();

		r = new BufferedReader(new InputStreamReader(
				sendingSocket.getInputStream(), "UTF-8"));
		input = "";
		while ((l = r.readLine()) != null) {
			l += System.lineSeparator();
			input += l;
		}
		sendingSocket.close();

		return input;
	}

	/* Getters */

	public String getType() {
		return this.type;
	}

	public int getId() {
		return this.id;
	}

	public int getLen() {
		return this.len;
	}

	public String getAction() {
		return this.action;
	}

	public String getBody() {
		return this.body;
	}

	/**
	 * Retorna essa instancia de Mensagem da forma como descrito no nosso
	 * relatorio em uma string.
	 *
	 * @return a string
	 */
	@Override
	public String toString() {
		String fullMessage;

		fullMessage = "SALA_INTEL" + System.lineSeparator();
		fullMessage += "TYPE: " + this.type + System.lineSeparator();
		fullMessage += "ID: " + this.id + System.lineSeparator();
		fullMessage += "LEN: " + this.len + System.lineSeparator();
		fullMessage += "ACTION: " + this.action;
		if (this.body != null && !this.body.trim().isEmpty()) {
			fullMessage += System.lineSeparator() + this.body;
		}

		return fullMessage;
	}

	/* Setters */

	public void setType(String newType) {
		this.type = newType;
	}

	public void setId(int newId) {
		this.id = newId;
	}

	public void setLen(int newLen) {
		this.len = newLen;
	}

	public void setAction(String newAction) {
		this.action = newAction;
	}

	public void setBody(String newBody, boolean adjustLen) {
		this.body = newBody;
		if (adjustLen) {
			this.len = newBody.length();
		}
	}

	public void setBody(String newBody) {
		this.setBody(newBody, true);
	}

}
