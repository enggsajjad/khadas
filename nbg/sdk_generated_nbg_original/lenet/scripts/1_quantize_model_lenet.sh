#!/bin/bash

NAME=lenet
ACUITY_PATH=../bin/

pegasus=${ACUITY_PATH}pegasus
if [ ! -e "$pegasus" ]; then
    pegasus=${ACUITY_PATH}pegasus.py
fi
#asymmetric_affine
$pegasus  quantize \
        --quantizer dynamic_fixed_point \
        --qtype int16 \
        --rebuild \
        --with-input-meta  ${NAME}_inputmeta.yml \
        --model  ${NAME}.json \
        --model-data  ${NAME}.data \
	--batch-size 1 



#tensorzone=${ACUITY_PATH}tensorzonex

#$tensorzone \
#    --action quantization \
#    --dtype float32 \
#    --source text \
#    --source-file data/validation_tf.txt \
#    --channel-mean-value '0 0 0 256' \
#    --reorder-channel '2 1 0' \
#    --model-input ${NAME}.json \
#    --model-data ${NAME}.data \
#    --quantized-dtype dynamic_fixed_point-i8 \
#    --quantized-rebuild \
##    --batch-size 2 \
##    --epochs 5
#
