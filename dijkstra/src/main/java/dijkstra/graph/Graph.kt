package dijkstra.graph

interface Graph {

    fun setStart(node: Node)

    fun getClosestUnvisitedNode(): Node?

    fun updateNeighbors(node: Node, update: (neighbor: Node, edge: Int) -> Unit)

    fun getDistances(): List<Int?>
}
