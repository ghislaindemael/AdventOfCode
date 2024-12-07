import java.util.ArrayList;



public class Guard {

    public int direction = 3;
    public int posX;
    public int posY;
    public int initialPosX;
    public int initialPosY;

    public ArrayList<State> visitedCells = new ArrayList<>();
    private State lastState;

    public Guard(int posX, int posY) {
        this.posX = posX;
        this.posY = posY;
    }

    public int getNextX(){
        return switch (direction) {
            case 1 -> posX + 1;
            case 3 -> posX - 1;
            default -> posX;
        };
    }

    public int getNextY(){
        return switch (direction) {
            case 0 -> posY + 1;
            case 2 -> posY - 1;
            default -> posY;
        };
    }

    public boolean isNextCellOutside(int size){
        return getNextX() >= size|| getNextX() < 0 || getNextY() >= size || getNextY() < 0;
    }

    public void rotate(){
        direction = (direction + 1) % 4;
    }

    public void move(){
        posX = getNextX();
        posY = getNextY();
        saveState();
    }

    public void resetPosition() {
        cleanStateHistory();
        this.posX = initialPosX;
        this.posY = initialPosY;
        this.direction = 3;
    }

    public void cleanStateHistory(){
        visitedCells.clear();
    }

    public void saveState() {
        lastState = new State(this.posX, this.posY, this.direction);
        visitedCells.add(lastState);
    }

    public boolean alreadyInState() {
        for (State state : visitedCells) {
            if (state != lastState &&
                    state.posX == this.posX &&
                    state.posY == this.posY &&
                    state.direction == this.direction) {
                //System.out.println("Guard already in " + posX + ", " + posY + ", " + direction);
                return true;
            }
        }
        return false;
    }
}
