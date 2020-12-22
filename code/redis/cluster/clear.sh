#!/bin/bash

cd redis-cluster1/
rm *.rdb
rm *.log
rm nodes.conf

cd ../redis-cluster2/
rm *.rdb
rm *.log
rm nodes.conf

cd ../redis-cluster3/
rm *.rdb
rm *.log
rm nodes.conf

cd ../redis-cluster4/
rm *.rdb
rm *.log
rm nodes.conf

cd ../redis-cluster5/
rm *.rdb
rm *.log
rm nodes.conf

cd ../redis-cluster6/
rm *.rdb
rm *.log
rm nodes.conf
