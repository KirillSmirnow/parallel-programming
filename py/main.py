import time
import subprocess
import matplotlib.pyplot as plot

EXECUTABLE = "../cpp/cmake-build-debug/open_mp"

calculation_modes = (
    0,  # serial
    1,  # parallel I
)

matrix_sizes = (
    (1, 1),
    (10, 10),
    (100, 100),
    (200, 200),
    (400, 400),
    (600, 600),
    (800, 800),
    (1000, 1000),
)

benchmarks = {
    mode: {size: -1 for size in matrix_sizes}
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


if __name__ == '__main__':
    benchmark()
    print("Results", benchmarks)
