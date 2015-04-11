#!/bin/sh
echo "* Source the config file..."
if [ ! -f ".regression_test_rc" ]; then
    echo ".regression_test_rc needed"
    exit 1
fi
source .regression_test_rc

echo "* Clean up working directory..."
if [ -x "$workspace_dir" ]; then
    rm -rf $workspace_dir
fi
mkdir $workspace_dir
for s_dir in ${master_list[*]}; do
    mkdir $workspace_dir/$s_dir
done
for s_dir in ${slavenode_list[*]}; do
    mkdir $workspace_dir/$s_dir
done
for s_dir in ${chunk_list[*]}; do
    mkdir $workspace_dir/$s_dir
done
for s_dir in ${keycenter_list[*]}; do
    mkdir $workspace_dir/$s_dir
done

echo "* Copy binary to working directory..."
if [ ! -f "$kyrin_master_binary" ]; then
    echo "Cannot find kyrin master binary at $kyrin_master_binary"
    exit 1
fi
for s_dir in ${master_list[*]}; do
    cp $kyrin_master_binary $workspace_dir/$s_dir
done
if [ ! -f "$kyrin_slavenode_binary" ]; then
    echo "Cannot find kyrin slavenode binary at $kyrin_slavenode_binary"
    exit 1
fi
for s_dir in ${slavenode_list[*]}; do
    cp $kyrin_slavenode_binary $workspace_dir/$s_dir
done
if [ ! -f "$kyrin_chunk_binary" ]; then
    echo "Cannot find kyrin chunk binary at $kyrin_chunk_binary"
    exit 1
fi
for s_dir in ${chunk_list[*]}; do
    cp $kyrin_chunk_binary $workspace_dir/$s_dir
done
if [ ! -f "$kyrin_keycenter_binary" ]; then
    echo "Cannot find kyrin keycenter binary at $kyrin_keycenter_binary"
    exit 1
fi
for s_dir in ${keycenter_list[*]}; do
    cp $kyrin_keycenter_binary $workspace_dir/$s_dir
done

echo "* Place configuration file..."
if [ ! -f "$kyrin_config_example" ]; then
    echo "Cannot find kyrin master config example file at $kyrin_config_example"
    exit 1
fi
if [ ! -f "$kyrin_slavenode_config_example" ]; then
    echo "Cannot find kyrin slavenode config example file at $kyrin_slavenode_config_example"
    exit 1
fi
if [ ! -f "$kyrin_keycenter_config_example" ]; then
    echo "Cannot find kyrin keycenter config example file at $kyrin_keycenter_config_example"
    exit 1
fi
echo "* Place master config..."
for (( i=0; i<${#master_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/DEPLOY_TO_PUT_KBID_IN/$bp/g"\
        $kyrin_config_example\
        > $workspace_dir"/"${master_list[$i]}"/"kyrinbox_config.json
done
echo "* Place chunk config..."
for (( i=0; i<${#chunk_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/DEPLOY_TO_PUT_KBID_IN/300$bp/g"\
        -e "s/DEPLOY_TO_PUT_ROLE/Chunk/g"\
        -e "s/7077/"$bp"7077/g"\
        -e "s/7270/"$bp"7270/g"\
        -e "s/7370/"$bp"7370/g"\
        -e "s/7477/"$bp"7477/g"\
        $kyrin_chunk_config_example\
        > $workspace_dir"/"${chunk_list[$i]}"/"kyrinbox_config.json
done
echo "* Place slavenode config..."
for (( i=0; i<${#slavenode_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/DEPLOY_TO_PUT_KBID_IN/2"$bp"/g"\
        -e "s/DEPLOY_TO_PUT_REDISPORT_IN/"$bp"6379/g"\
        -e "s/7000/"$bp"7000/g"\
        $kyrin_slavenode_config_example\
        > $workspace_dir"/"${slavenode_list[$i]}"/"kyrinbox_config.json
done
echo "* Place keycenter config..."
for (( i=0; i<${#keycenter_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/DEPLOY_TO_PUT_REDISPORT_IN/"$bp"6378/g"\
        $kyrin_keycenter_config_example\
        > $workspace_dir"/"${keycenter_list[$i]}"/"kyrinbox_config.json
done
for s_dir in ${master_list[*]}; do
    cp kyrinbox_master_* $workspace_dir/$s_dir
    cp kyrinbox_chunk_* $workspace_dir/$s_dir
    cp kyrinbox_keycenter_* $workspace_dir/$s_dir
done
for s_dir in ${slavenode_list[*]}; do
    cp kyrinbox_master_* $workspace_dir/$s_dir
    cp kyrinbox_chunk_* $workspace_dir/$s_dir
    cp kyrinbox_keycenter_* $workspace_dir/$s_dir
done
for s_dir in ${chunk_list[*]}; do
    cp kyrinbox_master_* $workspace_dir/$s_dir
    cp kyrinbox_chunk_* $workspace_dir/$s_dir
    cp kyrinbox_keycenter_* $workspace_dir/$s_dir
done

echo "* Start services..."
for s_dir in ${master_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_master 2>&1 > /dev/null &
    cd - > /dev/null
done
for s_dir in ${slavenode_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_slavenode 2>&1 > /dev/null &
    cd - > /dev/null
done
for s_dir in ${chunk_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_chunk 2>&1 > /dev/null &
    cd - > /dev/null
done
for s_dir in ${keycenter_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_keycenter 2>&1 > /dev/null &
    cd - > /dev/null
done
for (( i=1; i<=${#slavenode_list[@]}; i++)) do
    nohup $redis_server redis_conf_slavenode_"$i".conf 2>&1 > /dev/null &
done
for (( i=1; i<=${#keycenter_list[@]}; i++)) do
    nohup $redis_server redis_conf_keycenter_"$i".conf 2>&1 > /dev/null &
done
