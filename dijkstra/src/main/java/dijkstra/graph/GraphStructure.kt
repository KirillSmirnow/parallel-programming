package dijkstra.graph

import kotlin.math.abs
import kotlin.random.Random
import kotlin.random.nextInt

class GraphStructure(private val adjacency: Array<IntArray>) {

    companion object {
        fun random(verticesNumber: Int): GraphStructure {
            val adjacency = Array(verticesNumber - 1) { row ->
                IntArray(verticesNumber - 1 - row) { randomEdge() }
            }
            return GraphStructure(adjacency)
        }

        private fun randomEdge(): Int {
            val noEdge = Random.nextInt(3) == 0
            return if (noEdge) -1 else Random.nextInt(1..100)
        }
    }

    fun getVerticesNumber(): Int {
        return adjacency.size + 1
    }

    fun getEdge(id1: Int, id2: Int): Int? {
        if (id1 == id2) return null
        val row = if (id1 < id2) id1 else id2
        val column = abs(id1 - id2) - 1
        val edge = adjacency[row][column]
        return if (edge > 0) edge else null
    }
}
