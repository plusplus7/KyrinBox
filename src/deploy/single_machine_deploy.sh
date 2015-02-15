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
if [ ! -f "$kyrin_config_example" ]; then
    echo "Cannot find kyrin master config example file at $kyrin_config_example"
    exit 1
fi
for (( i=0; i<${#master_list[@]}; i++)) do
    (( bp=$i+1 ))
    sed -e "s/DEPLOY_TO_PUT_KBID_IN/$bp/g"\
        $kyrin_config_example\
        > $workspace_dir"/"${master_list[$i]}"/"kyrinbox_config.json
done
for s_dir in ${master_list[*]}; do
    cp kyrinbox_master_* $workspace_dir/$s_dir
done

echo "* Start services..."
for s_dir in ${master_list[*]}; do
    cd "$workspace_dir"/"$s_dir"
    nohup ./kyrin_master 2>&1 > /dev/null &
    cd - > /dev/null
done
