package utils;

import control.GameFrame;
import java.awt.Graphics;
import javax.swing.ImageIcon;

/**
 * Projeto de POO 2017
 * 
 * @author Luiz Eduardo
 * Baseado em material do Prof. Jose Fernando Junior
 */
public class Drawing {

    static GameFrame screen;

    public static GameFrame getGameFrame() {
        return screen;
    }

    public static void setGameFrame(GameFrame newScreen) {
        screen = newScreen;
    }

    public static void draw(Graphics g, ImageIcon imageIcon, double y, double x) {
        //System.out.println("y="+(y * Consts.CELL_SIZE)+", x="+(x * Consts.CELL_SIZE));
        imageIcon.paintIcon(screen, g, (int)Math.round(y * Consts.CELL_SIZE),(int)Math.round(x * Consts.CELL_SIZE));
    }
}
