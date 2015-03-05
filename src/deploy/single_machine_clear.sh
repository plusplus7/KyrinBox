#!/bin/sh
echo "* Close services..."
pids=`ps -ef | grep kyrin_master | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done

pids=`ps -ef | grep kyrin_slavenode | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done

echo "* Setup database..."
rm -rf /tmp/kyrinuserdata_db*
rm -rf /tmp/kyrinoplog_db*
