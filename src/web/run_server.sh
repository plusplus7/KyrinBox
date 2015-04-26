export PYTHONPATH=$PYTHONPATH:../sdk/python_sdk
cd html
mkdir status
tar zxf static.tar.gz 2>&1 > /dev/null
cd ..
python start_server.py $@
