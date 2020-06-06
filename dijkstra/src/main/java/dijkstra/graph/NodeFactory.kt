package dijkstra.graph

object NodeFactory {

    private var nextId = 0

    fun create(): Node = NodeImpl(nextId++)
}
