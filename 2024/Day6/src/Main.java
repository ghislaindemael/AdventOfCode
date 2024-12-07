import java.io.*;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {

        Guard guard = new Guard(0, 0);
        ArrayList<ArrayList<Cell>> map = new ArrayList<>();

        try (InputStream inputStream = Main.class.getClassLoader().getResourceAsStream("resources/input.txt");
             BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {
            String line;
            while ((line = br.readLine()) != null) {
                ArrayList<Cell> row = new ArrayList<>();
                int index = 0;
                for (char c : line.toCharArray()) {
                    switch (c) {
                        case '.':
                            row.add(new Cell(false));
                            break;
                        case '#':
                            row.add(new Cell(true));
                            break;
                        case '^':
                            Cell guardCell = new Cell(false);
                            guardCell.isVisited = true;
                            row.add(guardCell);
                            guard.posX = map.size();
                            guard.posY = index;
                            guard.initialPosX = guard.posX;
                            guard.initialPosY = guard.posY;
                            break;
                    }
                    index++;
                }
                map.add(row);
            }

            while (!guard.isNextCellOutside(map.size())) {
                Cell nextCell = map.get(guard.getNextX()).get(guard.getNextY());
                if (nextCell.isObstacle) {
                    guard.rotate();
                } else {
                    guard.move();
                    nextCell.isVisited = true;
                }
            }

            int visitedCells = 0;
            for (ArrayList<Cell> row : map) {
                for (Cell cell : row) {
                    if (cell.isVisited) {
                        visitedCells++;
                    }
                }
            }
            System.out.println("Visited cells: " + visitedCells);

            //Part two
            int guardStuckCases = 0;

            for (ArrayList<Cell> row : map) {
                for (Cell cell : row) {
                    if (!cell.isObstacle) {
                        cell.isObstacle = true;
                        guard.resetPosition();

                        while (true) {
                            if (guard.isNextCellOutside(map.size())) {
                                break;
                            }
                            if (guard.alreadyInState()) {
                                guardStuckCases++;
                                break;
                            }
                            Cell nextCell = map.get(guard.getNextX()).get(guard.getNextY());
                            if (nextCell.isObstacle) {
                                guard.rotate();
                            } else {
                                guard.move();
                            }
                        }
                        cell.isObstacle = false;
                    }
                }

            }

            System.out.println("New obstacle positions that blocks guard: " + guardStuckCases);

        } catch (IOException | NullPointerException e) {
            System.err.println("Error reading the file: " + e.getMessage());
        }
    }
}