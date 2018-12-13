package game;

import drawable.Drawable;
import resources.Utils;

import javax.swing.*;

import java.awt.*;

/**
 * Essa classe representa o painel em que o jogo sera desenhado.
 *
 */
@SuppressWarnings("serial")
public class GamePanel extends JPanel {

	/**
	 * Razao do painel de jogo (2:1). Preferivel = 1000x500
	 */
	private static final double ratio = 2 / 1.0;

	/**
	 * O metodo eh sobrescrito para poder desenhas os elementos do jogo no painel.
	 * A priori, este método  não deve ser modificado.
	 *
	 * @param g objeto grafico de desenho no painel
	 */
	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);

		Utils.acquireLock(GameScreen.game.mutex);
		for(Drawable dw : GameScreen.game.getEnemies()){
			dw.draw(g);
		}
		for(Drawable dw : GameScreen.game.getWeapons()) {
			dw.draw(g);
		}
		GameScreen.game.getTower().draw(g);
		GameScreen.game.mutex.release();
	}

	/**
	 * Usado para manter a razao de tela adequada.
	 *
	 * @return o tamanho ideal para o painel
	 */
	@Override
	public Dimension getPreferredSize() {
		int xPrefSize, yPrefSize;
		Dimension parentSize;

		parentSize = this.getParent().getSize();
		xPrefSize = (int) Math.min(parentSize.height*ratio, parentSize.width);
		yPrefSize = (int) Math.min(parentSize.width/ratio, parentSize.height);

		return new Dimension(xPrefSize, yPrefSize);
	}

}
