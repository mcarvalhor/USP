package main;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

import message.Message;

public class Projetor extends JDialog {

	private String serverAddr;
	private int serverPort;

	private JTextField tela;

	public Projetor(String serverAddr, int serverPort) {
		
		this.serverAddr = serverAddr;
		this.serverPort = serverPort;
		
		setSize(220, 280);

		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		this.setLocation(dim.width / 2 - this.getSize().width / 2, dim.height
				/ 2 - this.getSize().height / 2);
		setTitle("Projetor");
		setResizable(false);
		getContentPane().setLayout(null);
		
		JButton btnSolicitarTela = new JButton("Solicitar tela");
		btnSolicitarTela.setBounds(12, 35, 170, 25);
		getContentPane().add(btnSolicitarTela);
		
		tela = new JTextField();
		tela.setBounds(12, 88, 170, 22);
		getContentPane().add(tela);
		tela.setColumns(10);
		
		JButton btnMudarTela = new JButton("Mudar tela");
		btnMudarTela.setBounds(12, 123, 170, 25);
		getContentPane().add(btnMudarTela);
		
		JButton btnNewButton = new JButton("Voltar");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});
		btnNewButton.setBounds(12, 195, 170, 25);
		getContentPane().add(btnNewButton);
		
		btnSolicitarTela.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				
				try {
					String result = new Message(
							new Message("INTERFACE_CLIENTE", "GET_SCREEN",
									"").send(serverAddr,
									serverPort)).getBody();
					System.out.println("resultado: " + result);
					if (result.isEmpty()) {
						JOptionPane.showMessageDialog(null, "Nao foi possivel verificar a tela atual");
						return;
					}
					JOptionPane.showMessageDialog(null,
							(result));
				} catch (Exception exc) {
					JOptionPane.showMessageDialog(null,
							"Nao foi possivel enviar solicitacao");
					System.err.println(exc);
					exc.printStackTrace();
				}
			}
		});
		
		btnMudarTela.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				
				int n;
				try {
					n = Integer.parseInt(tela.getText());
				} catch (Exception ex) {
					JOptionPane
							.showMessageDialog(null,
									"Digite um valor de tela valido (de 0 a quantidade de telas - 1)");
					tela.setText("");
					return;
				}

				if (n < 0) {
					JOptionPane
							.showMessageDialog(null,
									"Digite um valor de tela valido (de 0 a quantidade de fileiras - 1)");
					tela.setText("");
					return;
				}
				
				try {
					String result = new Message(
							new Message("INTERFACE_CLIENTE", "SET_SCREEN",
									tela.getText()).send(serverAddr,
									serverPort)).getBody();
					System.out.println("resultado: " + result);
					JOptionPane.showMessageDialog(null,
							(result.equals("1") ? "Tela foi alterada"
									: "Nao foi possivel alterar a tela"));
				} catch (Exception exc) {
					JOptionPane.showMessageDialog(null,
							"Nao foi possivel enviar solicitacao");
					System.err.println(exc);
					exc.printStackTrace();
				}
			}
		});
		setModal(true);
	}
}
