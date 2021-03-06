system_sizes = tuple(i * 100 for i in range(1, 21))
try_processes = (1, 2, 4)

for size in system_sizes:
    for total_processes in try_processes:
        part_size = size // total_processes
        actual_sizes = []

        offset = 0
        for process in range(total_processes):
            current_part_size = size // total_processes + (size % total_processes > process)

            actual_sizes.append(current_part_size)
            print("size=%s, processes=%s, process=%s, offset=%s, size=%s" % (
                size, total_processes, process, offset, current_part_size
            ))

            offset += current_part_size

        if all(size == part_size for size in actual_sizes):
            print(">> OK. All parts are of equal size")
        else:
            print(">> !!! Actual parts sizes: " + str(actual_sizes))
            exit(-1)
