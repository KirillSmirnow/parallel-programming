# Алгоритм Дейкстры

Алгоритм Дейкстры реализован в классе `ShortestDistances`. На каждой итерации
алгоритма выбирается ещё не посещённая вершина, ближайшая к стартовой вершине,
`graph.getClosestUnvisitedNode()` и обновляются кратчайшие растояния до всех
вершин, соседствующих с ней, `graph.updateNeighbors(node) {}`.

Для гарантии корректности алгоритма написаны несколько тестов.

Структура графа реализована в виде матрицы смежности (`GraphStructure`). Так как
граф неориентированный, то хранится только половина матрицы смежности.

### SequentialGraph
* `getClosestUnvisitedNode` проходится по всем вершинам в поиске вершины,
удовлетворяющей условиям
* `updateNeighbors` проходится по всем вершинам и если вершина является соседом
данной вершины, то вызывает функцию обновления

### ParallelGraph

Данная реализация расширяет **SequentialGraph** и переопределяет методы
`getClosestUnvisitedNode` и `updateNeighbors`. Эти методы обходят набор вершин
графа, что представимо в виде работы с объектом Stream. Так как количество вершин
велико, то имеет смысл распараллелить их обработку, используя паттерн fork-join.
Паттерн fork-join реализуется при параллельном режиме работы объекта Stream.

[ThreadPool for Parallel Streams](https://www.baeldung.com/java-8-parallel-streams-custom-threadpool)

## Результаты измерений
![duration](res/duration.svg)
![speedup](res/speedup.svg)