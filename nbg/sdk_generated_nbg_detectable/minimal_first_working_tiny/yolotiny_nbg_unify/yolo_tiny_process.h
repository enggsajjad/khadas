#ifndef __YOLO_TINY_PROCESS__
#define __YOLO_TINY_PROCESS__

#include "vsi_nn_pub.h"
#include "vnn_global.h"
#include "nn_detect_common.h"

typedef unsigned char   uint8_t;
typedef unsigned int   uint32_t;

//void yolo_tiny_preprocess(input_image_t imageData, uint8_t *ptr);
void yolo_tiny_postprocess(vsi_nn_graph_t *graph, pDetResult resultData);
void yolo_tiny_preprocess_nbg(input_image_t imageData, uint8_t *ptr,vsi_nn_graph_t *graph,int nn_width, int nn_height, int channels);

#endif
