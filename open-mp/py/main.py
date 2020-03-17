import time
import subprocess
import matplotlib.pyplot as plot

EXECUTABLE = "../cpp/cmake-build-debug/open_mp"

calculation_modes = {
    0: "Serial",
    1: "Parallel For",
}

matrix_sizes = (
    (1, 1),
    (10, 10),
    (100, 100),
    (1, 25000),
    (25000, 1),
    (200, 200),
    (400, 400),
    (600, 600),
    (800, 800),
    (1000, 1000),
)

benchmarks = {
    mode: {(size[0] * size[1]): -1 for size in matrix_sizes}
    for mode in calculation_modes
}


def benchmark():
    for matrix_size in matrix_sizes:
        benchmark_matrix(matrix_size[0], matrix_size[1])


def benchmark_matrix(height, width):
    print("Benchmarking matrix of size %d×%d" % (height, width))
    size = height * width
    subprocess.call([EXECUTABLE, "1", str(height), str(width)])
    for mode in calculation_modes:
        duration = benchmark_matrix_mode(mode)
        benchmarks[mode][size] = duration


def benchmark_matrix_mode(mode) -> float:
    print("Benchmarking mode %d" % mode)
    iterations = 100
    start = time.time()
    for i in range(iterations):
        subprocess.call([EXECUTABLE, "2", str(mode)])
    finish = time.time()
    duration = (finish - start) / 100
    print("Duration: %.2f seconds" % duration)
    return duration


def plot_results():
    serial_durations = tuple(benchmarks[0].values())  # 0 - serial mode
    for mode in benchmarks:
        sizes = tuple(benchmarks[mode].keys())
        durations = tuple(benchmarks[mode].values())
        accelerations = tuple(sd / d for sd, d in zip(serial_durations, durations))
        plot.plot(sizes, accelerations, marker='o')
    plot.legend(calculation_modes.values())
    plot.show()


if __name__ == '__main__':
    benchmark()
    print("Results", benchmarks)
    plot_results()
