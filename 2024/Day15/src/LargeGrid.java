import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class LargeGrid extends Grid {

    public LargeGrid() {
        super();
    }

    public void addScaledRow(ArrayList<Cell> originalRow) {
        ArrayList<Cell> scaledRow = new ArrayList<>();
        for (Cell cell : originalRow) {
            switch (cell) {
                case WALL -> {
                    scaledRow.add(Cell.WALL);
                    scaledRow.add(Cell.WALL);
                }
                case BOX -> {
                    scaledRow.add(Cell.LEFT_BOX);
                    scaledRow.add(Cell.RIGHT_BOX);
                }
                default -> {
                    scaledRow.add(Cell.EMPTY);
                    scaledRow.add(Cell.EMPTY);
                }
            }
        }
        this.addRow(scaledRow);
    }

    public void scaleFromOriginalGrid(Grid originalGrid) {
        for (ArrayList<Cell> originalRow : originalGrid.getMap()) {
            addScaledRow(originalRow);
        }

        Robot originalRobot = originalGrid.getRobot();
        this.setRobotPosition(originalRobot.posX, originalRobot.posY * 2);
    }

    @Override
    public void moveRobot(char direction) {
        int newX = getRobot().posX;
        int newY = getRobot().posY;

        switch (direction) {
            case '^' -> newX--;
            case 'v' -> newX++;
            case '<' -> newY--;
            case '>' -> newY++;
        }

        if (this.getMap().get(newX).get(newY) == Cell.EMPTY) {
            getRobot().posX = newX;
            getRobot().posY = newY;
        } else if (isCellALargeBox(newX, newY)) {
            pushBoxesIfPossible(getRobot().posX, getRobot().posY, direction);
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

        if (dx == 0) {
            pushBoxesHorizontally(posX, posY, dy);
        } else {
            pushBoxesVertically(posX, posY, dx);
        }
    }

    private void pushBoxesHorizontally(int posX, int posY, int dy) {
        int currentY = posY;

        while (isCellALargeBox(posX, currentY + dy)) {
            currentY += dy;
        }

        if (this.getMap().get(posX).get(currentY + dy) == Cell.EMPTY) {
            for (int i = currentY; i != posY - dy; i -= dy) {
                this.getMap().get(posX).set(i + dy, this.getMap().get(posX).get(i));
            }

            this.getMap().get(posX).set(posY, Cell.EMPTY);

            getRobot().posY += dy;
        }
    }

    private boolean isCellALargeBox(int x, int y) {
        return getMap().get(x).get(y) == Cell.LEFT_BOX ||
                getMap().get(x).get(y) == Cell.RIGHT_BOX;
    }

    private void pushBoxesVertically(int posX, int posY, int dx) {

        Cell nextCell = getMap().get(posX + dx).get(posY);
        int[] baseCell= {-1, -1};
        if(nextCell == Cell.LEFT_BOX) {
            baseCell = new int[]{posX + dx, posY};
        } else if (nextCell == Cell.RIGHT_BOX) {
            baseCell = new int[]{posX + dx, posY - 1};
        }

        if (baseCell[0] + baseCell[1] > 0) {
            if(!canBoxBePushed(baseCell[0], baseCell[1], dx)){
                return;
            }
        }

        ArrayList<int[]> boxesToPush = new ArrayList<>();
        boxesToPush.add(new int[]{baseCell[0], baseCell[1]});
        collectBoxesToPush(baseCell[0], baseCell[1], dx, boxesToPush);
        filterDuplicateBoxes(boxesToPush);

        boxesToPush.sort((a, b) -> {
            int distA = Math.abs(a[0] - posX);
            int distB = Math.abs(b[0] - posX);
            return Integer.compare(distB, distA);
        });

        for (int[] box : boxesToPush) {
            this.getMap().get(box[0] + dx).set(box[1], Cell.LEFT_BOX);
            this.getMap().get(box[0] + dx).set(box[1] + 1, Cell.RIGHT_BOX);
            this.getMap().get(box[0]).set(box[1], Cell.EMPTY);
            this.getMap().get(box[0]).set(box[1] + 1, Cell.EMPTY);
        }

        getRobot().posX += dx;
    }

    public void filterDuplicateBoxes(ArrayList<int[]> boxesToPush) {
        Set<String> uniqueBoxes = new HashSet<>();

        ArrayList<int[]> filteredBoxes = new ArrayList<>();

        for (int[] box : boxesToPush) {
            if (uniqueBoxes.add(box[0] + "," + box[1])) {
                filteredBoxes.add(box);
            } else {
                //System.out.println("Duplicate found at: (" + box[0] + ", " + box[1] + ")");
            }
        }

        boxesToPush.clear();
        boxesToPush.addAll(filteredBoxes);
    }

    private void collectBoxesToPush(int x, int y, int dx, ArrayList<int[]> boxesToPush) {
        int nextX = x + dx;
        if(this.getMap().get(nextX).get(y) == Cell.LEFT_BOX) {
            boxesToPush.add(new int[]{nextX, y});
            collectBoxesToPush(nextX, y, dx, boxesToPush);
        } else {
            if (this.getMap().get(nextX).get(y - 1) == Cell.LEFT_BOX) {
                boxesToPush.add(new int[]{nextX, y - 1});
                collectBoxesToPush(nextX, y - 1, dx, boxesToPush);
            }
            if(this.getMap().get(nextX).get(y + 1) == Cell.LEFT_BOX) {
                boxesToPush.add(new int[]{nextX, y + 1});
                collectBoxesToPush(nextX, y + 1, dx, boxesToPush);
            }
        }

    }

    private boolean canBoxBePushed(int x, int y, int dx) {
        int nextX = x + dx;
        int nextAdjacentY = - 1;
        if(this.getMap().get(x).get(y) == Cell.LEFT_BOX) {
            nextAdjacentY = y + 1;
        } else if (this.getMap().get(x).get(y) == Cell.RIGHT_BOX) {
            nextAdjacentY = y - 1;
        }

        if(nextAdjacentY == -1){
            return true;
        }

        if(this.getMap().get(nextX).get(y) == Cell.EMPTY && this.getMap().get(nextX).get(nextAdjacentY) == Cell.EMPTY) {
            return true;
        }
        if(this.getMap().get(nextX).get(y) == Cell.WALL || this.getMap().get(nextX).get(nextAdjacentY) == Cell.WALL) {
            return false;
        }
        return canBoxBePushed(nextX, y, dx) && canBoxBePushed(nextX, nextAdjacentY, dx);
    }

}
