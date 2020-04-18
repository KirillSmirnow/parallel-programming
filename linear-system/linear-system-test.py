import csv
import time
import numpy
import subprocess
import matplotlib.pyplot as plot

EXECUTABLE = "./linear-system-impl/cmake-build-release/linear_system"

processes_counts = (1, 2, 4)
system_sizes = (300, 800, 2000)
precision = 0.1

benchmarks = {
    processes_count: {size: -1 for size in system_sizes}
    for processes_count in processes_counts
}


def benchmark():
    for size in system_sizes:
        benchmark_system(size)


def benchmark_system(system_size):
    print(f"Benchmarking system of size {system_size}")
    parameters = numpy.random.rand(system_size + 2, system_size)
    b = parameters[1]
    A = parameters[2:]
    with open("linear-system.input", "w") as file:
        file.write(f"{system_size} {precision}\n")
        for row in parameters:
            file.write(" ".join(str(e) for e in row) + "\n")
    for processes_count in processes_counts:
        duration = benchmark_system_with_processes_count(processes_count)
        benchmarks[processes_count][system_size] = duration
        verify_system_solved(A, b)


def verify_system_solved(A, b):
    x = numpy.fromfile("linear-system.output", sep=" ")
    correct = all(abs(numpy.dot(A, x) - b) < precision)
    if not correct:
        raise Exception("Solution incorrect")


def benchmark_system_with_processes_count(processes_count) -> float:
    print(f"Benchmarking {processes_count} processes")
    iterations = 100
    start = time.time()
    for i in range(iterations):
        exit_code = subprocess.call(["mpiexec", "-n", str(processes_count), EXECUTABLE])
        assert exit_code == 0
    finish = time.time()
    duration = (finish - start) / iterations
    print("Duration: %.2f seconds" % duration)
    return duration


def plot_results():
    serial_durations = tuple(benchmarks[1].values())
    for processes_count in benchmarks:
        durations = tuple(benchmarks[processes_count].values())
        accelerations = tuple(sd / d for sd, d in zip(serial_durations, durations))
        plot.plot(system_sizes, accelerations, marker='o')
    plot.legend(processes_counts)
    plot.show()


def save_results():
    records = []
    for process_count in benchmarks:
        for system_size in benchmarks[process_count]:
            records.append((system_size, process_count, benchmarks[process_count][system_size]))
    records.sort(key=lambda record: record[0])
    with open("results.csv", "w") as file:
        writer = csv.writer(file)
        writer.writerow(["length", "processes", "time"])
        for record in records:
            writer.writerow(record)
    print(records)


if __name__ == '__main__':
    benchmark()
    plot_results()
    save_results()
