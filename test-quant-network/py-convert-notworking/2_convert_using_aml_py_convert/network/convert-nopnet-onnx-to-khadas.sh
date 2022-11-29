#!/bin/bash
CONVERT_PATH="/acuity-toolkit/python/"
cd $CONVERT_PATH

./convert \
--model-name nopnet \
--platform onnx \
--inputs "input" \
--input-size-list '28,28,3' \
--model ./network/nopnet.onnx \
--mean-values '127.5,127.5,127.5,127.5' \
--quantized-dtype dynamic_fixed_point \
--qtype int8 \
--kboard VIM3 \
--print-level 1

