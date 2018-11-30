package control;

import com.sun.xml.internal.bind.v2.runtime.reflect.opt.Const;
import elements.Stage;
import utils.Consts;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;


/**
 * <b>Classe que implementa o menu do game e suas funcionalidades</b>
 *     <p>
 *         No primeiro momento é verificado se há ou não jogos salvos no diretório do game,
 *         se houver, a interface do botão "Continuar" fica normal, se não, fica como "clicado",
 *         não sendo possível escolher essa opção.
 *         <br>
 *         &emsp;Ao clicar em "Novo Jogo" (newGameButton), inicializa um jogo novo
 *         <br>
 *         &emsp;Ao clicar em "Continuar (continueButton), quando possível, é carregado um estado
 *         de jogo previamente salvo no diretório do jogo.
 *     </p>
 *
 * @author Brendon Hudson
 **/
public class GameMenu extends JFrame implements ActionListener {
    private Stage gameScreen;

    private JPanel GameMenu;
    private JPanel Buttons;
    private JPanel Title;
    private JButton continueButton;
    private JButton newGameButton;
    private JCheckBox challengeCheckBox;

    private JPanel background;
    private ImageIcon imgBackground;

    /**
     * Inicializa a tela de menu com seus botões e opções, verificando se há gravações
     * para continuar o jogo, ou, apenas iniciar um novo
     */
    GameMenu() {
        setTitle(Consts.GAME_NAME);
        setResizable(false);
        add(GameMenu); // Este JPanel contém os outros componentes

        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        this.setSize(Consts.NUM_COLUMNS * Consts.CELL_SIZE + getInsets().left + getInsets().right,
                Consts.NUM_LINES * Consts.CELL_SIZE + getInsets().top + getInsets().bottom);

        // Listeners para os botões
        newGameButton.addActionListener(this);
        continueButton.addActionListener(this);
    }

    /**
     * Inicia um novo jogo a partir de uma nova instância de Stage
     */
    private void startNewGame() {
        gameScreen = new Stage(challengeCheckBox.isSelected());
        this.setVisible(false);
        gameScreen.start();
    }

    /**
     * Método que responde as ações no Menu do Jogo
     * <p>
     *     <b>Novo Jogo</b>
     *         <p>Simplesmente, inicia um jogo do zero, verificando se o jogador deseja ir para um nível mais desafiador.</p>
     *
     *
     *     <b>Continuar</b>
     *         <p>Des-serializa o Objeto "GameFrame" serializado, referente a uma
     *             gravação previamente salva por um jogador, e então, inicia o jogo
     *             com o método startGame(savedGameScreen).
     *
     *     <b>Me desafie:</b>
     *         <p>Se marcado, o jogo iniciará um nível a frente.</p>
     *
     * @param e Evento gerado no frame do menu por algum de seus botões
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        Object src = e.getSource();

        if (src == newGameButton) {
            startNewGame();
        }
        else if (src == continueButton) {
            JFileChooser fileLoaderDialog;
            fileLoaderDialog = new JFileChooser();
            fileLoaderDialog.setDialogTitle("Carregar jogo...");
            fileLoaderDialog.setApproveButtonText("Abrir");
            fileLoaderDialog.setDialogType(JFileChooser.OPEN_DIALOG);
            fileLoaderDialog.setFileSelectionMode(JFileChooser.FILES_ONLY);
            fileLoaderDialog.setMultiSelectionEnabled(false);
            if(fileLoaderDialog.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
                ObjectInputStream loadStream = null;
                try {
                    loadStream = new ObjectInputStream(new FileInputStream(fileLoaderDialog.getSelectedFile()));
                    GameFrame savedGameScreen = new GameFrame(loadStream);
                    loadStream.close();
                    gameScreen = new Stage(savedGameScreen);
                    this.setVisible(false);
                    gameScreen.start();
                } catch (IOException | ClassNotFoundException exc) {
                    // Gera uma caixa de dialogo com o problema do carregamento do jogo
                    JOptionPane.showMessageDialog(this, exc.getMessage(),
                            "Problema no carregamento", JOptionPane.WARNING_MESSAGE);
                }
            }
        }
    }
}
