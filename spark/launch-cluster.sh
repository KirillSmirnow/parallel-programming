#!/bin/bash

docker-compose -p spark-hadoop-cluster up -d --scale spark-worker=2
