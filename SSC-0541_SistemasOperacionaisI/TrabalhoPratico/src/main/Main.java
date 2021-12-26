package main;

import home.Home;

import javax.swing.SwingUtilities;

public class Main {

	public static void main(String[] args) {
		Runnable swingThread;

		swingThread = new Runnable() {
			@Override
			public void run() {
				new Home();
			}
		};

		SwingUtilities.invokeLater(swingThread);

	}

}
