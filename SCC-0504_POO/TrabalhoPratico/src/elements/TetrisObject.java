package elements;

import control.GameFrame;
import utils.Consts;
import utils.Position;

import java.io.Serializable;
import java.util.Random;

/**
 * Classe que implementa uma peça qualquer do jogo
 *
 * <p> descrição detalhada do funcionamento da classe... </p>
 *
 * @author Sergio Piza, Brendon Hudson
 */
public class TetrisObject extends GameObject implements Serializable {
    public static TetrisObjectType nextType;
	private TetrisObjectType type;
	private short rotatePosition;
	private GameFrame gameFrame;

	static {
	    nextType = TetrisObjectType.values()[new Random().nextInt(TetrisObjectType.values().length)];
    }

	/**
	 * Inicializa uma nova peça aleatória
	 */
	public TetrisObject(GameFrame gameFrame) {
	    type = nextType;
		nextType = TetrisObjectType.values()[new Random().nextInt(TetrisObjectType.values().length)];
		squaresNumber = 4;
		pieces = new Square[squaresNumber];
		isActive = true;
		rotatePosition = 0;
		this.gameFrame = gameFrame;

		// Para debug
		System.out.println("Novo TetrisObject: " + type);


		/*Monta a peça de acordo com seu tipo: I, J, O, L, S, T, Z (TetrisObjectType)*/
		switch (type) {
			case I:
				for (int i = 0; i < squaresNumber; i++) {
					pieces[i] = new Square(Square.LIGHT_BLUE);
					pieces[i].setPosition(0, Consts.NUM_COLUMNS / 2 + i - 2);
				}
				break;

			case J:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.DARK_BLUE);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 2);
				break;

			case O:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.YELLOW);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				break;

			case L:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.ORANGE);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2 + 2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 2);
				break;

			case S:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.GREEN);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2 - 1);
				pieces[2].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[3].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				break;

			case T:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.PURPLE);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2 - 1);
				pieces[2].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				break;

			case Z:
				for (int i = 0; i < 4; i++) pieces[i] = new Square(Square.RED);
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				pieces[2].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 2);
				break;
		}
	}

	public static TetrisObjectType getNextType() {
	    return nextType;
    }

	/**
	 * Devolve o tipo enum correspondente a peça
	 * @return enum com valor da letra correspondente a peça
	 */
	public TetrisObjectType getType() {
		return type;
	}

	/**
	 *  Método auxiliar para as rotações das peças
	 */


	public void rotate() {

		Position keyPos = pieces[2].pos;

		if (this.type == TetrisObjectType.O) return;

		switch (rotatePosition) {
			case 0:
				rotatePos1(keyPos);
				rotatePosition = 1;
				break;
			case 1:
				rotatePos2(keyPos);
				rotatePosition = 2;
				break;
			case 2:
				rotatePos3(keyPos);
				rotatePosition = 3;
				break;
			case 3:
				rotatePos0(keyPos);
				rotatePosition = 0;
				break;
		}

		System.out.println("rotate position " + rotatePosition);
	}

	/**
	 * Rotaciona a peça para posição inicial
	 */
	void rotatePos0(Position keyPos) {
		/*Rotaciona a peça de acordo com seu tipo: I, J, O, L, S, T, Z (TetrisObjectType)*/

		switch (type) {
			case I:
				if (!gameFrame.objRightBoundsIsOccuped(this)) {
					this.shiftRight();

					if (!gameFrame.objRightBoundsIsOccuped(this)) {
						this.shiftRight();


						//if (!gameFrame.objRightBoundsIsOccuped(this)) {
						pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 2);
						pieces[1].setPosition(keyPos.getX(), keyPos.getY() - 1);
						pieces[3].setPosition(keyPos.getX(), keyPos.getY() + 1);
						//}

						if (!gameFrame.objLeftBoundsIsOccuped(this)) this.shiftLeft();
						if (!gameFrame.objLeftBoundsIsOccuped(this)) this.shiftLeft();
						break;

					} else this.shiftLeft();
				}

				if (!gameFrame.objLeftBoundsIsOccuped(this)) {
					this.shiftLeft();

					if (!gameFrame.objLeftBoundsIsOccuped(this)) {
						//this.shiftRight();

						//if (!gameFrame.objRightBoundsIsOccuped(this)) {
						pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 2);
						pieces[1].setPosition(keyPos.getX(), keyPos.getY() - 1);
						pieces[3].setPosition(keyPos.getX(), keyPos.getY() + 1);
						//}

						break;

					} else this.shiftLeft();
				}

				break;

			case J:
				try {
					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}
					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY() - 1);
				}
				break;


			case L:
				try {
					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}
					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY() + 1);
				}
				break;

			case S:
				try {
					if (!gameFrame.objLeftBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objLeftBoundsIsOccuped(this) && !gameFrame.objRightBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() -1, keyPos.getY() + 1);
				}
				break;

			case T:
				try {
					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}
					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY());
				}
				break;

			case Z:
				try {
					if (!gameFrame.objLeftBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objLeftBoundsIsOccuped(this) && !gameFrame.objRightBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() - 1, keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX(), keyPos.getY() + 1);
				}
				break;
		}

	}

	/**
	 * Rotaciona a peça para a segunda posição
	 */
	void rotatePos1(Position keyPos) {
		/*Monta a peça de acordo com seu tipo: I, J, O, L, S, T, Z (TetrisObjectType)*/
		switch (type) {
			case I:
				if (!gameFrame.objLowerBoundsIsOccuped(this)) {
					pieces[0].setPosition(keyPos.getX() - 2, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case J:
				try {

					if (gameFrame.objLowerBoundsIsOccuped(this)) {
						if (!gameFrame.objUpperBoundsAreOccupied(this)) {
							this.shiftUp();

						}
					}

					throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() - 1, keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY());
				}

				break;

			case O:
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				break;

			case L:
				try {

					if (gameFrame.objLowerBoundsIsOccuped(this)) {
						if (!gameFrame.objUpperBoundsAreOccupied(this)) {
							this.shiftUp();

						}
					}

					throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() + 1, keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY());
				}

				break;

			case S:
				try {
					if (gameFrame.objLowerBoundsIsOccuped(this)) {
						if (!gameFrame.objUpperBoundsAreOccupied(this)) {
							this.shiftUp();
							throw new Exception();
						}
					} else throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY() + 1);
				}
				break;

			case T:
				try {

					if (gameFrame.objLowerBoundsIsOccuped(this)) {
						if (!gameFrame.objUpperBoundsAreOccupied(this)) {
							this.shiftUp();

						}
					}

					throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case Z:
				try {
					if (gameFrame.objLowerBoundsIsOccuped(this)) {
						if (!gameFrame.objUpperBoundsAreOccupied(this)) {
							this.shiftUp();
							throw new Exception();
						}
					} else throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY() + 1);
				}
				break;
		}
	}

	/**
	 * Rotaciona a peça para a terceira posição
	 */
	void rotatePos2(Position keyPos) {
		/*Monta a peça de acordo com seu tipo: I, J, O, L, S, T, Z (TetrisObjectType)*/
		switch (type) {
			case I:
				rotatePos0(keyPos);
				break;

			case J:
				try {

					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();

					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY() + 1);
				}

				break;

			case O:
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				break;

			case L:
				try {

					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();

					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY() - 1);
				}
				break;

			case S:
				try {
					if (!gameFrame.objLeftBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objLeftBoundsIsOccuped(this) && !gameFrame.objRightBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() + 1, keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[3].setPosition(keyPos.getX(), keyPos.getY() + 1);
				}
				break;

			case T:
				try {

					if (!gameFrame.objRightBoundsIsOccuped(this)) throw new Exception();

					if (!gameFrame.objRightBoundsIsOccuped(this) && gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

					if (gameFrame.objRightBoundsIsOccuped(this) && !gameFrame.objLeftBoundsIsOccuped(this)) {
						this.shiftLeft();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() + 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case Z:
				try {
					if (!gameFrame.objLeftBoundsIsOccuped(this)) throw new Exception();
					if (gameFrame.objLeftBoundsIsOccuped(this) && !gameFrame.objRightBoundsIsOccuped(this)) {
						this.shiftRight();
						throw new Exception();
					}

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY() + 1);
				}
				break;
		}
	}

	/**
	 * Rotaciona a peça para a quarta posição
	 */
	void rotatePos3(Position keyPos) {
		/*Monta a peça de acordo com seu tipo: I, J, O, L, S, T, Z (TetrisObjectType)*/
		switch (type) {
			case I:
				rotatePos1(keyPos);
				break;

			case J:
				try {

					if (gameFrame.objUpperBoundsAreOccupied(this)) {
						if (!gameFrame.objLowerBoundsIsOccuped(this)) {
							this.shiftDown();
							throw new Exception();
						}
					} else throw new Exception();

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() + 1, keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}

				break;

			case O:
				pieces[0].setPosition(0, Consts.NUM_COLUMNS/2);
				pieces[1].setPosition(1, Consts.NUM_COLUMNS/2);
				pieces[2].setPosition(0, Consts.NUM_COLUMNS/2 + 1);
				pieces[3].setPosition(1, Consts.NUM_COLUMNS/2 + 1);
				break;

			case L:
				try {

					if (gameFrame.objUpperBoundsAreOccupied(this)) {
						if (!gameFrame.objLowerBoundsIsOccuped(this)) {
							this.shiftDown();
							throw new Exception();
						}
					} else throw new Exception();

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX() - 1, keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case S:
				try {
					if (gameFrame.objUpperBoundsAreOccupied(this)) {
						if (!gameFrame.objLowerBoundsIsOccuped(this)) {
							this.shiftDown();
							throw new Exception();
						}
					} else throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[1].setPosition(keyPos.getX() - 1, keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case T:
				try {

					if (gameFrame.objUpperBoundsAreOccupied(this)) {
						if (!gameFrame.objLowerBoundsIsOccuped(this)) {
							this.shiftDown();
							throw new Exception();
						}
					} else throw new Exception();

				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() - 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() + 1, keyPos.getY());
				}
				break;

			case Z:
				try {
					if (gameFrame.objUpperBoundsAreOccupied(this)) {
						if (!gameFrame.objLowerBoundsIsOccuped(this)) {
							this.shiftDown();
							throw new Exception();
						}
					} else throw new Exception();
				} catch (Exception e) {
					pieces[0].setPosition(keyPos.getX() + 1, keyPos.getY());
					pieces[1].setPosition(keyPos.getX(), keyPos.getY() - 1);
					pieces[3].setPosition(keyPos.getX() - 1, keyPos.getY() - 1);
				}
				break;
		}
	}

	private void shiftLeft() {
		for (int i = 0; i < 4; i++) pieces[i].pos.moveLeft();
	}

	private void shiftRight() {
		for (int i = 0; i < 4; i++) pieces[i].pos.moveRight();
	}

	private void shiftUp() {
		for (int i = 0; i < 4; i++) pieces[i].pos.moveUp();
	}

	private void shiftDown() {
		for (int i = 0; i < 4; i++) pieces[i].pos.moveDown();
	}


}
