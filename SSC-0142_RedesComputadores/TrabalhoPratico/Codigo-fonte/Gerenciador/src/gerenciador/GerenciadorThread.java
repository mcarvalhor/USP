package gerenciador;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class GerenciadorThread extends Thread {

	protected Gerenciador main;
	protected InetAddress remoteAddr;
	protected Socket socket;
	int remotePort;

	public GerenciadorThread(Socket socket, Gerenciador main) {
		this.socket = socket;
		this.remoteAddr = socket.getInetAddress();
		this.remotePort = socket.getPort();
		this.main = main;
	}

	public String getHostAddress() {
		return remoteAddr.getHostAddress();
	}

	public int getPort() {
		return this.remotePort;
	}

	@Override
	public void run() {
		BufferedWriter outputStream;
		Message entrada, saida;
		Dispositivo newD;
		Uspiano u;
		Matcher m;
		String s;
		int i;

		try {
			this.main.mutex.acquire();
		} catch(InterruptedException ex) {
			System.err.println("A requisicao nao pode ser processada. Algum erro ocorreu com a thread. :(");
			System.err.println(ex);
			ex.printStackTrace();
			return;
		}

		saida = new Message("GERENCIADOR", "", "");
		try {
			entrada = Message.getFromSocket(this.socket);
			saida.setAction(entrada.getAction());
			saida.setId(entrada.getId());

			switch (entrada.getAction().toUpperCase()) {
				case "CONNECT":
					switch(entrada.getType().toUpperCase()) {
						case "SISTEMA_ILUMINACAO":
							newD = new Dispositivo(this.getHostAddress(),Integer.parseInt(entrada.getBody()));
							if(main.alimentadorLuzes == null) {
								main.alimentadorLuzes = newD;
								saida.setBody("1");
							} else { // Opa! JÃ¡ tem um sistema de luzes conectado. Precisamos ver se ele nÃ£o caiu e Ã© ele mesmo reconectando...
								try {
									new Message("GERENCIADOR", "", "").send(main.alimentadorLuzes.getAddress(), main.alimentadorLuzes.getPort());
									saida.setBody("0"); // Ele nÃ£o caiu! Temos um impostor aqui!
								} catch(IOException exc) {
									main.alimentadorLuzes = newD;
									saida.setBody("1"); // Ele caiu! Reconectado.
								}
							}
							break;
						case "AR_CONDICIONADO":
							newD = new Dispositivo(this.getHostAddress(),Integer.parseInt(entrada.getBody()));
							if(main.alimentadorAr == null) {
								main.alimentadorAr = newD;
								saida.setBody("1");
							} else { // Opa! Ja tem um alimentador de ar condicionado conectado. Precisamos ver se ele nÃ£o caiu e Ã© ele mesmo reconectando...
								try {
									new Message("GERENCIADOR", "", "").send(main.alimentadorAr.getAddress(), main.alimentadorAr.getPort());
									saida.setBody("0"); // Ele nÃ£o caiu! Temos um impostor aqui!
								} catch(IOException exc) {
									main.alimentadorAr = newD;
									saida.setBody("1"); // Ele caiu! Reconectado.
								}
							}
							break;
						case "PROJETOR":
							newD = new Dispositivo(this.getHostAddress(),Integer.parseInt(entrada.getBody()));
							if(main.alimentadorProjetor == null) {
								main.alimentadorProjetor = newD;
								saida.setBody("1");
							} else { // Opa! Ja tem um alimentador do projetor conectado. Precisamos ver se ele nao caiu e eh ele mesmo reconectando...
								try {
									new Message("GERENCIADOR", "", "").send(main.alimentadorProjetor.getAddress(), main.alimentadorProjetor.getPort());
									saida.setBody("0"); // Ele nao caiu! Temos um impostor aqui!
								} catch(IOException exc) {
									main.alimentadorProjetor = newD;
									saida.setBody("1"); // Ele caiu! Reconectado.
								}
							}
							break;
						case "SENSOR_PRESENCA":
							saida.setBody("1");
							break;
						case "LEITOR_CARTAO":
							saida.setBody("1");
							break;
						case "CHAVE":
							saida.setBody("1");
							break;
						case "INTERFACE_CLIENTE":
							saida.setBody("1");
							break;
						default:
							saida.setBody("0");
					}
					break;
				case "DETECTED": // Sensor de presenca
					saida = null; // nao tem resposta pro sensor de presenca.
					this.main.ultimaPresenca = new Timestamp(new Date().getTime());
					try {
						if(!new Message(new Message("GERENCIADOR", "ON", "0").send(this.main.alimentadorLuzes.getAddress(), this.main.alimentadorLuzes.getPort())).getBody().equals("1")) {
							throw new Exception("Lights not on.");
						}
					} catch(Exception exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador das luzes. :O");
					}
					try {
						if(!new Message(new Message("GERENCIADOR", "ON", "").send(this.main.alimentadorAr.getAddress(), this.main.alimentadorAr.getPort())).getBody().equals("1")) {
							throw new Exception("Air not on.");
						}
					} catch(Exception exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador do ar-condicionado. :O");
					}
					break;
				case "READ": // Leitor de cartao
					m = Pattern.compile("^([0-9]+)\\s+(.+)$", Pattern.DOTALL).matcher(entrada.getBody());
					this.main.ultimaPresenca = new Timestamp(new Date().getTime());
					if(!m.matches()) {
						saida.setBody("0");
						break;
					}
					u = new Uspiano(m.group(1), m.group(2));
					if(u.getNumeroUSP() >= 1 && u.getNumeroUSP() <= 10) { // Ã professor.
						this.main.professor = u;
						this.main.listaPresenca = new ArrayList<>();
						break;
					}
					// eh aluno.
					if(this.main.professor == null) {
						System.out.println("Nao eh possivel registrar presenca pro aluno, porque nenhum outro professor registrou presenca antes! (numero usp de professor = 1...10)");
						saida.setBody("0");
						break;
					}
					this.main.listaPresenca.add(u);
					break;
				case "ON": // Chave projetor
					saida = null; // nao tem resposta pra chave do projetor.
					try {
						if(main.alimentadorProjetor != null) {
							new Message("GERENCIADOR", "ON", "").send(main.alimentadorProjetor.getAddress(), main.alimentadorProjetor.getPort());
						}
					} catch(IOException exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador do projetor. :O");
						System.err.println(exc);
						exc.printStackTrace();
					}
					try {
						if(!new Message(new Message("GERENCIADOR", "OFF", "0").send(this.main.alimentadorLuzes.getAddress(), this.main.alimentadorLuzes.getPort())).getBody().equals("1")) {
							throw new Exception("Lights not off.");
						}
					} catch(Exception exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador das luzes. :O");
					}
					break;
				case "OFF": // Chave projetor
					saida = null; // nao tem resposta pra chave do projetor.
					try {
						if(main.alimentadorProjetor != null) {
							new Message("GERENCIADOR", "OFF", "").send(main.alimentadorProjetor.getAddress(), main.alimentadorProjetor.getPort());
						}
					} catch(IOException exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador do projetor. :O");
						System.err.println(exc);
						exc.printStackTrace();
					}
					try {
						if(!new Message(new Message("GERENCIADOR", "ON", "0").send(this.main.alimentadorLuzes.getAddress(), this.main.alimentadorLuzes.getPort())).getBody().equals("1")) {
							throw new Exception("Lights not on.");
						}
					} catch(Exception exc) {
						System.err.println("Falha ao estabelecer comunicacao com o alimentador das luzes. :O");
					}
					break;
				case "LISTA":
					s = "";
					if(this.main.professor == null) {
						System.out.println("Nao tem ninguem na lista de presenca porque nenhum outro professor registrou presenca antes! (numero usp de professor = 1...10)");
					} else {
						Collections.sort(this.main.listaPresenca); // Ordena a lista de chamada
						for(i = 0; i < this.main.listaPresenca.size(); i++) {
							s += this.main.listaPresenca.get(i).toString() + System.lineSeparator();
						}
					}
					s = s.trim();
					saida.setBody(s);
					break;
				case "SET_TEMP":
					if(this.main.alimentadorAr == null) {
						saida.setBody("Erro: o alimentador do ar-condicionado nao esta conectado ao gerenciador!");
						break;
					}
					try {
						if(new Message(new Message("GERENCIADOR", "SET_TEMP", entrada.getBody()).send(this.main.alimentadorAr.getAddress(), this.main.alimentadorAr.getPort())).getBody().equals("1")) {
							saida.setBody("Temperatura alterada com sucesso.");
						} else {
							saida.setBody("Erro: o alimentador do ar-condicionado nao permitiu voce mudar a temperatura por algum motivo! O ar-condicionado ta ligado?");
						}
					} catch(Exception exc) {
						saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
						break;
					}
					break;
				case "GET_TEMP":
					if(this.main.alimentadorAr == null) {
						saida.setBody("Erro: o alimentador do ar-condicionado nao esta conectado ao gerenciador!");
						break;
					}
					try {
						saida.setBody(new Message(new Message("GERENCIADOR", "GET_TEMP", "").send(this.main.alimentadorAr.getAddress(), this.main.alimentadorAr.getPort())).getBody());
					} catch(Exception exc) {
						saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
						break;
					}
					break;
				case "SET_SCREEN":
					if(this.main.alimentadorProjetor == null) {
						saida.setBody("0");
						break;
					}
					try {
						if(new Message(new Message("GERENCIADOR", "SET_SCREEN", entrada.getBody()).send(this.main.alimentadorProjetor.getAddress(), this.main.alimentadorProjetor.getPort())).getBody().equals("1")) {
							saida.setBody("1");
						} else {
							saida.setBody("0");
						}
					} catch(Exception exc) {
						saida.setBody("0");
						break;
					}
					break;
				case "GET_SCREEN":
					if(this.main.alimentadorProjetor == null) {
						saida.setBody("Erro: o alimentador do projetor nao esta conectado ao gerenciador!");
						break;
					}
					try {
						saida.setBody(new Message(new Message("GERENCIADOR", "GET_SCREEN", "").send(this.main.alimentadorProjetor.getAddress(), this.main.alimentadorProjetor.getPort())).getBody());
					} catch(Exception exc) {
						saida.setBody("Erro: nao foi possivel conectar ao alimentador do projetor.");
						break;
					}
					break;
				case "ARON":
					if(main.alimentadorAr == null) {
						saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
					} else {
						try {
							if(new Message(new Message("GERENCIADOR", "ON", entrada.getBody()).send(main.alimentadorAr.getAddress(), main.alimentadorAr.getPort())).getBody().equals("1")) {
								saida.setBody("Ar-condicionado ligado com sucesso.");
							} else {
								saida.setBody("Nao foi possivel ligar o ar-condicionado.");
							}
						} catch(Exception exc) {
							saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
						}
					}
					break;
				case "AROFF":
					if(main.alimentadorAr == null) {
						saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
					} else {
						try {
							if(new Message(new Message("GERENCIADOR", "OFF", entrada.getBody()).send(main.alimentadorAr.getAddress(), main.alimentadorAr.getPort())).getBody().equals("1")) {
								saida.setBody("Ar-condicionado desligado com sucesso.");
							} else {
								saida.setBody("Nao foi possivel desligar o ar-condicionado.");
							}
						} catch(Exception exc) {
							saida.setBody("Erro: nao foi possivel conectar ao alimentador do ar-condicionado.");
						}
					}
					break;
				case "LUZON":
					if(main.alimentadorLuzes == null) {
						saida.setBody("0");
					} else {
						try {
							saida.setBody(new Message(new Message("GERENCIADOR", "ON", entrada.getBody()).send(main.alimentadorLuzes.getAddress(), main.alimentadorLuzes.getPort())).getBody());
						} catch(Exception exc) {
							saida.setBody("0");
						}
					}
					break;
				case "LUZOFF":
					if(main.alimentadorLuzes == null) {
						saida.setBody("0");
					} else {
						try {
							saida.setBody(new Message(new Message("GERENCIADOR", "OFF", entrada.getBody()).send(main.alimentadorLuzes.getAddress(), main.alimentadorLuzes.getPort())).getBody());
						} catch(Exception exc) {
							saida.setBody("0");
						}
					}
					break;
				default:
					saida = null;
			}
			if(saida != null) {
				outputStream = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "UTF-8"));
				outputStream.write(saida.toString());
				outputStream.flush();
			}
			socket.close();
		} catch (Exception ex) {
			System.err.println("A requisicao nao pode ser processada. :(");
			System.err.println(ex);
			ex.printStackTrace();
		}

		this.main.mutex.release();
	}
	
}
