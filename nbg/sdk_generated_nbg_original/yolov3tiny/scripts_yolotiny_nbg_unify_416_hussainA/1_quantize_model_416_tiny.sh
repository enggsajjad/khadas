#!/bin/bash

NAME=yolotiny
ACUITY_PATH=../bin/

pegasus=${ACUITY_PATH}pegasus
if [ ! -e "$pegasus" ]; then
    pegasus=${ACUITY_PATH}pegasus.py
fi

$pegasus  quantize \
        --quantizer dynamic_fixed_point \
        --qtype int8 \
        --rebuild \
        --with-input-meta  ${NAME}_inputmeta.yml \
        --model  ${NAME}.json \
        --model-data  ${NAME}.data
