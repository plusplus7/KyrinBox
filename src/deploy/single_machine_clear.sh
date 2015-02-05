#!/bin/sh
echo "* Close services..."
pids=`ps -ef | grep kyrin_master | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done
