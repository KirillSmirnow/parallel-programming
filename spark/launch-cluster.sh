#!/bin/bash

docker-compose -p spark -f docker-compose.spark.yml up -d --scale worker=2
docker-compose -p hadoop -f docker-compose.hadoop.yml up -d
