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

pids=`ps -ef | grep kyrin_chunk | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done

pids=`ps -ef | grep kyrin_keycenter | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done

pids=`ps -ef | grep redis | grep -v grep | awk '{print $2}'`
for i in ${pids[*]}; do
    kill -9 $i
done

echo "* Setup database..."
rm -rf /kyrinbox/kyrinuserdata_db*
rm -rf /kyrinbox/kyrinoplog_db*
