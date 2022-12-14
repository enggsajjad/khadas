/****************************************************************************
*   Generated by ACUITY 5.21.1_0702
*   Match ovxlib 1.1.30
*
*   Neural Network appliction post-process source file
****************************************************************************/
/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vsi_nn_pub.h"

#include "vnn_global.h"
#include "vnn_post_process.h"

#define _BASETSD_H

/*-------------------------------------------
                  Variable definitions
-------------------------------------------*/

/*{graph_output_idx, postprocess}*/
const static vsi_nn_postprocess_map_element_t* postprocess_map = NULL;


/*-------------------------------------------
                  Functions
-------------------------------------------*/
/*
static void save_output_data(vsi_nn_graph_t *graph)
{
    uint32_t i;
#define _DUMP_FILE_LENGTH 1028
#define _DUMP_SHAPE_LENGTH 128
    char filename[_DUMP_FILE_LENGTH] = {0}, shape[_DUMP_SHAPE_LENGTH] = {0};
    vsi_nn_tensor_t *tensor;

    for(i = 0; i < graph->output.num; i++)
    {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);
        vsi_nn_ShapeToString( tensor->attr.size, tensor->attr.dim_num,
            shape, _DUMP_SHAPE_LENGTH, FALSE );
        snprintf(filename, _DUMP_FILE_LENGTH, "output%u_%s.dat", i, shape);
        vsi_nn_SaveTensorToBinary(graph, tensor, filename);

    }
}*/

static void save_output_data(vsi_nn_graph_t *graph)
{
    uint32_t i;
#define _DUMP_FILE_LENGTH 1028
#define _DUMP_SHAPE_LENGTH 128
    char filename[_DUMP_FILE_LENGTH] = {0}, shape[_DUMP_SHAPE_LENGTH] = {0};
    vsi_nn_tensor_t *tensor;

    for(i = 0; i < graph->output.num; i++)
    {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);
        uint32_t *  size = tensor->attr.size;
        uint32_t   dim_num= tensor->attr.dim_num;
        
/* Expected output for 608x608 yolov3 network with all 80 classes
I [vsi_nn_CreateGraph:478]OVXLIB_VERSION==1.1.30
D [setup_node:441]Setup node id[0] uid[0] op[NBG]
D [print_tensor:146]in(0) : id[   0] vtl[0] const[0] shape[ 608, 608, 3, 1   ] fmt[i8 ] qnt[DFP fl=  7]
D [print_tensor:146]out(0): id[   1] vtl[0] const[0] shape[ 19, 19, 255, 1   ] fmt[i8 ] qnt[DFP fl=  2]
D [print_tensor:146]out(1): id[   2] vtl[0] const[0] shape[ 38, 38, 255, 1   ] fmt[i8 ] qnt[DFP fl=  2]
D [print_tensor:146]out(2): id[   3] vtl[0] const[0] shape[ 76, 76, 255, 1   ] fmt[i8 ] qnt[DFP fl=  2]
D [optimize_node:385]Backward optimize neural network
D [optimize_node:392]Forward optimize neural network
I [compute_node:327]Create vx node
D [compute_node:350]Instance node[0] "NBG" ...
Create Neural Network: 92ms or 92774us

*/


        printf("\nOutput %i ... dim_num: %i shape: ",i, dim_num);
        for(int x=0; x<dim_num;x++)printf(" %i",size[x]);

        vsi_nn_ShapeToString( tensor->attr.size, tensor->attr.dim_num,
           shape, _DUMP_SHAPE_LENGTH, FALSE );
        snprintf(filename, _DUMP_FILE_LENGTH, "output%u_%s.dat", i, shape);
        vsi_nn_SaveTensorToBinary(graph, tensor, filename);
//sajjad
    int nn_width,nn_height, nn_channel;
    vsi_nn_tensor_t *tensor = NULL;

    tensor = vsi_nn_GetTensor(graph, graph->input.tensors[0]);
    nn_width = tensor->attr.size[0];
    nn_height = tensor->attr.size[1];
    nn_channel = tensor->attr.size[2];
    printf("\nnn_width: %d",nn_width);
    printf("\nnn_height: %d",nn_height);
    printf("\nnn_channel: %d",nn_channel);
    (void)nn_channel;
    int sizee[3]={nn_width/32, nn_height/32,85*3};

    int sz[10];
    int i, j, stride;
    int output_cnt = 0;
    int output_len = 0;
    int num_class = 80;
    float threshold = 0.5;
    float iou_threshold = 0.4;
    uint8_t *tensor_data = NULL;
    float *predictions = NULL;

    for (i = 0; i < graph->output.num; i++) {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);
        sz[i] = 1;
        for (j = 0; j < tensor->attr.dim_num; j++) {
            sz[i] *= tensor->attr.size[j];
        }
        output_len += sz[i];
    }
    predictions = (float *)malloc(sizeof(float) * output_len);

    for (i = 0; i < graph->output.num; i++) {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);

        stride = vsi_nn_TypeGetBytes(tensor->attr.dtype.vx_type);//2 step-wise
        printf("\nstride: %d",stride);//sajjad
        tensor_data = (uint8_t *)vsi_nn_ConvertTensorToData(graph, tensor);

        //
        //sajjad 
        for (j = 0; j < 10; j++) {
            printf("\ntensor_data [%d]: %d",j,((int16_t*)tensor_data)[j]);
        }
        printf("\ntensor->attr.dtype.fl: %f",tensor->attr.dtype.fl);
        float fl = pow(2.,-tensor->attr.dtype.fl);
        for (j = 0; j < sz[i]; j++) {
            int val = tensor_data[stride*j];
            printf("\nval: %d", val);//sajjad
            int tmp1=(val>=128)?val-256:val;
            predictions[output_cnt]= tmp1*fl;
            printf("\npredictions [%d]: %f",output_cnt,predictions[output_cnt]);//sajjad
            output_cnt++;
        }
        vsi_nn_Free(tensor_data);
    }

    float biases[18] = {10/8., 13/8., 16/8., 30/8., 33/8., 23/8., 30/16., 61/16., 62/16., 45/16., 59/16., 119/16., 116/32., 90/32., 156/32., 198/32., 373/32., 326/32.};
    int size2[3] = {sizee[0]*2,sizee[1]*2,sizee[2]};
    int size4[3] = {sizee[0]*4,sizee[1]*4,sizee[2]};
    int len1 = sizee[0]*sizee[1]*sizee[2];
    int box1 = len1/(num_class+5);


    //box *boxes = (box *)calloc(box1*(1+4+16), sizeof(box));
    //float **probs = (float **)calloc(box1*(1+4+16), sizeof(float *));

    //yolo_v3_post_process_onescale(&predictions[0], size, &biases[12], boxes, &probs[0], threshold); //final layer
    //yolo_v3_post_process_onescale(&predictions[len1], size2, &biases[6], &boxes[box1], &probs[box1], threshold);
    //yolo_v3_post_process_onescale(&predictions[len1*(1+4)], size4, &biases[0],  &boxes[box1*(1+4)], &probs[box1*(1+4)], threshold);
    //do_nms_sort(boxes, probs, box1*21, num_class, iou_threshold);
    //get_detections_result(resultData, box1*21, threshold, boxes, probs, coco_names, num_class);

    /*free(boxes);
    boxes = NULL;

    if (predictions) free(predictions);

    for (j = 0; j < box1*(1+4+16); ++j) {
        free(probs[j]);
    }
    free(probs);
    return;
*/
    }
    printf("\n");
}

static vsi_bool get_top
    (
    float *pfProb,
    float *pfMaxProb,
    uint32_t *pMaxClass,
    uint32_t outputCount,
    uint32_t topNum
    )
{
    uint32_t i, j, k;

    #define MAX_TOP_NUM 20
    if (topNum > MAX_TOP_NUM) return FALSE;

    memset(pfMaxProb, 0xfe, sizeof(float) * topNum);
    memset(pMaxClass, 0xff, sizeof(float) * topNum);

    for (j = 0; j < topNum; j++)
    {
        for (i=0; i<outputCount; i++)
        {
            for (k=0; k < topNum; k ++)
            {
                if(i == pMaxClass[k])
                    break;
            }

            if (k != topNum)
                continue;

            if (pfProb[i] > *(pfMaxProb+j))
            {
                *(pfMaxProb+j) = pfProb[i];
                *(pMaxClass+j) = i;
            }
        }
    }

    return TRUE;
}

static vsi_status show_top5
    (
    vsi_nn_graph_t *graph,
    vsi_nn_tensor_t *tensor
    )
{
    vsi_status status = VSI_FAILURE;
    uint32_t i,sz,stride;
    float *buffer = NULL;
    uint8_t *tensor_data = NULL;
    uint32_t MaxClass[5];
    float fMaxProb[5];
    uint32_t topk = 5;

    sz = 1;
    for(i = 0; i < tensor->attr.dim_num; i++)
    {
        sz *= tensor->attr.size[i];
    }

    if(topk > sz)
        topk = sz;

    stride = vsi_nn_TypeGetBytes(tensor->attr.dtype.vx_type);
    tensor_data = (uint8_t *)vsi_nn_ConvertTensorToData(graph, tensor);
    buffer = (float *)malloc(sizeof(float) * sz);

    for(i = 0; i < sz; i++)
    {
        status = vsi_nn_DtypeToFloat32(&tensor_data[stride * i], &buffer[i], &tensor->attr.dtype);
    }

    if (!get_top(buffer, fMaxProb, MaxClass, sz, topk))
    {
        printf("Fail to show result.\n");
        goto final;
    }

    printf(" --- Top%d ---\n", topk);
    for(i = 0; i< topk; i++)
    {
        printf("%3d: %8.6f\n", MaxClass[i], fMaxProb[i]);
    }
    status = VSI_SUCCESS;

final:
    if(tensor_data)vsi_nn_Free(tensor_data);
    if(buffer)free(buffer);
    return status;
}

vsi_status vnn_PostProcessLenet(vsi_nn_graph_t *graph)
{
    vsi_status status = VSI_FAILURE;

    /* Show the top5 result */
    status = show_top5(graph, vsi_nn_GetTensor(graph, graph->output.tensors[0]));
    TEST_CHECK_STATUS(status, final);

    /* Save all output tensor data to txt file */
    save_output_data(graph);

final:
    return VSI_SUCCESS;
}

const vsi_nn_postprocess_map_element_t * vnn_GetPostProcessMap()
{
    return postprocess_map;
}

uint32_t vnn_GetPostProcessMapCount()
{
    if (postprocess_map == NULL)
       return 0;
    else
        return sizeof(postprocess_map) / sizeof(vsi_nn_postprocess_map_element_t);
}
