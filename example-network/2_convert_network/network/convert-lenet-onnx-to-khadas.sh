#!/bin/bash
CONVERT_PATH="/acuity-toolkit/python/"
cd $CONVERT_PATH

./convert \
--model-name lenet \
--platform onnx \
--outputs "output" \
--inputs "input" \
--input-size-list '28,28,3' \
--model ./network/lenet.onnx \
--mean-values '127.5,127.5,127.5,255.0' \
--quantized-dtype asymmetric_affine \
--qtype int16 \
--kboard VIM3 \
--print-level 1

