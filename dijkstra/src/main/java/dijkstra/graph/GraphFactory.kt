package dijkstra.graph

import dijkstra.graph.impl.SequentialGraph

object GraphFactory {

    fun create(graphStructure: GraphStructure, concurrency: Int = 0): List<Graph> = listOf(
            SequentialGraph(graphStructure)
    )
}
