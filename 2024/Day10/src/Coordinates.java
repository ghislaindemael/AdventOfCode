import java.util.Objects;

public class Coordinates {

    private final int first;
    private final int second;

    public Coordinates(int first, int second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Coordinates keyPair = (Coordinates) o;
        return first == keyPair.first && second == keyPair.second;
    }

    @Override
    public int hashCode() {
        return Objects.hash(first, second);
    }

    @Override
    public String toString() {
        return "(" + first + ", " + second + ")";
    }


}
