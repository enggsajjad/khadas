/****************************************************************************
*   Generated by ACUITY 6.0.12
*   Match ovxlib 1.1.34
*
*   Neural Network appliction pre-process header file
****************************************************************************/
#ifndef _VNN_PRE_PROCESS_H_
#define _VNN_PRE_PROCESS_H_

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _vnn_file_type
{
    NN_FILE_NONE,
    NN_FILE_TENSOR,
    NN_FILE_QTENSOR,
    NN_FILE_JPG,
    NN_FILE_BINARY
} vnn_file_type_e;

typedef enum _vnn_pre_order
{
    VNN_PREPRO_NONE = -1,
    VNN_PREPRO_REORDER,
    VNN_PREPRO_MEAN,
    VNN_PREPRO_SCALE,
    VNN_PREPRO_NUM
} vnn_pre_order_e;

typedef struct _vnn_input_meta
{
    union
    {
        struct
        {
            int32_t preprocess[VNN_PREPRO_NUM];
            uint32_t reorder[4];
            float mean[4];
            float scale;
        } image;
    };
} vnn_input_meta_t;

vsi_status vnn_PreProcessLenet
    (
    vsi_nn_graph_t *graph,
    const char **inputs,
    uint32_t input_num
    );

vsi_status vnn_PreProcessLenet_ImageProcess
    (
    vsi_nn_graph_t *graph,
    const char **inputs,
    uint32_t input_num
    );

vsi_bool vnn_UseImagePreprocessNode();

void vnn_ReleaseBufferImage();

vsi_size_t vnn_LoadFP32DataFromTextFile
    (
    const char * fname,
    uint8_t ** buffer_ptr,
    vsi_size_t * buffer_sz
    );

vsi_size_t vnn_LoadRawDataFromBinaryFile
    (
    const char * fname,
    uint8_t ** buffer_ptr,
    vsi_size_t * buffer_sz
    );

const vsi_nn_preprocess_map_element_t * vnn_GetPreProcessMap();

uint32_t vnn_GetPreProcessMapCount();

#ifdef __cplusplus
}
#endif


#endif
