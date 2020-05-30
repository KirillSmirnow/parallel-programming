# Обработка логов с помощью Spark + Hadoop

## Подготовка
1. Сконфигурируем развёртывание Spark + Hadoop кластера в docker-compose.yml

1. Положим логи в папку на HDFS с помощью команд `hdfs dfs -mkdir -p /nasa/input` и
`hdfs dfs -put * /nasa/input`. При чтении директории из Спарка содержимое файлов
объединится.
![hdfs](res/hdfs.png)

1. Скрипт deploy-app.sh будет компилировать и запускать приложение

## Программа на Спарке
1. Считаем входную директорию с логами и попытаемся представить каждую строку в
виде объекта класса Request. Сохраним строки, которые не удалось распарсить.
![unparsed-requests](res/unparsed-requests.png)

1. Имея RDD\<Request>, выберем запросы, завершившиеся ошибкой сервера (код 5xx).
Количество таких запросов выведем в логах = 106.
![server-errors](res/server-errors.png)

1. Задание "Подготовить временной ряд количества запросов для всех комбинаций
методов и статусов" можно решить по-разному в зависимости от желаемого
представления результатов.

    1. Представление { (method, status, date) → count },
    класс `MethodStatusDateCounter`
    ![method-status-date-counts](res/method-status-date-counts.png)

    1. Представление { (date, method, status) → count },
    класс `DateMethodStatusCounter`
    ![date-method-status-counts](res/date-method-status-counts.png)

1. Найдём суммарное количество ошибок (коды 4xx и 5xx) по семидневным интервалам
![errors-per-week](res/errors-per-week.png)
