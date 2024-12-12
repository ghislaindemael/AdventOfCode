import java.io.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

public class Main {
    public static void main(String[] args) {

        ArrayList<ArrayList<Integer>> map = new ArrayList<>();
        try (InputStream inputStream = Main.class.getClassLoader().getResourceAsStream("resources/input.txt");
             BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {
            String line;
            while ((line = br.readLine()) != null) {
                ArrayList<Integer> row = new ArrayList<>();
                for (char c : line.toCharArray()) {
                    row.add(c - 48);
                }
                map.add(row);
            }

            int sumOfUniqueTrailheadScores = 0;
            int sumOfAllTrailheadScores = 0;

            for (int i = 0; i < map.size(); i++) {
                for (int j = 0; j < map.get(i).size(); j++) {
                    if (map.get(i).get(j) == 0) {
                        Set<Coordinates> uniqueReachablePeaks = new HashSet<>();
                        exploreAllDirections(map, uniqueReachablePeaks, i, j, 1);
                        sumOfUniqueTrailheadScores += uniqueReachablePeaks.size();

                        ArrayList<Coordinates> reachablePeaks = new ArrayList<>();
                        exploreAllDirections(map, reachablePeaks, i, j, 1);
                        sumOfAllTrailheadScores += reachablePeaks.size();
                    }
                }
            }

            System.out.println("Sum of unique trailhead scores: " + sumOfUniqueTrailheadScores);
            System.out.println("Sum of all trailhead scores: " + sumOfAllTrailheadScores);

        } catch (IOException | NullPointerException e) {
            System.err.println("Error reading the file: " + e.getMessage());
        }
    }

    static void exploreTrailhead(ArrayList<ArrayList<Integer>> map, Collection<Coordinates> reachablePeaks, int i, int j, int nextNum) {
        if (i < 0 || j < 0 || i >= map.size() || j >= map.get(i).size() || map.get(i).get(j) != nextNum) return;
        if (nextNum == 9) {
            if (map.get(i).get(j) == 9) {
                reachablePeaks.add(new Coordinates(i, j));
            }
            return;
        }
        exploreAllDirections(map, reachablePeaks, i, j, nextNum + 1);
    }

    static <T extends Collection<Coordinates>> void exploreAllDirections(ArrayList<ArrayList<Integer>> map, T collection, int i, int j, int nextNum) {
        exploreTrailhead(map, collection, i, j + 1, nextNum);
        exploreTrailhead(map, collection, i, j - 1, nextNum);
        exploreTrailhead(map, collection, i + 1, j, nextNum);
        exploreTrailhead(map, collection, i - 1, j, nextNum);
    }

}