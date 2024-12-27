import java.util.ArrayList;

public class Grid {
    private final ArrayList<ArrayList<Cell>> map;
    private final Robot robot;

    public Grid() {
        this.map = new ArrayList<>();
        this.robot = new Robot(0, 0);
    }

    public Robot getRobot() {
        return robot;
    }

    public void addRow(ArrayList<Cell> row) {
        map.add(row);
    }

    public ArrayList<ArrayList<Cell>> getMap() {
        return map;
    }

    public void setRobotPosition(int x, int y) {
        robot.posX = x;
        robot.posY = y;
    }

    public void moveRobot(char direction) {
        int newX = robot.posX;
        int newY = robot.posY;

        switch (direction) {
            case '^' -> newX--;
            case 'v' -> newX++;
            case '<' -> newY--;
            case '>' -> newY++;
        }

        if (map.get(newX).get(newY) == Cell.EMPTY) {
            setRobotPosition(newX, newY);
        } else if (map.get(newX).get(newY) == Cell.BOX) {
            pushBoxesIfPossible(robot.posX, robot.posY, direction);
        }
    }

    private void pushBoxesIfPossible(int posX, int posY, char direction) {
        int dx = 0, dy = 0;

        switch (direction) {
            case '^' -> dx = -1;
            case 'v' -> dx = 1;
            case '<' -> dy = -1;
            case '>' -> dy = 1;
        }

        int nextX = posX + dx;
        int nextY = posY + dy;
        ArrayList<int[]> boxPositions = new ArrayList<>();

        while (map.get(nextX).get(nextY) == Cell.BOX) {
            boxPositions.add(new int[]{nextX, nextY});
            nextX += dx;
            nextY += dy;
        }

        if (map.get(nextX).get(nextY) == Cell.EMPTY) {
            for (int i = boxPositions.size() - 1; i >= 0; i--) {
                int[] box = boxPositions.get(i);
                int newBoxX = box[0] + dx;
                int newBoxY = box[1] + dy;

                map.get(newBoxX).set(newBoxY, Cell.BOX);
                map.get(box[0]).set(box[1], Cell.EMPTY);
            }

            robot.posX += dx;
            robot.posY += dy;
        }
    }

    public void printGrid() {
        for (int i = 0; i < map.size(); i++) {
            ArrayList<Cell> row = map.get(i);
            for (int j = 0; j < row.size(); j++) {
                if(i == robot.posX && j == robot.posY) {
                    System.out.print("@");
                    continue;
                }
                switch (map.get(i).get(j)) {
                    case WALL -> System.out.print("#");
                    case BOX -> System.out.print('0');
                    case EMPTY -> System.out.print('.');
                    case LEFT_BOX -> System.out.print('[');
                    case RIGHT_BOX -> System.out.print(']');
                }
            }
            System.out.println();
        }

    }

    public int getSumOfGPSCoordinates() {
        int sum = 0;
        for(int i = 0; i < this.map.size(); i++) {
            for(int j = 0; j < this.map.get(i).size(); j++) {
                Cell cell = map.get(i).get(j);
                if(cell == Cell.BOX || cell == Cell.LEFT_BOX) {
                    sum += 100 * i + j;
                }
            }
        }
        return sum;
    }

}
