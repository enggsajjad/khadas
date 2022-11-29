#!/bin/bash

NAME=yolotiny
ACUITY_PATH=../bin/

pegasus=${ACUITY_PATH}pegasus
if [ ! -e "$pegasus" ]; then
    pegasus=${ACUITY_PATH}pegasus.py
fi

#--model  /home/sajjad/sajjad/models-zoo/darknet/yolov3/yolov3/yolov3-tiny.cfg \
#--weights  /home/sajjad/sajjad/yolov3.weights \
$pegasus import darknet\
    --model  /home/sajjad/sajjad/darknet/cfg/yolov3-tiny.cfg \
    --weights /home/sajjad/sajjad/darknet/yolov3-tiny.weights \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data \

#generate inpumeta  --source-file dataset.txt
$pegasus generate inputmeta \
        --model ${NAME}.json \
        --input-meta-output ${NAME}_inputmeta.yml \
        --channel-mean-value "0 0 0 0.003906"  \
	--source-file data/validation_tf_416.txt

