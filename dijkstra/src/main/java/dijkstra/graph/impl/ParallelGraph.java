package dijkstra.graph.impl;

import dijkstra.graph.GraphStructure;
import dijkstra.graph.Node;
import kotlin.Unit;
import kotlin.jvm.functions.Function2;
import org.jetbrains.annotations.NotNull;

import java.util.concurrent.ForkJoinPool;

import static java.util.Comparator.comparing;
import static java.util.stream.IntStream.range;

public class ParallelGraph extends SequentialGraph {

    private final ForkJoinPool forkJoinPool;

    public ParallelGraph(GraphStructure graphStructure, int concurrency) {
        super(graphStructure);
        forkJoinPool = new ForkJoinPool(concurrency);
    }

    @Override
    public Node getClosestUnvisitedNode() {
        return forkJoinPool.submit(() -> nodes.parallelStream()
                .filter(node -> !node.getVisited())
                .filter(node -> node.getDistance() != null)
                .min(comparing(Node::getDistance))
                .orElse(null)
        ).join();
    }

    @Override
    public void updateNeighbors(@NotNull Node node, @NotNull Function2<? super Node, ? super Integer, Unit> update) {
        int verticesNumber = getGraphStructure().getVerticesNumber();
        forkJoinPool.submit(() -> range(0, verticesNumber).parallel().forEach(neighborId -> {
            Integer edge = getGraphStructure().getEdge(node.getId(), neighborId);
            if (edge != null) {
                Node neighbor = nodes.get(neighborId);
                update.invoke(neighbor, edge);
            }
        })).join();
        node.setVisited();
    }

    @NotNull
    @Override
    public String toString() {
        return String.format("Parallel(%d)", forkJoinPool.getParallelism());
    }
}
