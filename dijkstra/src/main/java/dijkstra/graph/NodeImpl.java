package dijkstra.graph;

public class NodeImpl implements Node {

    private final int id;
    private Integer distance;
    private boolean visited;

    public NodeImpl(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public Integer getDistance() {
        return distance;
    }

    public void setDistance(int distance) {
        this.distance = distance;
    }

    public boolean getVisited() {
        return visited;
    }

    public void setVisited() {
        visited = true;
    }
}
