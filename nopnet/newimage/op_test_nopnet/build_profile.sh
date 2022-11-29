#!/bin/bash

set -e

export VIVANTE_SDK_DIR=/home/khadas/hussain/Just_for_get_op_time_new/data/vcmdtools
export LD_LIBRARY_PATH=/home/khadas/hussain/Just_for_get_op_time_new/data/drivers_64_exportdata
export VIV_VX_DEBUG_LEVEL=1
export CNN_PERF=1
export NN_LAYER_DUMP=1

cd bin_r_cv4
./nopnet ../nopnet_88.nb ../28x28.jpg
./nopnet ../nopnet_88.nb ../black.jpg
cd ..

