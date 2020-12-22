#!/bin/bash

cd redis-master/
./redis-server redis.conf &
./redis-sentinel sentinel.conf &

cd ../redis-slave1/
./redis-server redis.conf &
./redis-sentinel sentinel.conf &

cd ../redis-slave2/
./redis-server redis.conf &
./redis-sentinel sentinel.conf &

cd ..
