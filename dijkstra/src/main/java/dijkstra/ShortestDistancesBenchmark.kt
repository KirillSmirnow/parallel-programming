package dijkstra

import dijkstra.graph.*
import java.time.Duration
import kotlin.system.measureNanoTime

private val verticesNumbers = listOf(1000, 5000, 10000, 15000, 20000, 25000)
private val concurrencyModes = listOf(1, 2, 4)

fun main() {
    val warmUp = ShortestDistancesBenchmark(2)
    val benchmark = ShortestDistancesBenchmark(10)
    val start = NodeFactory.get(0)
    verticesNumbers.forEach { verticesNumber ->
        println("Vertices number: $verticesNumber")
        val graphStructure = GraphStructure.random(verticesNumber)
        concurrencyModes.forEach { concurrency ->
            GraphFactory.create(graphStructure, concurrency).forEach { graph ->
                warmUp.execute(graph, start)
                val duration = benchmark.execute(graph, start)
                println(" >> $graph => ${duration.toMillis()} ms")
            }
        }
    }
}

class ShortestDistancesBenchmark(private val iterations: Int) {
    fun execute(graph: Graph, start: Node): Duration = Duration.ofNanos((1..iterations).map {
        measureNanoTime { ShortestDistances(graph, start).find() }
    }.average().toLong())
}
