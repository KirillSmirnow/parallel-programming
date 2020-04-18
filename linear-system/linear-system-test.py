import csv
import time
import random
import subprocess
import matplotlib.pyplot as plot

EXECUTABLE = "./linear-system-impl/cmake-build-release/linear-system"

processes_counts = (1, 2, 4)
array_lengths = (10_000, 100_000, 500_000, 1_000_000, 3_000_000, 6_000_000, 10_000_000)

benchmarks = {
    processes_count: {length: -1 for length in array_lengths}
    for processes_count in processes_counts
}


def benchmark():
    for array_length in array_lengths:
        benchmark_array(array_length)


def benchmark_array(array_length):
    print(f"Benchmarking array of length {array_length}")
    with open("quicksort.input", "w") as file:
        array = tuple(str(random.randint(-1_000_000, 1_000_000)) for _ in range(array_length))
        file.writelines((f"{array_length}\n", " ".join(array)))
    for processes_count in processes_counts:
        duration = benchmark_array_with_processes_count(processes_count)
        benchmarks[processes_count][array_length] = duration
        verify_array_sorted()


def verify_array_sorted():
    with open("quicksort.output") as file:
        file.readline()
        array = tuple(map(int, file.readline().strip().split(" ")))
        for index in range(len(array) - 1):
            if array[index] > array[index + 1]:
                raise Exception("Array was not sorted")


def benchmark_array_with_processes_count(processes_count) -> float:
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
        plot.plot(array_lengths, accelerations, marker='o')
    plot.legend(processes_counts)
    plot.show()


def save_results():
    records = []
    for process_count in benchmarks:
        for array_length in benchmarks[process_count]:
            records.append((array_length, process_count, benchmarks[process_count][array_length]))
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
