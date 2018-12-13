package tower;

import drawable.Drawable;
import game.GameScreen;
import resources.Resources;

import java.awt.*;

/**
 * Esta classe representa a torre que deve ser protegida.
 *
 */
@SuppressWarnings("serial")
public class Tower extends Drawable {

	public static final int maxLife = 1000;

	public Tower() {
		this.position = new Point(80, 0);
		this.size = new Dimension(20, 50);
		this.resource = Resources.Image.TOWER;
		this.life = maxLife;
	}

	/**
	 * O código abaixo controla tudo relacionado a torre e roda em uma thread separada.
	 *
	 */
	@Override
	public void run() {
		while(true) {
			super.run(); // Vai bloquear aqui se o jogo tiver pausado.
			if (this.life <= 0) {
				GameScreen.game.gameOver();
			}
			try { Thread.sleep(1000); } catch(Exception exc) { }
		}
	}

}
