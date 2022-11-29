#!/bin/bash

NAME=yolov3
ACUITY_PATH=../bin/


pegasus=${ACUITY_PATH}pegasus
if [ ! -e "$pegasus" ]; then
    pegasus=${ACUITY_PATH}pegasus.py
fi


#Darknet
#$pegasus import darknet\
#    --model  /home/sajjad/sajjad/darknet/cfg/yolov3.cfg \
#    --weights  /home/sajjad/sajjad/darknet/yolov3.weights \
#    --output-model ${NAME}.json \
#    --output-data ${NAME}.data \

#/home/sajjad/sajjad/darknet/cfg/yolov3.cfg
#/home/sajjad/sajjad/models-zoo/darknet/yolov3/yolov3/yolov3.cfg 
$pegasus import darknet\
    --model  /home/sajjad/sajjad/darknet/cfg/yolov3.cfg \
    --weights  /home/sajjad/sajjad/darknet/yolov3.weights \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data \

#generate inpumeta  --source-file dataset.txt
$pegasus generate inputmeta \
        --model ${NAME}.json \
        --input-meta-output ${NAME}_inputmeta.yml \
	--channel-mean-value "0 0 0 0.00390625"  \
	--source-file data/validation_tf_608.txt
#	--source-file dataset.txt



<<COMMENT
#Tensorflow
$pegasus import tensorflow  \
		--model ./model/mobilenet_v1.pb \
		--inputs input \
		--outputs MobilenetV1/Predictions/Reshape_1 \
		--input-size-list '224,224,3' \
		--output-data ${NAME}.data \
		--output-model ${NAME}.json

#CAFFE
$pegasus import caffe \
	--model caffe/alexnet.prototxt \
	--weights caffe/alexnet.caffemodel  \
	--output-data ${NAME}.data \
	--output-model ${NAME}.json \

#TFLITE
#$pegasus import tflite\
    --model  ${NAME}.tflite \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data 

#Onnx
#$pegasus import onnx\
    --model  ${NAME}.onnx \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data 

#Kreas
#$pegasus import keras\
    --model  ${NAME}.hdf5 \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data 

#Pytorch
#$pegasus import pytorch\
    --model  ${NAME}.pt \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data \
    --input-size-list '3,224,224' \

#Darknet
#$pegasus import darknet\
    --model  ${NAME}.cfg \
	--weights  ${NAME}.weights \
    --output-model ${NAME}.json \
    --output-data ${NAME}.data \
COMMENT


