#!/bin/bash

cd redis-cluster1/
./redis-server redis.conf &

cd ../redis-cluster2/
./redis-server redis.conf &

cd ../redis-cluster3/
./redis-server redis.conf &

cd ../redis-cluster4/
./redis-server redis.conf &

cd ../redis-cluster5/
./redis-server redis.conf &

cd ../redis-cluster6/
./redis-server redis.conf &

cd ..
