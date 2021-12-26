package elements;

public abstract class GameObject {
    protected static int squaresNumber;
    protected boolean isActive;
    public Square[] pieces;

    GameObject() {
        squaresNumber = 0;
        isActive = false;
        pieces = null;
    }

    public boolean isActive() {
        return isActive;
    }

    public Boundaries getObjectBoundaries() {
        Boundaries b = new Boundaries();
        b.highestX = pieces[0].pos.getX();
        b.lowestY = pieces[0].pos.getY();
        b.highestY = pieces[0].pos.getY();
        for (int i = 1; i < squaresNumber; i++) {
            if (pieces[i].pos.getX() > b.highestX) b.highestX = pieces[i].pos.getX();
            if (pieces[i].pos.getY() < b.lowestY) b.lowestY = pieces[i].pos.getY();
            if (pieces[i].pos.getY() > b.highestY) b.highestY = pieces[i].pos.getY();
        }

        //System.out.println(b.toString());

        return b;
    }

    public void desactivatePieces() {
        if (isActive()) {
            isActive = false;
            for (int i = 0; i < squaresNumber; i++) pieces[i].desactivate();
        }
    }
}
