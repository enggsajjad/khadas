#ifndef __YOLO_V3_INCLUDE__
#define __YOLO_V3_INCLUDE__


#ifdef __cplusplus
extern "C"{
#endif


#include "vnn_global.h"
#include "nn_detect_common.h"
#include "nn_detect_utils.h"

det_status_t model_create(const char * data_file_path, dev_type type);
vsi_nn_graph_t * model_create_nbg(const char * data_file_path, dev_type type);
void model_getsize(int *width, int *height, int *channel);
void model_getsize_nbg(int *width, int *height, int *channel,vsi_nn_graph_t * graph);

void model_setinput(input_image_t imageData, uint8_t* data);
void model_setinput_nbg(input_image_t imageData, uint8_t* data,vsi_nn_graph_t *graph);
det_status_t model_getresult(pDetResult resultData, uint8_t* data);
det_status_t model_getresult_nbg(pDetResult resultData, uint8_t* data,vsi_nn_graph_t *graph);

void model_release(dev_type type);
void model_release_nbg(dev_type type,vsi_nn_graph_t *graph);
void myprint(void);


#ifdef __cplusplus
}
#endif

#endif
