package dijkstra.graph

import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Test

class GraphStructureTest {

    @Test
    fun test() {
        val fullAdjacencyMatrix = arrayOf(
                arrayOf(null, 4, 1, 10, null),
                arrayOf(4, null, null, 2, 3),
                arrayOf(1, null, null, 4, null),
                arrayOf(10, 2, 4, null, 5),
                arrayOf(null, 3, null, 5, null)
        )

        val graphStructure = GraphStructure(arrayOf(
                intArrayOf(4, 1, 10, -1),
                intArrayOf(-1, 2, 3),
                intArrayOf(4, -1),
                intArrayOf(5)
        ))

        assertEquals(5, graphStructure.getVerticesNumber())

        for (row in 0..4) {
            for (column in 0..4) {
                val edge = graphStructure.getEdge(row, column)
                assertEquals(fullAdjacencyMatrix[row][column], edge)
            }
        }
    }
}
