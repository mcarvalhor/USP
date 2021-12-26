package drawable;

import game.GameScreen;
import resources.Resources;
import resources.Utils;

import java.awt.*;
import java.io.Serializable;
import java.util.concurrent.Semaphore;

/**
 * Essa classe representa um objeto que pode ser desenhado no painel do jogo
 * (Torre, Inimigo, Arma, ...).
 *
 */
@SuppressWarnings("serial")
public class Drawable implements Serializable, Runnable {

	/**
	 * Posicao (x, y).
	 */
	protected Point position;

	/**
	 * Tamanho (width, height);
	 */
	protected Dimension size;

	protected int x, y, width, height;

	/**
	 * ID da imagem que sera desenhada na posicao "position" e de tamanho
	 * "size".
	 */
	protected int resource;

	/**
	 * Vida, protecao e dano de ataque do objeto.
	 *
	 */
	public double life, speed, damage;

	/**
	 * Semaforo que controla o acesso unico (Mutex) aos recursos do objeto.
	 *
	 */
	public transient Semaphore mutex;

	/**
	 * Thread que controla o funcionamento deste objeto (enemy, weapon, tower,
	 * ...)
	 *
	 */
	public transient Thread runningThread;

	/**
	 * Tamanho da tela em que trabalharemos. Eh usado para calculo de escala.
	 */
	private static final Dimension progSize = new Dimension(100, 50);

	public void init() {
		runningThread = new Thread(this);
		mutex = new Semaphore(1);
		runningThread.start();
	}

	/**
	 * Desenha este elemento no painel
	 * 
	 * @param g
	 *            Objeto grafico de desenho do painel.
	 */
	public void draw(Graphics g) {
		Dimension currentSize;

		currentSize = g.getClipBounds().getSize();
		g.drawImage(Resources.Image.get(this.resource),
				(position.x * currentSize.width) / progSize.width,
				(position.y * currentSize.height) / progSize.height,
				(size.width * currentSize.width) / progSize.width,
				(size.height * currentSize.height) / progSize.height, null);
	}

	@Override
	public void run() {
		Utils.acquireLock(GameScreen.game.pause);
		GameScreen.game.pause.release();
		if(GameScreen.game.state != 0)
			return;
	}

	public Point getPosition() {
		return position;
	}

	public void setPosition(Point position) {
		this.position = position;
	}

	public int getResource() {
		return resource;
	}

	public void setResource(int resource) {
		this.resource = resource;
	}

	public Dimension getSize() {
		return size;
	}

	public void setSize(Dimension size) {
		this.size = size;
	}

	public void setDamage(double damage) {
		this.damage = damage;
	}

	public double getDamage() {
		return this.damage;
	}

	public void setLife(double life) {
		this.life = life;
	}

	public double getLife() {
		return this.life;
	}

	public void setSpeed(double armor) {
		this.speed = armor;
	}

	public double getSpeed() {
		return this.speed;
	}

}
