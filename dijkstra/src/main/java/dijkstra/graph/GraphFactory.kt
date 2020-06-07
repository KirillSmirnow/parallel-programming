package dijkstra.graph

import dijkstra.graph.impl.ParallelGraph
import dijkstra.graph.impl.SequentialGraph

object GraphFactory {

    fun create(graphStructure: GraphStructure): List<Graph> = listOf(
            SequentialGraph(graphStructure),
            ParallelGraph(graphStructure, 1),
            ParallelGraph(graphStructure, 2),
            ParallelGraph(graphStructure, 3),
            ParallelGraph(graphStructure, 4)
    )

    fun create(graphStructure: GraphStructure, concurrency: Int): List<Graph> {
        if (concurrency == 1) {
            return listOf(SequentialGraph(graphStructure))
        }
        return listOf(ParallelGraph(graphStructure, concurrency))
    }
}
