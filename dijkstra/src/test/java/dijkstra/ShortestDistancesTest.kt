package dijkstra

import dijkstra.graph.GraphFactory
import dijkstra.graph.GraphStructure
import dijkstra.graph.NodeFactory
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Test

class ShortestDistancesTest {

    @Test
    fun `Connected graph`() {
        val graphStructure = GraphStructure(arrayOf(
                intArrayOf(4, 1, 10, -1),
                intArrayOf(-1, 2, 3),
                intArrayOf(4, -1),
                intArrayOf(5)
        ))
        val graph = GraphFactory.create(graphStructure)
        val start = NodeFactory.get(0)
        val shortestDistances = ShortestDistances(graph, start).find()
        assertEquals(listOf(0, 4, 1, 5, 7), shortestDistances)
    }

    @Test
    fun `Disconnected graph`() {
        val graphStructure = GraphStructure(arrayOf(
                intArrayOf(1, 2, -1, -1),
                intArrayOf(4, -1, -1),
                intArrayOf(-1, -1),
                intArrayOf(5)
        ))
        val graph = GraphFactory.create(graphStructure)
        val start = NodeFactory.get(1)
        val shortestDistances = ShortestDistances(graph, start).find()
        assertEquals(listOf(1, 0, 3, null, null), shortestDistances)
    }
}
