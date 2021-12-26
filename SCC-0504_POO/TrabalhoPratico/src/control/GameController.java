package control;

import elements.Boundaries;
import elements.Element;
import elements.Lolo;
import elements.Square;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyCombination;

import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;

/**
 * Projeto de POO 2017
 * 
 * @author Luiz Eduardo
 * Baseado em material do Prof. Jose Fernando Junior
 */
public class GameController implements KeyListener {
    private GameFrame game;

    /**
     * Inicializa o controlador a partir de um GameFrame inicializado
     * @param game
     */
    GameController(GameFrame game) {
        this.game = game;
    }

    public void drawAllElements(ArrayList<Element> elemArray, Graphics g){
        for(int i=0; i<elemArray.size(); i++){
            elemArray.get(i).autoDraw(g);
        }
    }
    public void processAllElements(ArrayList<Element> e){
        if(e.isEmpty())
            return;
        
//        Lolo lLolo = (Lolo)e.get(0);
//        if (!isValidPosition(e, lLolo)) {
//            lLolo.backToLastPosition();
//
//            return;
//        }
//
//        Element eTemp;
//        for(int i = 1; i < e.size(); i++){
//            eTemp = e.get(i);
//            if(lLolo.overlap(eTemp))
//                if(eTemp.isMortal())
//                    e.remove(eTemp);
//        }
    }
    public boolean isValidPosition(ArrayList<Element> elemArray, Element elem){
        Element elemAux;
        for(int i = 1; i < elemArray.size(); i++){
            elemAux = elemArray.get(i);            
            if(!elemAux.isTransposable())
                if(elemAux.overlap(elem))
                    return false;
        }        
        return true;
    }


    /**
     * Método "ouvinte" das teclas que são pressionadas no teclado
     * @param e Evento de tecla
     */
    public void keyPressed(KeyEvent e) {
        /* Para cada um dos 4 "Squares" que compõem a peça, faça o movimento
         * requisitado (direita, esquerda ou pra baixo)
         * SE e SOMENTE SE, não estiver nas extremidades ou, houver algo impedindo */

        if (game.currentTetrisObject.isActive()) {

            /*BAIXO*/
            if (e.getKeyCode() == KeyEvent.VK_DOWN) {

                /*Se não está tocando em nada, desce*/
                if (!game.objLowerBoundsIsOccuped(game.currentTetrisObject)) {
                    for (Square s : game.currentTetrisObject.pieces)
                        s.moveDown(); // Desce
                } else {
                    game.setDeactivatedPiece();
                }
            }

            /*ESQUERDA*/
            else if (e.getKeyCode() == KeyEvent.VK_LEFT) {

                /*Verifica se não está tocando nada a esquerda*/
                if (!game.objLeftBoundsIsOccuped(game.currentTetrisObject)) {
                    for (Square s : game.currentTetrisObject.pieces)
                        s.moveLeft(); // Move para esquerda
                }

            }

            /*DIREITA*/
            else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {

                /*Verifica se já não está tocando nada a direita*/
                if (!game.objRightBoundsIsOccuped(game.currentTetrisObject)) {
                    for (Square s : game.currentTetrisObject.pieces)
                        s.moveRight(); // Move para direita
                }

            }

            /*ROTAÇÃO*/
            else if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                game.currentTetrisObject.rotate();
            }

            /*SALVAMENTO*/
            else if (e.getKeyCode() == KeyEvent.VK_S && e.isControlDown()) {
                game.saveGame();
            }
        }

        //repaint(); /*invoca o paint imediatamente, sem aguardar o refresh*/
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }
}
