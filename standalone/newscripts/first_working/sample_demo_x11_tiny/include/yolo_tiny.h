#ifndef __YOLO_TINY_INCLUDE__
#define __YOLO_TINY_INCLUDE__

#ifdef __cplusplus
extern "C"{
#endif

#include "vnn_global.h"
#include "nn_detect_common.h"
#include "nn_detect_utils.h"

det_status_t model_create(const char * data_file_path, dev_type type);
void model_getsize(int *width, int *height, int *channel);
void model_setinput(input_image_t imageData, uint8_t* data);
det_status_t model_getresult(pDetResult resultData, uint8_t* data);
void model_release(dev_type type);

#ifdef __cplusplus
}
#endif

#endif