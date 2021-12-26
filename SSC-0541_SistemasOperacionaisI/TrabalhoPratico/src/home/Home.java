package home;

import game.Game;
import game.GameScreen;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.List;

import javax.swing.*;

import resources.Resources;
import resources.Utils;
import score.Score;
import score.ScoreDAO;

/**
 * Essa classe representa a janela de inicio.
 *
 */
@SuppressWarnings("serial")
public class Home extends JFrame implements ActionListener {

	private JPanel panel;
	private JButton
			newGameButton, loadGameButton;
	private JList<String> scoreBoard;
	private JScrollPane scrollPane;
	
	private String[] scores;
	/**
	 * Construtor da janela.
	 */
	public Home() {
		super("CALCULUS");
		GridBagConstraints panelConstraints;

		if(!Resources.load(this)) {
			JOptionPane.showMessageDialog(this, "Não foi possível carregar os recursos do jogo! Verifique se você executou no diretório (workspace) correto.", "Erro de Carregamento",
					JOptionPane.ERROR_MESSAGE);
			JOptionPane.showMessageDialog(this, "Os recursos são referenciados da seguinte maneira: src/resources.", "Referenciação de recursos",
					JOptionPane.WARNING_MESSAGE);
			Utils.fatalError();
		}
		
		panel = new JPanel();
		newGameButton = new JButton("Novo Jogo");
		loadGameButton = new JButton("Carregar Jogo");
		scrollPane = new JScrollPane();
		panelConstraints = new GridBagConstraints();
		
		updateScore();
		
		panel.setLayout(new GridBagLayout());
		newGameButton.addActionListener(this);
		loadGameButton.addActionListener(this);
		scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);

		panelConstraints.gridx = 0;
		panelConstraints.gridy = 0;
		panelConstraints.gridwidth = 2;
		panelConstraints.gridheight = 1;
		panelConstraints.weightx = 1.0;
		panelConstraints.weighty = 0.0;
		panelConstraints.insets = new Insets(5, 5, 5, 5);
		panelConstraints.fill = GridBagConstraints.BOTH;
		panel.add(newGameButton, panelConstraints);

		panelConstraints.gridy = 1;
		panel.add(loadGameButton, panelConstraints);

		panelConstraints.gridy = 2;
		panelConstraints.weighty = 1.0;
		panel.add(scrollPane, panelConstraints);

		panelConstraints.gridy = 3;
		panelConstraints.gridwidth = 1;
		panelConstraints.weightx = 1.0;
		panelConstraints.weighty = 0.0;
		panel.add(new JPanel(), panelConstraints);

		this.add(panel);
		this.setSize(new Dimension(400, 400));
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null); // Pra aparecer no centro da tela
		this.setVisible(true);
	}

	/**
	 * Metodo para lidar com cliques em botoes da janela.
	 *
	 * @param e evento do botao
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		JFileChooser fileChooseDialog;
		ObjectInputStream loadStream;
		Object src = e.getSource();
		Game game;
		String name;
		long timer;

		if (src == newGameButton) {
			String player = JOptionPane.showInputDialog(this, "Player: ", "Seu nome", JOptionPane.QUESTION_MESSAGE);
			if(player != null && !player.equals("")) {
				this.setVisible(false);
				new GameScreen(this, player);
			}
		} else if (src == loadGameButton) {
			fileChooseDialog = new JFileChooser();
			fileChooseDialog.setDialogTitle("Carregar jogo...");
			fileChooseDialog.setApproveButtonText("Carregar");
			fileChooseDialog.setDialogType(JFileChooser.OPEN_DIALOG);
			fileChooseDialog.setFileSelectionMode(JFileChooser.FILES_ONLY);
			fileChooseDialog.setMultiSelectionEnabled(false);
			if(fileChooseDialog.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
				 loadStream = null;
				try {
					loadStream = new ObjectInputStream(new FileInputStream(fileChooseDialog.getSelectedFile()));
					game = (Game) loadStream.readObject();
					name = (String) loadStream.readObject();
					timer = loadStream.readLong();
					new GameScreen(this, game, name, timer);
					this.setVisible(false);
					loadStream.close();
				} catch(Exception exc) {
					JOptionPane.showMessageDialog(this, "Ocorreu um erro ao carregar o jogo. Verifique se há permissão para leitura e se o arquivo existe.",
							"Erro ao Carregar", JOptionPane.WARNING_MESSAGE);
					exc.printStackTrace();
				}
			}
		}
	}
	
	public void updateScore() {
		List<Score> list = ScoreDAO.getAll();
		
		scores = new String [list.size()];
		for (int i = 0; i < list.size(); i++) {
			scores[i] = list.get(i).getPlayer() + "    -    Score: " + list.get(i).getScore() + "    Time: " + list.get(i).getDuration() + " sec";
		}
		
		if (scores.length == 0) {
			scores = new String [1];
			scores[0] = "No scores";
		}
		
		scoreBoard = new JList<>(scores);
		scrollPane.setViewportView(scoreBoard);
	}

}