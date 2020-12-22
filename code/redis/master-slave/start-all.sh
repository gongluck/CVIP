#!/bin/bash

cd redis-master/
./redis-server redis.conf &

cd ../redis-slave1/
./redis-server redis.conf &

cd ../redis-slave2/
./redis-server redis.conf &

cd ..
