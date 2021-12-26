package game;

import home.Home;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;

import javax.sound.sampled.Clip;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.swing.*;

import resources.Resources;
import resources.Utils;
import score.Score;
import score.ScoreDAO;
import tower.Tower;

/**
 * Classe que representa a janela do jogo em execucao.
 *
 */
@SuppressWarnings("serial")
public class GameScreen extends JFrame implements WindowListener, ActionListener, LineListener {

	public static Game game;

	private Home home;
	private Timer repaintTimer, counter;

	private JPanel
			mainPanel, gamePanel;
	private JButton
			playPauseButton;
	private JLabel
			currentLevel, currentScore, currentTime;
	private JProgressBar progressBar;
	private JButton weapon[];
	private int weaponBlock[];

	Clip backgroundMusic;

	private String name;
	private long timer;


	/**
	 * Construtor padrao, que inicia um novo jogo do zero.
	 * @param homeScreen a janela de inicio
	 */
	public GameScreen(Home homeScreen, String name) {
		this(homeScreen, new Game(), name, 0);
	}

	/**
	 * Construtor que permite carregar um estado de jogo ja do disco.
	 * @param homeScreen tela de inicio
	 * @param gameState jogo que sera carregado
	 */
	public GameScreen(Home homeScreen, Game gameState, String name, long timer) {
		super("CALCULUS: Jogatina");
		GridBagConstraints panelConstraints;
		JPanel gameContainer;

		game = gameState;
		this.timer = timer;
		this.name = name;
		repaintTimer = new Timer(200, this);
		counter = new Timer(1000, this);
		weapon = new JButton[5];
		weaponBlock = new int[] {0, 0, 0, 0, 0};

		home = homeScreen;
		mainPanel = new JPanel();
		gamePanel = new GamePanel();
		gameContainer = new JPanel();
		playPauseButton = new JButton();
		currentLevel = new JLabel("");
		currentScore = new JLabel(String.format("%015d", 0));
		currentTime = new JLabel("00:00:00");
		panelConstraints = new GridBagConstraints();
		progressBar = new JProgressBar();

		playPauseButton.addActionListener(this);
		currentLevel.setVerticalTextPosition(JLabel.CENTER);
		currentLevel.setHorizontalTextPosition(JLabel.CENTER);
		currentLevel.setFont(new Font(null, Font.BOLD, 15));
		currentLevel.setForeground(new Color(0, 156, 222));
		//currentScore.setSize(new Dimension(100, 75));
		currentScore.setVerticalTextPosition(JLabel.CENTER);
		currentScore.setHorizontalTextPosition(JLabel.CENTER);
		currentScore.setFont(new Font(null, Font.BOLD, 15));
		currentScore.setForeground(new Color(0, 156, 222));
		//currentTime.setSize(new Dimension(100, 75));
		currentTime.setVerticalTextPosition(JLabel.CENTER);
		currentTime.setHorizontalTextPosition(JLabel.CENTER);
		currentTime.setFont(new Font(null, Font.BOLD, 15));
		gamePanel.setBackground(Color.WHITE);
		progressBar.setPreferredSize(new Dimension(100, 10));
		mainPanel.setLayout(new GridBagLayout()); // GridBagLayout permite maior flexibilidade em diferentes tamanhos de tela/janela
		gameContainer.setLayout(new GridBagLayout());
		progressBar.setStringPainted(true);
		progressBar.setValue(0);
		progressBar.setMaximum(Tower.maxLife);

		panelConstraints.gridx = 0;
		panelConstraints.gridy = 0;
		panelConstraints.gridwidth = 1;
		panelConstraints.gridheight = 1;
		panelConstraints.weightx = 1.0;
		panelConstraints.weighty = 0.0;
		panelConstraints.fill = GridBagConstraints.NONE;
		panelConstraints.anchor = GridBagConstraints.CENTER;
		mainPanel.add(currentLevel, panelConstraints);

		panelConstraints.gridx = 1;
		panelConstraints.weightx = 0.0;
		mainPanel.add(playPauseButton, panelConstraints);

		panelConstraints.gridx = 2;
		mainPanel.add(currentScore, panelConstraints);

		panelConstraints.gridx = 3;
		mainPanel.add(currentTime, panelConstraints);
		
		panelConstraints.gridx = 4;
		panelConstraints.fill = GridBagConstraints.BOTH;
		mainPanel.add(progressBar, panelConstraints);

		gameContainer.add(gamePanel);

		panelConstraints.gridx = 0;
		panelConstraints.gridy = 1;
		panelConstraints.gridwidth = 5;
		panelConstraints.weightx = 1.0;
		panelConstraints.weighty = 1.0;
		panelConstraints.fill = GridBagConstraints.BOTH;
		gameContainer.setBackground(Color.BLACK);
		mainPanel.add(gameContainer, panelConstraints);
		
		panelConstraints.gridy = 2;
		panelConstraints.gridwidth = 1;
		panelConstraints.weightx = 1.0;
		panelConstraints.weighty = 0.0;
		
		for (int i = 0; i < 5; i++) {
			weapon[i] = new JButton();
			weapon[i].setEnabled(false);
			ImageIcon icon = new ImageIcon(Resources.Image.get(i + Resources.Image.WEAPON_START));
			Image image = icon.getImage().getScaledInstance(50, 50, java.awt.Image.SCALE_SMOOTH);
			icon = new ImageIcon(image);
			weapon[i].setIcon(icon);
			panelConstraints.gridx = i;
			mainPanel.add(weapon[i], panelConstraints);
			weapon[i].addActionListener(this);
		}
		weaponBlock[0] = 5;
		weaponBlock[1] = 5;
		weaponBlock[2] = 5;
		weaponBlock[3] = 5;
		weaponBlock[4] = 5;
		
		panelConstraints.gridx = 1;
		panelConstraints.weightx = 0.0;
		
		this.add(mainPanel);
		this.setMinimumSize(new Dimension(450, 450));
		this.setSize(new Dimension(600, 600));
		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.addWindowListener(this);
		this.setLocationRelativeTo(null);

		game.init();
		repaintTimer.start();
		counter.start();

		if(game.isPaused()) {
			playPauseButton.setIcon(new ImageIcon(Resources.Image.get(Resources.Image.PLAY_ICON)));
		} else {
			playPauseButton.setIcon(new ImageIcon(Resources.Image.get(Resources.Image.PAUSE_ICON)));
		}

		backgroundMusic = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.BACKGROUND_END - Resources.Audio.BACKGROUND_START + 1) + Resources.Audio.BACKGROUND_START);
		backgroundMusic.setFramePosition(0);
		backgroundMusic.addLineListener(this);
		backgroundMusic.start();

		this.setVisible(true);

	}

	/**
	 * Para atualizar o painel do jogo ou ao clicar em botoes.
	 * @param e evento do timer ou do botao
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		Object src;
		Clip c;
		int i;

		src = e.getSource();

		if(src == repaintTimer) {
			gamePanel.repaint();
			if(game.state == 1) {
				repaintTimer.stop();
				counter.stop();
				this.setVisible(false);
				Score s = new Score(name);
				s.setDuration(timer);
				s.setScore(game.score);
				ScoreDAO.add(s);
				home.updateScore();
				JOptionPane.showMessageDialog(this, "Você perdeu! Reprovando cálculo de novo? :(", "Fim de Jogo", JOptionPane.INFORMATION_MESSAGE);
				//home.setVisible(true);
				System.exit(0);
			} else if(game.state == 2) {
				repaintTimer.stop();
				counter.stop();
				this.setVisible(false);
				Score s = new Score(name);
				s.setDuration(timer);
				s.setScore(game.score);
				ScoreDAO.add(s);
				home.updateScore();
				JOptionPane.showMessageDialog(this, "Você venceu! Algum dia tinha que se formar, não é mesmo? :)", "Fim de Jogo", JOptionPane.INFORMATION_MESSAGE);
				//home.setVisible(true);
				System.exit(0);
			} else {
				if (GameScreen.game.getTower().life > 0) {
					progressBar.setValue((int) GameScreen.game.getTower().life);
				}
				currentScore.setText(String.format("%015d", game.score));
				currentLevel.setText(String.format("%dº semestre: P%d", game.semester + 1, game.test + 1));
				currentTime.setText(String.format("%02d:%02d:%02d", ((timer/60)/60), (timer / 60) % 60, timer % 60));
			}
		} else if(src == playPauseButton) {
			if(game.isPaused()) {
				game.unpause(playPauseButton);
				backgroundMusic.start();
				counter.start();
			} else {
				game.pause(playPauseButton);
				backgroundMusic.stop();
				counter.stop();
			}
		} else if (src == weapon[0]) {
			c = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.PENCIL_END - Resources.Audio.PENCIL_START + 1) + Resources.Audio.PENCIL_START);
			c.setFramePosition(0);
			c.start();
			game.damage(0);
			weapon[0].setEnabled(false);
			weaponBlock[0] = 5;
		} else if (src == weapon[1]) {
			c = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.BOOK_END - Resources.Audio.BOOK_START + 1) + Resources.Audio.BOOK_START);
			c.setFramePosition(0);
			c.start();
			game.damage(1);
			weapon[1].setEnabled(false);
			weaponBlock[1] = 10;
		} else if (src == weapon[2]) {
			c = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.POSSANI_END - Resources.Audio.POSSANI_START + 1) + Resources.Audio.POSSANI_START);
			c.setFramePosition(0);
			c.start();
			game.damage(2);
			weapon[2].setEnabled(false);
			weaponBlock[2] = 20;
		} else if (src == weapon[3]) {
			c = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.MESALVA_END - Resources.Audio.MESALVA_START + 1) + Resources.Audio.MESALVA_START);
			c.setFramePosition(0);
			c.start();
			game.damage(3);
			weapon[3].setEnabled(false);
			weaponBlock[3] = 40;
		} else if (src == weapon[4]) {
			c = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.WOLFRAM_END - Resources.Audio.WOLFRAM_START + 1) + Resources.Audio.WOLFRAM_START);
			c.setFramePosition(0);
			c.start();
			game.damage(4);
			weapon[4].setEnabled(false);
			weaponBlock[4] = 50;
		} else if (src == counter) {
			if(!game.isPaused())
				timer += 1;
			for(i = 0; i < 5; i++) {
				if (weaponBlock[i] > 1)
					weaponBlock[i]--;
				else
					weapon[i].setEnabled(true);
			}
		}
	}
	/**
	 * Confirma se o usuario deseja mesmo sair do jogo, e caso verdadeiro, se deseja salvar este.
	 * @param e evento da janela
	 */
	@Override
	public void windowClosing(WindowEvent e) {
		int confirmResult;

		confirmResult = JOptionPane.showConfirmDialog(this, "Deseja salvar o jogo antes de sair?", "Sair do Jogo",
				JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE);

		if(confirmResult == JOptionPane.YES_OPTION) {
			repaintTimer.stop();
			counter.stop();
			JFileChooser fileSaverDialog;
			fileSaverDialog = new JFileChooser();
			fileSaverDialog.setDialogTitle("Salvar jogo...");
			fileSaverDialog.setApproveButtonText("Salvar");
			fileSaverDialog.setDialogType(JFileChooser.SAVE_DIALOG);
			fileSaverDialog.setFileSelectionMode(JFileChooser.FILES_ONLY);
			fileSaverDialog.setMultiSelectionEnabled(false);
			if(fileSaverDialog.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
				ObjectOutputStream saveStream = null;
				try {
					saveStream = new ObjectOutputStream(new FileOutputStream(fileSaverDialog.getSelectedFile()));
					saveStream.writeObject(game);
					saveStream.writeObject(name);
					saveStream.writeLong(timer);
					JOptionPane.showMessageDialog(this, "Jogo salvo com sucesso!",
							"Jogo Salvo", JOptionPane.INFORMATION_MESSAGE);
					saveStream.close();
					this.setVisible(false);
					game.gameOver();
					game.unpause();
					//home.setVisible(true);
					System.exit(0);
				} catch(Exception exc) {
					JOptionPane.showMessageDialog(this, "Ocorreu um erro ao salvar o jogo. Verifique se há permissão para escrita.",
							"Erro ao Salvar", JOptionPane.WARNING_MESSAGE);
					exc.printStackTrace();
					//System.err.printf();
				}
			}
		} else if(confirmResult == JOptionPane.NO_OPTION) {
			repaintTimer.stop();
			counter.stop();
			this.setVisible(false);
			game.gameOver();
			game.unpause();
			//home.setVisible(true);
			System.exit(0);
		}
	}

	/**
	 * A janela perdeu o foco. eh desejavel que o jogo pause.
	 * @param e evento da janela
	 */
	@Override
	public void windowDeactivated(WindowEvent e) {
		if(game.state == 0 && !game.isPaused()) {
			game.pause(playPauseButton);
			backgroundMusic.stop();
			counter.stop();
		}
	}

	@Override
	public void update(LineEvent e) {
		if(backgroundMusic.isRunning() || game.isPaused()) {
			return;
		}
		backgroundMusic.removeLineListener(this);
		backgroundMusic = Resources.Audio.get(Utils.getRandom().nextInt(Resources.Audio.BACKGROUND_END - Resources.Audio.BACKGROUND_START + 1) + Resources.Audio.BACKGROUND_START);
		backgroundMusic.setFramePosition(0);
		backgroundMusic.addLineListener(this);
		backgroundMusic.start();
	}


	/* As implementacoes vazias abaixo sao apenas para satisfazer as interfaces da classe, mas nao sao usadas. */

	@Override
	public void windowActivated(WindowEvent e) { }

	@Override
	public void windowIconified(WindowEvent e) { }

	@Override
	public void windowDeiconified(WindowEvent e) { }

	@Override
	public void windowOpened(WindowEvent e) { }

	@Override
	public void windowClosed(WindowEvent e) { }

}
