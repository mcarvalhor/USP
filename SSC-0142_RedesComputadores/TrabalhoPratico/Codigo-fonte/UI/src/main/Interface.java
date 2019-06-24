package main;

import message.Message;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.*;

@SuppressWarnings("serial")
public class Interface extends JFrame implements ActionListener, WindowListener {

	private static final String ADDR_REGEX = "^\\s*([^:]+):([0-9]+)\\s*$";
	private static final Pattern ADDR_PATTERN = Pattern.compile(ADDR_REGEX);

	public String serverAddr;
	public int serverPort;

	JButton btnPresenca;
	JButton btnProjetor;
	JButton btnAr;
	JButton btnLuz;

	public Interface() {
		String s;
		Matcher m;

		if (serverAddr == null) {

			do {
				s = (String) JOptionPane
						.showInputDialog(
								this,
								"Qual o endereco do servidor do gerenciador? (formato = endereco:porta)",
								"Gerenciador", JOptionPane.PLAIN_MESSAGE);
				if (s == null) {
					System.exit(0);
				}
				m = ADDR_PATTERN.matcher(s);
				if (!m.matches()) {
					JOptionPane
							.showMessageDialog(
									this,
									"Endereco invalido! Deve ser no formato endereco:porta. Exemplos: dominio.com:9000 ou 192.168.0.110:8500.",
									"Gerenciador", JOptionPane.WARNING_MESSAGE);
				}
			} while (s == null || !m.matches());

			this.serverAddr = m.group(1);
			this.serverPort = Integer.parseInt(m.group(2));
		}
		try {
			if (!new Message(
					new Message("INTERFACE_CLIENTE", "CONNECT", "").send(
							this.serverAddr, this.serverPort)).getBody()
					.equals("1")) {
				throw new Exception("Not connected");
			}
		} catch (Exception exc) {
			JOptionPane.showMessageDialog(this,
					"Não foi possível conectar ao gerenciador em "
							+ this.serverAddr + ":" + this.serverPort + ".",
					"Gerenciador", JOptionPane.ERROR_MESSAGE);
			System.err.println(exc);
			exc.printStackTrace();
			System.exit(1);
		}

		setSize(430, 300);
		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		this.setLocation(dim.width / 2 - this.getSize().width / 2, dim.height
				/ 2 - this.getSize().height / 2);

		getContentPane().setLayout(null);
		setResizable(false);

		setTitle("Gerenciador");

		btnPresenca = new JButton("<html>Ver lista<br>de presenca</html>");
		btnPresenca.addActionListener(this);

		btnPresenca.setBounds(12, 13, 153, 102);
		getContentPane().add(btnPresenca);

		btnProjetor = new JButton("<html>Gerenciar<br>projetor</html>");
		btnProjetor.addActionListener(this);
		btnProjetor.setBounds(12, 138, 153, 102);
		getContentPane().add(btnProjetor);

		btnAr = new JButton("<html>Gerenciar<br>ar-condicionado</html>");
		btnAr.addActionListener(this);
		btnAr.setBounds(267, 13, 153, 102);
		getContentPane().add(btnAr);
		
		btnLuz = new JButton("<html>Gerenciar<br>luzes</html>");
		btnLuz.addActionListener(this);
		btnLuz.setBounds(267, 138, 153, 102);
		getContentPane().add(btnLuz);

		this.addWindowListener(this);

	}

	@Override
	public void actionPerformed(ActionEvent e) {
		Object src;

		src = e.getSource();
		if (src == btnPresenca) {
			new ListaPresenca(serverAddr, serverPort).setVisible(true);
		} else if (src == btnProjetor) {
			new Projetor(serverAddr, serverPort).setVisible(true);
		} else if (src == btnAr) {
			new Ar(serverAddr, serverPort).setVisible(true);
		} else if (src == btnLuz) {
			new Luz(serverAddr, serverPort).setVisible(true);
		}
	}

	public void windowClosing(WindowEvent e) {
		System.exit(0);
	}

	public void windowOpened(WindowEvent e) {
	}
	public void windowClosed(WindowEvent e) {
	}
	public void windowIconified(WindowEvent e) {
	}
	public void windowDeiconified(WindowEvent e) {
	}
	public void windowActivated(WindowEvent e) {
	}
	public void windowDeactivated(WindowEvent e) {
	}
}
