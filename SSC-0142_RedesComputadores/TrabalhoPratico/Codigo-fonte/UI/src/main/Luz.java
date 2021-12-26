package main;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.NumberFormat;

import javax.swing.*;
import javax.swing.text.NumberFormatter;

import message.Message;

public class Luz extends JDialog {

	private String serverAddr;
	private int serverPort;

	private JTextField fileira;

	public Luz(String serverAddr, int serverPort) {
		
		this.serverAddr = serverAddr;
		this.serverPort = serverPort;

		setResizable(false);
		setSize(325, 300);

		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		this.setLocation(dim.width / 2 - this.getSize().width / 2, dim.height
				/ 2 - this.getSize().height / 2);
		setTitle("Luzes");
		getContentPane().setLayout(null);

		JRadioButton rdbtnAcender = new JRadioButton("Acender");
		rdbtnAcender.setBounds(105, 88, 127, 25);
		rdbtnAcender.setSelected(true);
		getContentPane().add(rdbtnAcender);

		JRadioButton rdbtnApagar = new JRadioButton("Apagar");
		rdbtnApagar.setBounds(105, 118, 127, 25);
		getContentPane().add(rdbtnApagar);

		ButtonGroup group = new ButtonGroup();
		group.add(rdbtnAcender);
		group.add(rdbtnApagar);

		JButton btnConfirmar = new JButton("Enviar");
		btnConfirmar.setBounds(105, 152, 97, 25);
		getContentPane().add(btnConfirmar);

		fileira = new JTextField();
		fileira.setBounds(105, 64, 116, 22);
		getContentPane().add(fileira);
		fileira.setColumns(10);

		JLabel lblNumeroDaFileira = new JLabel("Num. da fileira");
		lblNumeroDaFileira.setBounds(105, 40, 116, 16);
		getContentPane().add(lblNumeroDaFileira);

		JButton btnVoltar = new JButton("Voltar");
		btnVoltar.setBounds(105, 190, 97, 25);
		getContentPane().add(btnVoltar);

		btnVoltar.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});

		btnConfirmar.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {

				int n;

				try {
					n = Integer.parseInt(fileira.getText());
				} catch (Exception ex) {
					JOptionPane
							.showMessageDialog(null,
									"Digite um valor de fileira valido (de 0 a quantidade de fileiras - 1)");
					fileira.setText("");
					return;
				}

				if (n < 0) {
					JOptionPane
							.showMessageDialog(null,
									"Digite um valor de fileira valido (de 0 a quantidade de fileiras - 1)");
					fileira.setText("");
					return;
				}

				if (rdbtnAcender.isSelected()) {

					try {
						String result = new Message(
								new Message("INTERFACE_CLIENTE", "LUZON",
										fileira.getText()).send(serverAddr,
										serverPort)).getBody();
						System.out.println("resultado: " + result);
						JOptionPane.showMessageDialog(null,
								(result.equals("1") ? "Fileira foi ligada"
										: "Nao foi possivel ligar a fileira"));
					} catch (Exception exc) {
						JOptionPane.showMessageDialog(null,
								"Nao foi possivel enviar solicitacao");
						System.err.println(exc);
						exc.printStackTrace();
						System.exit(1);
					}

				} else if (rdbtnApagar.isSelected()) {
					try {
						String result = new Message(new Message(
								"INTERFACE_CLIENTE", "LUZOFF", fileira
										.getText())
								.send(serverAddr, serverPort)).getBody();
						JOptionPane.showMessageDialog(
								null,
								(result.equals("1") ? "Fileira foi desligada"
										: "Nao foi possivel desligar a fileira"));
					} catch (Exception exc) {
						JOptionPane.showMessageDialog(null,
								"Nao foi possivel enviar solicitacao");
						System.err.println(exc);
						exc.printStackTrace();
						System.exit(1);
					}
				}

			}
		});

		setModal(true);

	}
}
