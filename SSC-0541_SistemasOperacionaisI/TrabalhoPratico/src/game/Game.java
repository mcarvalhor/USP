package game;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import drawable.Drawable;
import resources.Resources;
import resources.Utils;
import score.Score;
import tower.Tower;
import weapon.Weapon;
import enemy.Enemy;

/**
 * Essa classe representa um jogo e todos os seus possiveis estados.
 *
 */
@SuppressWarnings("serial")
public class Game implements Serializable, Runnable {

	/**
	 * Semaforos de pause e dos objetos.
	 *
	 */
	public transient Semaphore pause;
	public transient Semaphore mutex;
	public transient Thread runningThread;
	private boolean paused;
	public transient int state;

	public long score;

	/**
	 * Lista dos inimigos, armas e torre do jogo.
	 *
	 */
	private ArrayList<Enemy> enemies;
	private ArrayList<Weapon> weapons;
	private Tower tower;
	public int semester, test, killedEnemies, totalEnemies;

	public Game() {
		enemies = new ArrayList<>();
		weapons = new ArrayList<>();
		tower = new Tower();
		paused = false;
		state = semester = test = killedEnemies = 0;
		totalEnemies = 5;
	}

	public void init() {
		runningThread = new Thread(this);
		pause = new Semaphore((paused) ? 0 : 1);
		mutex = new Semaphore(1);
		for(Enemy e : enemies) {
			e.init();
		}
		for(Weapon w : weapons) {
			w.init();
		}
		tower.init();
		state = 0;

		// inicia o jogo
		runningThread.start();
	}

	public void damage(int type) {
		Weapon w;

		w = new Weapon(type);
		Utils.acquireLock(this.mutex);
		weapons.add(w);
		this.mutex.release();
		w.init();
	}

	/**
	 * Metodo que roda em thread separada para controle do jogo.
	 */
	@Override
	public void run() {
		Drawable ens[], wes[], tow;
		Enemy e;
		while(true) {
			Utils.acquireLock(GameScreen.game.pause);
			GameScreen.game.pause.release();
			if(GameScreen.game.state != 0) {
				Utils.acquireLock(mutex);
				ens = (Enemy[]) enemies.toArray(new Enemy[0]);
				wes = (Weapon[]) weapons.toArray(new Weapon[0]);
				tow = (Tower) tower;
				mutex.release();
				for(Drawable dw : ens) {
					Utils.joinThread(dw.runningThread);
				}
				for(Drawable dw : wes) {
					Utils.joinThread(dw.runningThread);
				}
				Utils.joinThread(tow.runningThread);
				return;
			}
			if(semester >= 5) {
				gameWon();
			} else if(test >= 5) {
				semester++;
				test = 0;
			} else if(killedEnemies >= totalEnemies) {
				test++;
				killedEnemies = 0;
				totalEnemies = (test + 1) * (semester + 1) * 2;
			} else {
				//if (Utils.getRandom().nextInt(100) < 25) {
					e = new Enemy(semester, test);
					Utils.acquireLock(mutex);
					enemies.add(e);
					e.init();
					mutex.release();
				//}
				Utils.sleep(2500 - ((semester + 1) * (test + 1) * 100));
				continue;
			}
			Thread.yield();
		}
	}

	public boolean isPaused() {
		return paused;
	}

	public void pause() {
		try {
			pause.acquire();
		} catch (InterruptedException exc) {
			System.err.printf("Could not acquire pause lock: %s.",
					exc.getMessage());
			Utils.fatalError();
		}
		paused = true;
	}

	public void pause(JButton playPauseButton) {
		pause();
		playPauseButton.setIcon(new ImageIcon(Resources.Image
				.get(Resources.Image.PLAY_ICON)));
	}

	public void unpause() {
		pause.release();
		paused = false;
	}

	public void unpause(JButton playPauseButton) {
		unpause();
		playPauseButton.setIcon(new ImageIcon(Resources.Image
				.get(Resources.Image.PAUSE_ICON)));
	}

	public void gameOver() {
		state = 1;
	}

	public void gameWon() {
		state = 2;
	}

	public ArrayList<Enemy> getEnemies() {
		return enemies;
	}

	public ArrayList<Weapon> getWeapons() {
		return weapons;
	}

	public void setWeapons(ArrayList<Weapon> weapons) {
		this.weapons = weapons;
	}

	public Tower getTower() {
		return tower;
	}

	public void setTower(Tower tower) {
		this.tower = tower;
	}

}
