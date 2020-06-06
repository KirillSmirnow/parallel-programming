package dijkstra.graph

import dijkstra.graph.impl.SequentialGraph

object GraphFactory {

    fun create(graphStructure: GraphStructure): List<Graph> = listOf(
            SequentialGraph(graphStructure)
    )
}
