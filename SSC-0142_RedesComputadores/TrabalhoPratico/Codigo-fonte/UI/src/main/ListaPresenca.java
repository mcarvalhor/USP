package main;

import message.Message;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

public class ListaPresenca extends JDialog {

	private String serverAddr;
	private int serverPort;

	DefaultListModel model;
	JList lst;

	public ListaPresenca(String serverAddr, int serverPort) {
		String s;

		setTitle("Lista de presenca");
		getContentPane().setLayout(new FlowLayout());
		setSize(500, 600);
		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		setLocation(dim.width / 2 - this.getSize().width / 2, dim.height
				/ 2 - this.getSize().height / 2);
		setResizable(false);

		this.serverAddr = serverAddr;
		this.serverPort = serverPort;

		model = new DefaultListModel();
		lst = new JList(model);
		lst.setPreferredSize(new Dimension(400, 530));
		getContentPane().add(lst);

		setModal(true);
	}

	@Override
	public void setVisible(boolean visible) {
		String[] names;
		String s;
		int i;

		if(visible) {
			try {
				s = new Message(new Message("INTERFACE_CLIENTE", "LISTA", "").send(this.serverAddr, this.serverPort)).getBody();
				names = s.trim().split("[\\r\\n]+");
				for(i = 0; i < names.length; i++) {
					model.add(i, names[i]);
				}
			} catch (Exception exc) {
				JOptionPane.showMessageDialog(this, "Nao foi possivel comunicar com o gerenciador!", "Lista", JOptionPane.ERROR_MESSAGE);
				return;
			}
		}

		super.setVisible(visible);
	}

}
