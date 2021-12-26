package utils;

import java.awt.*;
import java.io.File;

/**
 * Projeto de POO 2017
 * 
 * @author Luiz Eduardo
 * Baseado em material do Prof. Jose Fernando Junior
 */
public class Consts {
    public static final int CELL_SIZE = 30;
    public static final int NUM_COLUMNS = 10;
    public static final int NUM_LINES = 18;
    public static final int BASE_POINT_INC = 50;

    public static final int DELAY_SCREEN_UPDATE = 20;

    public static final String GAME_NAME = "TetriZika";
    public static final String IMG_PATH = File.separator + "imgs" + File.separator;
    public static final String MENU_BG_PATH = IMG_PATH + "bgMenu1.png";
    public static final String BG_NAME = "bricks.png";

    public static final Font SCORE_FONT = new Font("Arial", Font.BOLD, 14);

}
