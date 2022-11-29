#!/bin/bash

NAME=nopnet
ACUITY_PATH=../bin/

convert_caffe=${ACUITY_PATH}convertcaffe
convert_tf=${ACUITY_PATH}convertensorflow
convert_tflite=${ACUITY_PATH}convertflit
convert_darknet=${ACUITY_PATH}convertdarknet
convert_onnx=${ACUITY_PATH}convertonnx
convert_keras=${ACUITY_PATH}convertkeras
convert_pytorch=${ACUITY_PATH}convertpytorch

# $convert_tf \
#     --tf-pb ./model/mobilenet_v1.pb \
#     --inputs input \
#     --input-size-list '224,224,3' \
#     --outputs MobilenetV1/Predictions/Softmax \
#     --net-output ${NAME}.json \
#     --data-output ${NAME}.data 
	
#$convert_caffe \
#    --caffe-model xx.prototxt   \
#	--caffe-blobs xx.caffemodel \
#    --net-output ${NAME}.json \
#    --data-output ${NAME}.data 
	
#$convert_tflite \
#    --tflite-mode  xxxx.tflite \
#    --net-output ${NAME}.json \
#    --data-output ${NAME}.data 

#$convert_darknet \
#    --net-input xxx.cfg \
#	--weight-input xxx.weights \
#    --net-output ${NAME}.json \
#    --data-output ${NAME}.data 
	
$convert_onnx \
   --onnx-model  ./network/${NAME}.onnx \
   --inputs "input" \
   --input-size-list "3,28,28" \
   --outputs "output" \
   --input-dtype-list "float" \
   --net-output ${NAME}.json \
   --data-output ${NAME}.data  \
   --size-with-batch "1" 

#$convert_keras \
#	--keras-model xxx.hdf5 \
#	--net-output ${NAME}.json --data-output ${NAME}.data


#$convert_pytorch --pytorch-model xxxx.pt \
#        --net-output ${NAME}.json \
#        --data-output ${NAME}.data \
#	--input-size-list '1,480,854'
