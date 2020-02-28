import subprocess

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


def benchmark():
    for matrix_size in matrix_sizes:
        benchmark_matrix(matrix_size[0], matrix_size[1])


def benchmark_matrix(height, width):
    print("Benchmarking matrix of size %d×%d" % (height, width))
    subprocess.call([EXECUTABLE, "1", str(height), str(width)])
    for mode in calculation_modes:
        benchmark_matrix_mode(mode)


def benchmark_matrix_mode(mode):
    print("Benchmarking mode %d" % mode)
    for i in range(100):
        subprocess.call([EXECUTABLE, "2", str(mode)])


if __name__ == '__main__':
    benchmark()
