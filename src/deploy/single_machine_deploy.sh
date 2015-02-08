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

echo "* Copy binary to working directory..."
if [ ! -f "$kyrin_master_binary" ]; then
    echo "Cannot find kyrin master binary at $kyrin_master_binary"
    exit 1
fi
for s_dir in ${master_list[*]}; do
    cp $kyrin_master_binary $workspace_dir/$s_dir
done

echo "* Place configuration file..."
if [ ! -f "$kyrin_master_config_example" ]; then
    echo "Cannot find kyrin master config example file at $kyrin_master_config_example"
    exit 1
fi
for (( i=0; i<${#master_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/\/tmp\/kyrinuserdata_db/\/tmp\/kyrinuserdata_db$bp/g"\
        -e "s/\/tmp\/kyrinoplog_db/\/tmp\/kyrinoplog_db$bp/g"\
        -e "s/7070/"$bp"7070/g"\
        -e "s/7700/"$bp"7700/g"\
        -e "s/7770/"$bp"7770/g"\
        $kyrin_master_config_example\
        > $workspace_dir"/"${master_list[$i]}"/"kyrinbox_master_config.json
    sed -e "s/3\ 1/3\ "$i"/g"\
        $kyrin_master_sentinel_example\
        > $workspace_dir"/"${master_list[$i]}"/"sentinel_config.txt
done

echo "* Start services..."
for s_dir in ${master_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_master sentinel_config.txt 2>&1 > /dev/null &
    cd - > /dev/null
done
