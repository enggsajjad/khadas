#!/bin/bash

NAME=mnist
ACUITY_PATH=../bin/

convert_caffe=${ACUITY_PATH}convertcaffe
convert_tf=${ACUITY_PATH}convertensorflow
convert_tflite=${ACUITY_PATH}convertflit
convert_darknet=${ACUITY_PATH}convertdarknet
convert_onnx=${ACUITY_PATH}convertonnx
convert_keras=${ACUITY_PATH}convertkeras
convert_pytorch=${ACUITY_PATH}convertpytorch

$convert_onnx \
   --onnx-model  ./network/${NAME}rB.onnx \
   --inputs "input" \
   --input-size-list "3,28,28" \
   --outputs "output" \
   --input-dtype-list "float" \
   --net-output ${NAME}.json \
   --data-output ${NAME}.data  \
   --size-with-batch "1" 

