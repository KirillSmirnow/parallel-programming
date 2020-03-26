# Parallel Quicksort с помощью MPI

[Send and Receive](https://mpitutorial.com/tutorials/mpi-send-and-receive/)

## Запуск
Установить реализацию MPI

`apt install mpich`

Запуск на одной машине

`mpiexec -n <totalProcesses> ./quicksort`

Запуск на кластере. Узлы должны быть доступны друг другу по ssh
и иметь одинаковую файловую структуру

`mpiexec --host <host1,host2...> -n <totalProcesses> ./quicksort`
