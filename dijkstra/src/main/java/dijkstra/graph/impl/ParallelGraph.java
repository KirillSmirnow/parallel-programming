package dijkstra.graph.impl;

import dijkstra.graph.GraphStructure;
import dijkstra.graph.Node;
import org.jetbrains.annotations.NotNull;

import java.util.concurrent.ForkJoinPool;

import static java.util.Comparator.comparing;

public class ParallelGraph extends SequentialGraph {

    private final ForkJoinPool forkJoinPool;

    public ParallelGraph(GraphStructure graphStructure, int concurrency) {
        super(graphStructure);
        forkJoinPool = new ForkJoinPool(concurrency);
    }

    @Override
    public Node getClosestUnvisitedNode() {
        try {
            return forkJoinPool.submit(() -> nodes.parallelStream()
                    .filter(node -> !node.getVisited())
                    .filter(node -> node.getDistance() != null)
                    .min(comparing(Node::getDistance))
                    .orElse(null)
            ).get();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @NotNull
    @Override
    public String toString() {
        return String.format("Parallel(%d)", forkJoinPool.getParallelism());
    }
}
