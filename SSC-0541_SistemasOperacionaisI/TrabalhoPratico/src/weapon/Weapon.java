package weapon;

import drawable.Drawable;
import enemy.Enemy;
import game.GameScreen;
import resources.Resources;
import resources.Utils;

import java.awt.*;

@SuppressWarnings("serial")
public class Weapon extends Drawable {

	public Weapon(int type) {
		this.resource = Resources.Image.WEAPON_START + type;
		this.size = new Dimension(20, 20);
		this.position = new Point(40,15);
		switch(type) {
			case 0:
				this.damage = 5;
				break;
			case 1:
				this.damage = 10;
				break;
			case 2:
				this.damage = 20;
				break;
			case 3:
				this.damage = 40;
				break;
			case 4:
				this.damage = 80;
				break;
		}
	}

	/**
	 * Roda em thread separada.
	 */
	@Override
	public void run() {
		super.run();
		for(Enemy e: GameScreen.game.getEnemies()) {
			Utils.acquireLock(e.mutex);
			e.setLife(e.getLife() - this.damage);
			e.mutex.release();
		}
		Utils.sleep(1000);
		Utils.acquireLock(GameScreen.game.mutex);
		GameScreen.game.getWeapons().remove(this);
		GameScreen.game.score += this.damage * 10;
		GameScreen.game.mutex.release();
	}
}
