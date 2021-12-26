package control;

import elements.*;
import utils.Consts;
import utils.Drawing;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Janela principal do Jogo em ação
 * <p>
 *     Classe que implementa o jogo propriamente dito
 * </p>
 *
 *
 *
 * @author Sergio Piza, Brendon Hudson, Matheus C.
 *  - Baseado em material do Prof. Luiz Eduardo
 */
public class GameFrame extends javax.swing.JFrame {
    private static final boolean FREE = false;
    private static final boolean OCCUPED = true;

    private ArrayList<Element> elemArray;
    private GameController controller;
    private Pontuation points;

    protected TetrisObject currentTetrisObject;
    private Square[][] gameSquares;  // Cada quadrado do game
    private boolean isFull;

    private int gameScreenWidth;
    private int gameScreenHeight;

	/*Placar provisório*/
	private static JDialog placar;
	private static JLabel placarScore;


	static {
		placar = new JDialog();
		placarScore = new JLabel("Score: 0");
		//placarScore.setFont(Consts.SCORE_FONT);

		placar.add(placarScore);
		placar.setSize(200,100);
	}

	/**
	 * Inicializa uma tela de jogo com seus elementos e obstáculos.
	 * @param elemArray elementos no jogo
	 * @param gameSquares inicia com os obstáculos (se houverem)
	 */
	public GameFrame(ArrayList<Element> elemArray, Square[][] gameSquares) {
		Drawing.setGameFrame(this);
		initComponents();

		controller = new GameController(this); /*Controlador para o jogo atual*/
		points = new Pontuation(Consts.BASE_POINT_INC); /*Sistema de pontuação*/

		/*O objeto GameController controller passa a "ouvir" o teclado*/
		this.addKeyListener(controller);

		gameScreenWidth = Consts.NUM_COLUMNS * Consts.CELL_SIZE + getInsets().left + getInsets().right;
		gameScreenHeight = Consts.NUM_LINES * Consts.CELL_SIZE + getInsets().top + getInsets().bottom;

		/*Cria a janela do tamanho do tabuleiro + insets (bordas) da janela*/
		this.setSize(gameScreenWidth, gameScreenHeight);

		this.elemArray = elemArray;

		/*Inicializa matriz de controle de blocos ocupados na tela do game*/
		this.gameSquares = gameSquares;
		isFull = false;

		currentTetrisObject = null;

		initScore();
		/*Lança a primeira peça*/
		playGame();
	}

	/**
	 * Inicializa um jogo salvo em disco.
	 * @param gameFile o Stream que representa o arquivo em disco
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public GameFrame(ObjectInputStream gameFile) throws IOException, ClassNotFoundException {
		Drawing.setGameFrame(this);
		initComponents();
		this.elemArray = (ArrayList<Element>) gameFile.readObject();
		this.points = (Pontuation) gameFile.readObject();
		this.gameSquares = (Square[][]) gameFile.readObject();
		this.isFull = gameFile.readBoolean();
		this.gameScreenWidth = gameFile.readInt();
		this.gameScreenHeight = gameFile.readInt();
		this.controller = new GameController(this);
		this.addKeyListener(controller);
		this.setSize(gameScreenWidth, gameScreenHeight);
		this.currentTetrisObject = null;
		initScore();
		playGame();
	}

	/**
	 * Inicializa o Frame do Score.
	 *
	 */
	private void initScore() {
		placar.setLocation(gameScreenWidth+15,0);
		placar.transferFocus();
		placarScore.setText("Score: "+ points.getPoints());
		placar.setVisible(true);
		this.requestFocus();
	}

    /**
     * Método que inicia a lógica do game
     *
     * <p>
     *     Enquanto a "matriz" do game não está coberta até o topo com peças,
     *     é mandado mais peças para o jogador alocar
     * </p>
     */
    private void playGame() {

        /*Gera aleatóriamente peças do game até chegar ao topo*/
        if (!isFull) {
	        currentTetrisObject = new TetrisObject(this);

            /*Adiciona os blocos da peça ao array de elementos do frame*/
            for (int i = 0; i < 4; i++) {
                this.addElement(currentTetrisObject.pieces[i]);
            }
        }
    }

    /**
     * Método que finaliza o jogo e retorna para o Menu
     */
    private void finishGame() {
	    Main.MAIN_MENU.setVisible(true);
	    this.setVisible(false);
	    placar.setVisible(false);
    }

    /**
     * Método que serializa o GameFrame atual, salvando o jogo
     */
    void saveGame() {
	    JFileChooser fileSaverDialog;
	    fileSaverDialog = new JFileChooser();
	    fileSaverDialog.setDialogTitle("Salvar jogo...");
	    fileSaverDialog.setApproveButtonText("Salvar");
	    fileSaverDialog.setDialogType(JFileChooser.SAVE_DIALOG);
	    fileSaverDialog.setFileSelectionMode(JFileChooser.FILES_ONLY);
	    fileSaverDialog.setMultiSelectionEnabled(false);
	    if(fileSaverDialog.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
		    ObjectOutputStream saveStream = null;
		    try {
			    //currentTetrisObject = null;
			    for (int i = 0; i < 4; i++) {
				    this.removeElement(currentTetrisObject.pieces[i]);
			    }
			    saveStream = new ObjectOutputStream(new FileOutputStream(fileSaverDialog.getSelectedFile()));
			    saveStream.writeObject(elemArray);
			    saveStream.writeObject(points);
			    //saveStream.writeObject(currentTetrisObject);
			    saveStream.writeObject(gameSquares);
			    saveStream.writeBoolean(isFull);
			    saveStream.writeInt(gameScreenWidth);
			    saveStream.writeInt(gameScreenHeight);
			    saveStream.close();
			    finishGame();
			    JOptionPane.showMessageDialog(Main.MAIN_MENU, "Jogo salvo",
					    "Salvo com sucesso", JOptionPane.WARNING_MESSAGE);
		    } catch(IOException e) {
			    JOptionPane.showMessageDialog(this, e.getMessage(),
					    "Problema no salvamento", JOptionPane.WARNING_MESSAGE);
		    }
	    }
    }

    private void addElement(Element elem) {
        elemArray.add(elem);
    }

    public void removeElement(Element elem) {
        elemArray.remove(elem);
    }

    @Override
    /**
     * Método que aparentemente desenha o jogo constantemente (Brendon)
     */
    public void paint(Graphics gOld) {
        Graphics g = getBufferStrategy().getDrawGraphics();

        /*Criamos um contexto grafico*/
        Graphics g2 = g.create(getInsets().right, getInsets().top, getWidth() - getInsets().left, getHeight() - getInsets().bottom);

        /* DESENHA CENARIO
           Trocar essa parte por uma estrutura mais bem organizada
           Utilizando a classe Stage
        */
        for (int i = 0; i < Consts.NUM_LINES; i++) {
            for (int j = 0; j < Consts.NUM_COLUMNS; j++) {
                try {
                    Image newImage = Toolkit.getDefaultToolkit().getImage(new java.io.File(".").getCanonicalPath() + Consts.IMG_PATH + "bricks.png");
                    g2.drawImage(newImage, j * Consts.CELL_SIZE,
                                           i * Consts.CELL_SIZE,
                                              Consts.CELL_SIZE,
                                              Consts.CELL_SIZE, null);

                } catch (IOException ex) {
                    Logger.getLogger(GameFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }

        /*Desenha todos os elementos do "elemArray" na tela e escreve a peça atual*/
        this.controller.drawAllElements(elemArray, g2);
        this.controller.processAllElements(elemArray);
        this.setTitle("Next: " + currentTetrisObject.getNextType().toString());

        /*------------CONDICIONAIS IMPORTANTES----------------------
         *--------------------------------------------------------------------
         * 1º Verifica se a peça tocou em algo e se já deu o "TIME_FIRE"
         * 2º Ocupa a "gameSquares" com cada Square do GameObject (peça)
         * -------------------------------------------------------------------*/
        if (objLowerBoundsIsOccuped(currentTetrisObject) && currentTetrisObject.pieces[0].getContIntervals() == Square.TIMER_FIRE - 1) {
            setDeactivatedPiece();
        }
	    g2.drawString("Pontuação: " + points.getPoints(), 0, 0);
	    g.dispose();
	    g2.dispose();
	    if (!getBufferStrategy().contentsLost()) {
		    getBufferStrategy().show();
	    }
    }

    /**
     * Método que adiciona um objeto do tipo "GameObject" quadrado por quadrado
     * na matriz "gameSquares" que representa a tela do jogo.
     * @param obj objeto a ser adicionado
     */
    private void addGameObjectSquares(GameObject obj) {
        for (Square s : obj.pieces) {
            gameSquares[s.getPos().getX()][s.getPos().getY()] = s;
        }
    }

    /**
     * Verifica se há alguma linha preenchida atualmente
     * @return true, se sim, false, se não.
     */
    private boolean hasFilledRow() {
        /*Para cada linha, verificar se todos seus quadrados estão preenchidos*/
        for (int i = Consts.NUM_LINES-1; i >= 0 ; i--)
            if (isFilledRow(i)) return true;

        return false;
    }

    /**
     * Verifica se uma linha está preenchida
     * @param rowNumber número da linha a ser verificada
     * @return true, se estiver, false, se não estiver.
     */
    private boolean isFilledRow(int rowNumber) {
        for(Square s : gameSquares[rowNumber])
            if (s == null) return false;

        return true;
    }

    /**
     * Libera as linhas preenchidas
     * @return número de linhas liberadas
     */
    private int freeFilledRows() {
        int freedRows = 0;
        /*Para cada linha preenchida, libero seus squares para descerem*/
        for (int i = Consts.NUM_LINES-1; i >= 0; i--) {

            /*Enquanto ela estiver preenchida*/
            while (isFilledRow(i)) {
                freedRows++;

                /*1. Removo seus elementos*/
                for (Square s : gameSquares[i]) {
                    gameSquares[s.getPos().getX()][s.getPos().getY()] = null;
	                s.erase();
                }

                /*2. Desço todos os Squares que estavam acima*/
	            for(int j = i; j > 0; j--) {
		            for(int k = 0; k < Consts.NUM_COLUMNS; k++) {
			            if (gameSquares[j][k] != null && gameSquares[j][k].isObstacle)
				            continue;
			            gameSquares[j][k] = gameSquares[j - 1][k];
			            if(gameSquares[j][k] != null)
			                gameSquares[j][k].setPosition(gameSquares[j][k].getPos().getX() + 1, gameSquares[j][k].getPos().getY());
		            }
	            }
	            for(int j = 0; j < Consts.NUM_COLUMNS; j++)
		            gameSquares[0][j] = null;


            }
        }
        return freedRows;
    }

    /**
     * Método que verifica se o objeto está encostando em algo em baixo
     * <p>
     *     Para cada um dos Squares que compõem o objeto, é testado se há bloco
     *     bloqueado na "gameSquares" em baixo, se houver, é retornado
     *     <em>true</em>, se não houver nada, <em>false</em>.
     * </p>
     * @param obj um GameObject qualquer
     * @return true or false
     */
    public boolean objLowerBoundsIsOccuped(GameObject obj) {
        int x, y;

        for (Square s : obj.pieces) {
            x = s.getPos().getX(); y = s.getPos().getY();

            /*Se o Square atual está adjacente ao chão, true*/
            if (x == Consts.NUM_LINES-1) return true;

            /*Se está adjacente a qualquer outro obstaculo de "gameSquares", true*/
            else if (gameSquares[x + 1][y] != null) return true;

        }
        return false;
    }

	public boolean objUpperBoundsAreOccupied(GameObject obj) {
		int x, y;

		for (Square s : obj.pieces) {
			x = s.getPos().getX(); y = s.getPos().getY();

			/*Se o Square atual está adjacente ao chão, true*/
			if (x == 0) return true;

				/*Se está adjacente a qualquer outro obstaculo de "gameSquares", true*/
			else if (gameSquares[x - 1][y] != null) return true;

		}
		return false;
	}

    /**
     * Método que verifica se o objeto está encostando em algo a esquerda
     * <p>
     *     Para cada um dos Squares que compõem o objeto, é testado se há bloco
     *     bloqueado na "gameSquares" a esquerda, se houver, é retornado
     *     <em>true</em>, se não houver nada, <em>false</em>.
     * </p>
     * @param obj um GameObject qualquer
     * @return true or false
     */
    public boolean objLeftBoundsIsOccuped(GameObject obj) {
        int x, y;
        for (Square s : obj.pieces) {
            x = s.getPos().getX();
            y = s.getPos().getY();

            /*O Square atual não pode estar adjacente a parede esquerda*/
            if (y == 0) return true;

            /*A posição adjacente esquerda não pode estar ocupada*/
            else if (gameSquares[x][y-1] != null) return true;

        }
        return false;
}

    /**
     * Método que verifica se o objeto está encostando em algo a direita
     * <p>
     *     Para cada um dos Squares que compõem o objeto, é testado se há bloco
     *     bloqueado na "gameSquares" a direita, se houver, é retornado
     *     <em>true</em>, se não houver nada, <em>false</em>.
     * </p>
     * @param obj um GameObject qualquer
     * @return true or false
     */
    public boolean objRightBoundsIsOccuped(GameObject obj) {
        int x, y;
        for (Square s : obj.pieces) {
            x = s.getPos().getX();
            y = s.getPos().getY();

            /*O Square atual não pode estar adjacente a parede esquerda*/
            if (y == Consts.NUM_COLUMNS - 1) return true;

            /*A posição adjacente esquerda não pode estar ocupada*/
            else if (gameSquares[x][y+1] != null) return true;

        }
        return false;
    }

    /**
     * Método que cria um TimerTask para dar constantes "repaint()" ao frame
     * até que "isFull" seja verdadeiro
     */
    public void go() {
        TimerTask task = new TimerTask() {
            public void run() {
                repaint();
            }
        };
        Timer timer = new Timer();
        timer.schedule(task, 0, Consts.DELAY_SCREEN_UPDATE);
    }


	/**
	 * Desativa uma peça.
	 *
	 */
	public void setDeactivatedPiece() {
		currentTetrisObject.desactivatePieces();

		/*MARCA COMO OCUPADO ONDE A PEÇA CAIU e adiciona seus squares a "rowsSquares"*/
		addGameObjectSquares(currentTetrisObject);

		/*VERIFICA SE HOUVE PONTUAÇÃO*/
		if (hasFilledRow()) {
			int multPontuation = freeFilledRows();
			points.gain(multPontuation);
			placarScore.setText("Score: "+ points.getPoints());
			System.out.println("Pontuação: " + points.getPoints());
		}
		/*Possível GAME_OVER*/
		else if (currentTetrisObject.getObjectBoundaries().highestX < 3) {
			System.out.println("GAME OVER");
			isFull = true;

			finishGame();
			JOptionPane.showMessageDialog(Main.MAIN_MENU, "Game Over",
					"Game Over", JOptionPane.WARNING_MESSAGE);
		}

		/*Lança uma NOVA PEÇA*/
		playGame();
	}

    /*Provavelmente Trecho herdado do GUI Form do netbeans, onde foi originalmente implementado o frame*/
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle(Consts.GAME_NAME);
        setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        setLocation(new java.awt.Point(20, 20));
        setResizable(false);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGap(0, 500, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGap(0, 500, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents
    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables


}
