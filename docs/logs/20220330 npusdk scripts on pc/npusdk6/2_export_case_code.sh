#!/bin/bash

NAME=mobilenet_tf
ACUITY_PATH=../bin/

export_ovxlib=${ACUITY_PATH}ovxgenerator

$export_ovxlib \
    --model-input ${NAME}.json \
    --data-input ${NAME}.data \
    --model-quantize ${NAME}.quantize \
    --reorder-channel '0 1 2' \
    --channel-mean-value '128 128 128 128' \
    --export-dtype quantized \

