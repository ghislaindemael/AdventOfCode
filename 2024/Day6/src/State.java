
public class State {
    public int posX;
    public int posY;
    public int direction;
    State(int x, int y, int dir) {
        posX = x;
        posY = y;
        direction = dir;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        State state = (State) obj;
        return posX == state.posX && posY == state.posY && direction == state.direction;
    }

}
