package dijkstra.graph.impl

import dijkstra.graph.Graph
import dijkstra.graph.GraphStructure
import dijkstra.graph.Node
import dijkstra.graph.NodeFactory

class SequentialGraph(private val graphStructure: GraphStructure) : Graph {

    private lateinit var nodes: List<Node>

    override fun getDistances(): List<Int?> = nodes.map { it.getDistance() }

    override fun setStart(node: Node) {
        val verticesNumber = graphStructure.getVerticesNumber()
        nodes = (0 until verticesNumber).map { NodeFactory.get(it) }
        nodes[node.getId()].setDistance(0)
    }

    override fun getClosestUnvisitedNode(): Node? = nodes
            .filterNot { it.getVisited() }
            .filter { it.getDistance() != null }
            .minBy { it.getDistance()!! }

    override fun updateNeighbors(node: Node, update: (neighbor: Node, edge: Int) -> Unit) {
        val verticesNumber = graphStructure.getVerticesNumber()
        for (neighborId in 0 until verticesNumber) {
            val edge = graphStructure.getEdge(node.getId(), neighborId) ?: continue
            val neighbor = nodes[neighborId]
            update(neighbor, edge)
        }
        node.setVisited()
    }
}
