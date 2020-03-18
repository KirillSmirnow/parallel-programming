import subprocess
import matplotlib.pyplot as plot

EXECUTABLE = "../cpp/cmake-build-release/open_mp"

calculation_modes = {
    0: "Serial",
    1: "Parallel For, Static Schedule",
    2: "Parallel Tasks, Static Schedule",
    3: "Parallel Tasks, Dynamic Schedule",
    4: "Parallel Tasks, Guided Schedule",
}

matrix_sizes = (
    (250, 250),
    (500, 500),
    (600, 800),
    (1523, 449),
    (900, 950),
    (1000, 1000),
    (1, 1_000_000),
    (1_000_000, 1),
)

benchmarks = {
    mode: {size: -1 for size in matrix_sizes}
    for mode in calculation_modes
}


def benchmark():
    for matrix_size in matrix_sizes:
        benchmark_matrix(matrix_size[0], matrix_size[1])


def benchmark_matrix(height, width):
    print(f"Benchmarking matrix of size {height}×{width}")
    size = height, width
    for mode in calculation_modes:
        duration = benchmark_matrix_mode(height, width, mode)
        benchmarks[mode][size] = duration


def benchmark_matrix_mode(height, width, mode) -> float:
    print("Benchmarking mode %d" % mode)
    iterations = 100
    iterations_time = 0
    for i in range(iterations):
        process = subprocess.Popen([EXECUTABLE, str(height), str(width), str(mode)], stdout=subprocess.PIPE)
        iterations_time += float(process.stdout.readline())
    duration = iterations_time / iterations
    print("Duration: %.2f seconds" % duration)
    return duration


def plot_results():
    serial_durations = tuple(benchmarks[0].values())  # 0 - serial mode
    for mode in benchmarks:
        durations = tuple(benchmarks[mode].values())
        accelerations = tuple(sd / d for sd, d in zip(serial_durations, durations))
        plot.plot(accelerations, marker='o')
    plot.legend(calculation_modes.values())
    plot.show()


if __name__ == '__main__':
    benchmark()
    print("Results", benchmarks)
    plot_results()
