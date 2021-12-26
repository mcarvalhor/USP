package elements;

import utils.Drawing;
import java.awt.Graphics;
import java.io.Serializable;

/**
 * Projeto de POO 2017
 * 
 * @author Luiz Eduardo
 * Baseado em material do Prof. Jose Fernando Junior
 */
public class Lolo extends Element implements Serializable{
    
    public Lolo(String imageName) {
        super(imageName);
    }
    
    @Override
    public void autoDraw(Graphics g){
        Drawing.draw(g, this.imageIcon, pos.getY(), pos.getX());
    }

    public void backToLastPosition(){
        this.pos.comeBack();
    }
}
