#!/bin/bash

# Build app
cd nasa-logs
mvn clean package
cd ..

# Transfer app's jar into container
cp ./nasa-logs/target/*jar-with-dependencies.jar app.jar
docker cp app.jar "spark_master:/opt/bitnami/spark"
rm -f app.jar

# Submit app
docker exec "spark_master" ./bin/spark-submit \
    --master=spark://spark-master:7077 \
    app.jar
