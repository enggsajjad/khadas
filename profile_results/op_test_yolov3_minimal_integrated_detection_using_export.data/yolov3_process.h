#ifndef __YOLOV3_PROCESS__
#define __YOLOV3_PROCESS__

#include "vsi_nn_pub.h"
#include "vnn_global.h"
#include "nn_detect_common.h"

typedef unsigned char   uint8_t;
typedef unsigned int   uint32_t;

//void yolov3_preprocess(input_image_t imageData, uint8_t *ptr,vsi_nn_graph_t *graph);
void yolov3_postprocess(vsi_nn_graph_t *graph, pDetResult resultData);
void yolov3_preprocess_nbg(input_image_t imageData, uint8_t *ptr,vsi_nn_graph_t *graph,int nn_width, int nn_height, int channels);

#endif
