package enemy;

import drawable.Drawable;
import game.GameScreen;
import resources.Resources;
import resources.Utils;

import java.awt.*;

@SuppressWarnings("serial")
public class Enemy extends Drawable {

	private int aspect, killScore;

	public Enemy(int semester, int test) {
		super();
		int i;

		this.life = (semester + 1) * (Utils.getRandom().nextInt(test + 1) + 1) * 5;
		this.speed = 325 - (Utils.getRandom().nextInt((semester + 1) * (test + 1) * 10 + 1) + (semester + 1) * 10);
		this.damage = (semester + 1) * (test + 1) * 2;
		this.aspect = Utils.getRandom().nextInt(10);
		this.killScore = (int) ((325 - this.speed) * this.life * this.damage);
		if(this.aspect < 2) {
			this.aspect = 0;
		} else if(this.aspect < 6) {
			this.aspect = 1;
		} else {
			this.aspect = 2;
		}
		switch(this.aspect) {
			case 0:
				// Object quadrado
				this.resource = Utils.getRandom().nextInt(Resources.Image.SQUARY_END - Resources.Image.SQUARY_START + 1) + Resources.Image.SQUARY_START;
				i = Utils.getRandom().nextInt(16) + 5;
				this.size = new Dimension(i, i);
				break;
			case 1:
				// Objeto horizontal
				this.resource = Utils.getRandom().nextInt(Resources.Image.HORIZONTAL_END - Resources.Image.HORIZONTAL_START + 1) + Resources.Image.HORIZONTAL_START;
				i = Utils.getRandom().nextInt(16) + 5;
				this.size = new Dimension(i, (int)(i / 1.5));
				break;
			case 2:
				// Object vertical
				this.resource = Utils.getRandom().nextInt(Resources.Image.VERTICAL_END - Resources.Image.VERTICAL_START + 1) + Resources.Image.HORIZONTAL_START;
				i = Utils.getRandom().nextInt(16) + 5;
				this.size = new Dimension((int)(i / 1.5), i);
				break;
		}
		this.position = new Point(-22, Utils.getRandom().nextInt(50 - this.size.height));
	}

	/**
	 * O código abaixo controla tudo relacionado ao inimigo e roda em uma thread separada.
	 *
	 */
	@Override
	public void run() {
		while(true) {
			super.run(); // Bloqueia a thread se tiver pausado.
			if(this.life <= 0) {
				this.resource = Resources.Image.DIED_ENEMY;
				Utils.sleep(500);
				Utils.acquireLock(GameScreen.game.mutex);
				GameScreen.game.score += this.killScore;
				GameScreen.game.getEnemies().remove(this);
				GameScreen.game.killedEnemies++;
				GameScreen.game.mutex.release();
				return;
			}
			if (this.getPosition().getX() + this.getSize().getWidth() >= GameScreen.game.getTower().getPosition().getX()) {
				Utils.acquireLock(GameScreen.game.getTower().mutex);
				GameScreen.game.getTower().life -= this.getDamage();
				GameScreen.game.getTower().mutex.release();
			} else {
				Point p = new Point((int) this.getPosition().getX() + 1, (int) this.getPosition().getY());
				this.setPosition(p);
			}
			Utils.sleep((long) (this.speed)); // Dorme a thread por speed ms (isso controla a velocidade)
		}
	}

}
