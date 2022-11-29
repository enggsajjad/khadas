/****************************************************************************
*   Generated by ACUITY 5.21.1_0702
*   Match ovxlib 1.1.30
*
*   Neural Network appliction post-process header file
****************************************************************************/
#ifndef _VNN_POST_PROCESS_H_
#define _VNN_POST_PROCESS_H_

#ifdef __cplusplus
extern "C"{
#endif

vsi_status vnn_PostProcessMnist(vsi_nn_graph_t *graph);

const vsi_nn_postprocess_map_element_t * vnn_GetPostProcessMap();

uint32_t vnn_GetPostProcessMapCount();

#ifdef __cplusplus
}
#endif

#endif
