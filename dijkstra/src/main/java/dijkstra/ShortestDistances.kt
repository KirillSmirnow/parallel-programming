package dijkstra

import dijkstra.graph.Graph
import dijkstra.graph.Node

class ShortestDistances(private val graph: Graph, private val start: Node) {

    fun find(): List<Int?> {
        graph.setStart(start)
        while (true) {
            val node = graph.getClosestUnvisitedNode() ?: return graph.getDistances()
            graph.updateNeighbors(node) { neighbor, edge ->
                val distanceFromNode = node.getDistance()!! + edge
                if (neighbor.getDistance() == null || distanceFromNode < neighbor.getDistance()!!) {
                    neighbor.setDistance(distanceFromNode)
                }
            }
        }
    }
}
