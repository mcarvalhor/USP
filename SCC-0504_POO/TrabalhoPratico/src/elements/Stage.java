package elements;

import control.GameFrame;
import utils.Consts;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class Stage {

	GameFrame gamePlayingFrame;
	Square[][] gameSquares;
	ArrayList<Element> elemArray;

	/**
	 * Cria um novo nível de jogo.
	 * @param obstacles indica se o nível terá obstáculos.
	 */
	public Stage(boolean obstacles) {
		gameSquares = new Square[Consts.NUM_LINES][Consts.NUM_COLUMNS];
		for (Square[] s : gameSquares) Arrays.fill(s, null);

		elemArray = new ArrayList<Element>();

		if(obstacles) { // Vamos ter que adicionar alguns obstáculos no jogo.

			Random rand = new Random();
			int counter = rand.nextInt(3) + 1;
			System.out.println("Sortenado " + counter + "quadrados.");
			for(int i = 0; i < counter; i++) {
				gameSquares[rand.nextInt(5) + 5][rand.nextInt(5) + 5] = new Square();
			}

		}

		gamePlayingFrame = new GameFrame(elemArray, gameSquares);
	}

	/**
	 * Cria um novo nível de jogo carregado do disco.
	 * @param gameFrame o jogo carregado do disco.
	 */
	public Stage(GameFrame gameFrame) {
		gamePlayingFrame = gameFrame;
	}

	/**
	 * Inicia um jogo carregado.
	 */
	public void start() {
		gamePlayingFrame.setVisible(true);
		gamePlayingFrame.createBufferStrategy(2);
		gamePlayingFrame.go();
	}

}
