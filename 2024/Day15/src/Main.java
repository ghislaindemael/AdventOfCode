import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void main(String[] args) {

        String filename = "resources/input.txt";

        StringBuilder moves = parseMovesFromFile(filename);

        Grid simpleGrid = parseGridFromFile(filename);
        LargeGrid largeGrid = new LargeGrid();
        largeGrid.scaleFromOriginalGrid(simpleGrid);

        for (int i = 0; i < moves.length(); i++) {
            char move = moves.charAt(i);
            simpleGrid.moveRobot(move);
            largeGrid.moveRobot(move);
        }

        simpleGrid.printGrid();
        System.out.println("Sum of GPS coordinates of boxes: " + simpleGrid.getSumOfGPSCoordinates());
        largeGrid.printGrid();
        System.out.println("Sum of GPS coordinates of boxes: " + largeGrid.getSumOfGPSCoordinates());
    }

    private static Grid parseGridFromFile(String filePath) {
        Grid grid = new Grid();

        try (InputStream inputStream = Main.class.getClassLoader().getResourceAsStream(filePath);
             BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {

            List<String> lines = br.lines().toList();
            int emptyLineIndex = lines.indexOf("");

            for (int i = 0; i < emptyLineIndex; i++) {
                String line = lines.get(i);
                ArrayList<Cell> row = new ArrayList<>();
                int index = 0;
                for (char c : line.toCharArray()) {
                    switch (c) {
                        case '#' -> row.add(Cell.WALL);
                        case 'O' -> row.add(Cell.BOX);
                        case '@' -> {
                            row.add(Cell.EMPTY);
                            grid.setRobotPosition(grid.getMap().size(), index);
                        }
                        default -> row.add(Cell.EMPTY);
                    }
                    index++;
                }
                grid.addRow(row);
            }

        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + filePath, e);
        }

        return grid;
    }

    private static StringBuilder parseMovesFromFile(String filePath) {
        StringBuilder moves = new StringBuilder();

        try (InputStream inputStream = Main.class.getClassLoader().getResourceAsStream(filePath);
             BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {

            List<String> lines = br.lines().toList();

            int emptyLineIndex = lines.indexOf("");

            for (int i = emptyLineIndex + 1; i < lines.size(); i++) {
                String line = lines.get(i);
                moves.append(line);
            }

        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + filePath, e);
        }

        return moves;
    }


}
