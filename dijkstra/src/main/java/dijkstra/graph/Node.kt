package dijkstra.graph

interface Node {

    fun getId(): Int

    fun getDistance(): Int?

    fun setDistance(distance: Int)

    fun getVisited(): Boolean

    fun setVisited()
}
