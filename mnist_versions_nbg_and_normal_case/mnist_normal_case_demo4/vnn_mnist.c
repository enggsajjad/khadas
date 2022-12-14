/****************************************************************************
*   Generated by ACUITY 5.21.1_0702
*   Match ovxlib 1.1.30
*
*   Neural Network appliction network definition source file
****************************************************************************/
/*-------------------------------------------
                   Includes
 -------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "vsi_nn_pub.h"

#include "vnn_global.h"
#include "vnn_mnist.h"

/*-------------------------------------------
                   Macros
 -------------------------------------------*/

#define NEW_VXNODE(_node, _type, _in, _out, _uid) do {\
        _node = vsi_nn_AddNode( graph, _type, _in, _out, NULL );\
        if( NULL == _node ) {\
            goto error;\
        }\
        _node->uid = (uint32_t)_uid;\
    } while(0)

#define NEW_VIRTUAL_TENSOR(_id, _attr, _dtype) do {\
        memset( _attr.size, 0, VSI_NN_MAX_DIM_NUM * sizeof(uint32_t));\
        _attr.dim_num = VSI_NN_DIM_AUTO;\
        _attr.vtl = !VNN_APP_DEBUG;\
        _attr.is_const = FALSE;\
        _attr.dtype.vx_type = _dtype;\
        _id = vsi_nn_AddTensor( graph, VSI_NN_TENSOR_ID_AUTO,\
                & _attr, NULL );\
        if( VSI_NN_TENSOR_ID_NA == _id ) {\
            goto error;\
        }\
    } while(0)

// Set const tensor dims out of this macro.
#define NEW_CONST_TENSOR(_id, _attr, _dtype, _ofst, _size) do {\
        data = load_data( fp, _ofst, _size  );\
        _attr.vtl = FALSE;\
        _attr.is_const = TRUE;\
        _attr.dtype.vx_type = _dtype;\
        _id = vsi_nn_AddTensor( graph, VSI_NN_TENSOR_ID_AUTO,\
                & _attr, data );\
        free( data );\
        if( VSI_NN_TENSOR_ID_NA == _id ) {\
            goto error;\
        }\
    } while(0)

// Set generic tensor dims out of this macro.
#define NEW_NORM_TENSOR(_id, _attr, _dtype) do {\
        _attr.vtl = FALSE;\
        _attr.is_const = FALSE;\
        _attr.dtype.vx_type = _dtype;\
        _id = vsi_nn_AddTensor( graph, VSI_NN_TENSOR_ID_AUTO,\
                & _attr, NULL );\
        if( VSI_NN_TENSOR_ID_NA == _id ) {\
            goto error;\
        }\
    } while(0)

// Set generic tensor dims out of this macro.
#define NEW_NORM_TENSOR_FROM_HANDLE(_id, _attr, _dtype) do {\
        _attr.vtl = FALSE;\
        _attr.is_const = FALSE;\
        _attr.dtype.vx_type = _dtype;\
        _id = vsi_nn_AddTensorFromHandle( graph, VSI_NN_TENSOR_ID_AUTO,\
                & _attr, NULL );\
        if( VSI_NN_TENSOR_ID_NA == _id ) {\
            goto error;\
        }\
    } while(0)

#define NET_NODE_NUM            (21)
#define NET_NORM_TENSOR_NUM     (2)
#define NET_CONST_TENSOR_NUM    (18)
#define NET_VIRTUAL_TENSOR_NUM  (21)
#define NET_TOTAL_TENSOR_NUM    (NET_NORM_TENSOR_NUM + NET_CONST_TENSOR_NUM + NET_VIRTUAL_TENSOR_NUM)

/*-------------------------------------------
               Local Variables
 -------------------------------------------*/

/*-------------------------------------------
                  Functions
 -------------------------------------------*/
static uint8_t* load_data
    (
    FILE  * fp,
    size_t  ofst,
    size_t  sz
    )
{
    uint8_t* data;
    int32_t ret;
    data = NULL;
    if( NULL == fp )
    {
        return NULL;
    }

    ret = fseek(fp, ofst, SEEK_SET);
    if (ret != 0)
    {
        VSILOGE("blob seek failure.");
        return NULL;
    }

    data = (uint8_t*)malloc(sz);
    if (data == NULL)
    {
        VSILOGE("buffer malloc failure.");
        return NULL;
    }
    ret = fread(data, 1, sz, fp);
    return data;
} /* load_data() */

vsi_nn_graph_t * vnn_CreateMnist
    (
    const char * data_file_name,
    vsi_nn_context_t in_ctx,
    const vsi_nn_preprocess_map_element_t * pre_process_map,
    uint32_t pre_process_map_count,
    const vsi_nn_postprocess_map_element_t * post_process_map,
    uint32_t post_process_map_count
    )
{
    uint32_t                _infinity = VSI_NN_FLOAT32_INF;
    vsi_status              status;
    vsi_bool                release_ctx;
    vsi_nn_context_t        ctx;
    vsi_nn_graph_t *        graph;
    vsi_nn_node_t *         node[NET_NODE_NUM];
    vsi_nn_tensor_id_t      norm_tensor[NET_NORM_TENSOR_NUM];
    vsi_nn_tensor_id_t      const_tensor[NET_CONST_TENSOR_NUM];
    vsi_nn_tensor_attr_t    attr;
    FILE *                  fp;
    uint8_t *               data;
    uint32_t                i = 0;
    char *                  use_img_process_s;
    int32_t                 enable_pre_post_process = 0;
    vsi_bool                sort = FALSE;

    uint32_t   shape_1[] = { 800, -1 };




    (void)(_infinity);
    ctx = NULL;
    graph = NULL;
    status = VSI_FAILURE;
    memset( &attr, 0, sizeof( attr ) );

    fp = fopen( data_file_name, "rb" );
    if( NULL == fp )
    {
        VSILOGE( "Open file %s failed.", data_file_name );
        goto error;
    }

    if( NULL == in_ctx )
    {
        ctx = vsi_nn_CreateContext();
    }
    else
    {
        ctx = in_ctx;
    }

    use_img_process_s = getenv( "VSI_USE_IMAGE_PROCESS" );
    if( use_img_process_s )
    {
        enable_pre_post_process = atoi(use_img_process_s);
    }

    graph = vsi_nn_CreateGraph( ctx, NET_TOTAL_TENSOR_NUM, NET_NODE_NUM );
    if( NULL == graph )
    {
        VSILOGE( "Create graph fail." );
        goto error;
    }
    vsi_nn_SetGraphVersion( graph, VNN_VERSION_MAJOR, VNN_VERSION_MINOR, VNN_VERSION_PATCH );
    vsi_nn_SetGraphInputs( graph, NULL, 1 );
    vsi_nn_SetGraphOutputs( graph, NULL, 1 );

/*-----------------------------------------
  Register client ops
 -----------------------------------------*/


/*-----------------------------------------
  Node definitions
 -----------------------------------------*/

    /*-----------------------------------------
      lid       - Conv_Conv_0_21
      var       - node[0]
      name      - Conv_Conv_0
      operation - convolution
      input     - [28, 28, 3, 1]
      filter    - [5, 5, 3, 20]
      output    - [24, 24, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[0], VSI_NN_OP_CONV2D, 3, 1, 21);
    node[0]->nn_param.conv2d.ksize[0] = 5;
    node[0]->nn_param.conv2d.ksize[1] = 5;
    node[0]->nn_param.conv2d.weights = 20;
    node[0]->nn_param.conv2d.stride[0] = 1;
    node[0]->nn_param.conv2d.stride[1] = 1;
    node[0]->nn_param.conv2d.pad[0] = 0;
    node[0]->nn_param.conv2d.pad[1] = 0;
    node[0]->nn_param.conv2d.pad[2] = 0;
    node[0]->nn_param.conv2d.pad[3] = 0;
    node[0]->nn_param.conv2d.group = 1;
    node[0]->nn_param.conv2d.dilation[0] = 1;
    node[0]->nn_param.conv2d.dilation[1] = 1;
    node[0]->nn_param.conv2d.multiplier = 0;
    node[0]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[0]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[0]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_1_20
      var       - node[1]
      name      - Relu_Relu_1
      operation - relu
      input     - [24, 24, 20, 1]
      output    - [24, 24, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[1], VSI_NN_OP_RELU, 1, 1, 20);

    /*-----------------------------------------
      lid       - MaxPool_MaxPool_2_19
      var       - node[2]
      name      - MaxPool_MaxPool_2
      operation - pooling
      input     - [24, 24, 20, 1]
      output    - [12, 12, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[2], VSI_NN_OP_POOL, 1, 1, 19);
    node[2]->nn_param.pool.ksize[0] = 2;
    node[2]->nn_param.pool.ksize[1] = 2;
    node[2]->nn_param.pool.stride[0] = 2;
    node[2]->nn_param.pool.stride[1] = 2;
    node[2]->nn_param.pool.pad[0] = 0;
    node[2]->nn_param.pool.pad[1] = 0;
    node[2]->nn_param.pool.pad[2] = 0;
    node[2]->nn_param.pool.pad[3] = 0;
    node[2]->nn_param.pool.type = VX_CONVOLUTIONAL_NETWORK_POOLING_MAX;
    node[2]->nn_param.pool.round_type = VSI_NN_ROUND_FLOOR;
    node[2]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Conv_Conv_3_18
      var       - node[3]
      name      - Conv_Conv_3
      operation - convolution
      input     - [12, 12, 20, 1]
      filter    - [5, 5, 20, 20]
      output    - [8, 8, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[3], VSI_NN_OP_CONV2D, 3, 1, 18);
    node[3]->nn_param.conv2d.ksize[0] = 5;
    node[3]->nn_param.conv2d.ksize[1] = 5;
    node[3]->nn_param.conv2d.weights = 20;
    node[3]->nn_param.conv2d.stride[0] = 1;
    node[3]->nn_param.conv2d.stride[1] = 1;
    node[3]->nn_param.conv2d.pad[0] = 0;
    node[3]->nn_param.conv2d.pad[1] = 0;
    node[3]->nn_param.conv2d.pad[2] = 0;
    node[3]->nn_param.conv2d.pad[3] = 0;
    node[3]->nn_param.conv2d.group = 1;
    node[3]->nn_param.conv2d.dilation[0] = 1;
    node[3]->nn_param.conv2d.dilation[1] = 1;
    node[3]->nn_param.conv2d.multiplier = 0;
    node[3]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[3]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[3]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_4_17
      var       - node[4]
      name      - Relu_Relu_4
      operation - relu
      input     - [8, 8, 20, 1]
      output    - [8, 8, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[4], VSI_NN_OP_RELU, 1, 1, 17);

    /*-----------------------------------------
      lid       - Conv_Conv_5_16
      var       - node[5]
      name      - Conv_Conv_5
      operation - convolution
      input     - [8, 8, 20, 1]
      filter    - [1, 1, 20, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[5], VSI_NN_OP_CONV2D, 3, 1, 16);
    node[5]->nn_param.conv2d.ksize[0] = 1;
    node[5]->nn_param.conv2d.ksize[1] = 1;
    node[5]->nn_param.conv2d.weights = 50;
    node[5]->nn_param.conv2d.stride[0] = 1;
    node[5]->nn_param.conv2d.stride[1] = 1;
    node[5]->nn_param.conv2d.pad[0] = 0;
    node[5]->nn_param.conv2d.pad[1] = 0;
    node[5]->nn_param.conv2d.pad[2] = 0;
    node[5]->nn_param.conv2d.pad[3] = 0;
    node[5]->nn_param.conv2d.group = 1;
    node[5]->nn_param.conv2d.dilation[0] = 1;
    node[5]->nn_param.conv2d.dilation[1] = 1;
    node[5]->nn_param.conv2d.multiplier = 0;
    node[5]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[5]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[5]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_6_15
      var       - node[6]
      name      - Relu_Relu_6
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[6], VSI_NN_OP_RELU, 1, 1, 15);

    /*-----------------------------------------
      lid       - Conv_Conv_7_14
      var       - node[7]
      name      - Conv_Conv_7
      operation - convolution
      input     - [8, 8, 50, 1]
      filter    - [1, 1, 50, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[7], VSI_NN_OP_CONV2D, 3, 1, 14);
    node[7]->nn_param.conv2d.ksize[0] = 1;
    node[7]->nn_param.conv2d.ksize[1] = 1;
    node[7]->nn_param.conv2d.weights = 50;
    node[7]->nn_param.conv2d.stride[0] = 1;
    node[7]->nn_param.conv2d.stride[1] = 1;
    node[7]->nn_param.conv2d.pad[0] = 0;
    node[7]->nn_param.conv2d.pad[1] = 0;
    node[7]->nn_param.conv2d.pad[2] = 0;
    node[7]->nn_param.conv2d.pad[3] = 0;
    node[7]->nn_param.conv2d.group = 1;
    node[7]->nn_param.conv2d.dilation[0] = 1;
    node[7]->nn_param.conv2d.dilation[1] = 1;
    node[7]->nn_param.conv2d.multiplier = 0;
    node[7]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[7]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[7]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_8_13
      var       - node[8]
      name      - Relu_Relu_8
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[8], VSI_NN_OP_RELU, 1, 1, 13);

    /*-----------------------------------------
      lid       - Conv_Conv_9_12
      var       - node[9]
      name      - Conv_Conv_9
      operation - convolution
      input     - [8, 8, 50, 1]
      filter    - [1, 1, 50, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[9], VSI_NN_OP_CONV2D, 3, 1, 12);
    node[9]->nn_param.conv2d.ksize[0] = 1;
    node[9]->nn_param.conv2d.ksize[1] = 1;
    node[9]->nn_param.conv2d.weights = 50;
    node[9]->nn_param.conv2d.stride[0] = 1;
    node[9]->nn_param.conv2d.stride[1] = 1;
    node[9]->nn_param.conv2d.pad[0] = 0;
    node[9]->nn_param.conv2d.pad[1] = 0;
    node[9]->nn_param.conv2d.pad[2] = 0;
    node[9]->nn_param.conv2d.pad[3] = 0;
    node[9]->nn_param.conv2d.group = 1;
    node[9]->nn_param.conv2d.dilation[0] = 1;
    node[9]->nn_param.conv2d.dilation[1] = 1;
    node[9]->nn_param.conv2d.multiplier = 0;
    node[9]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[9]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[9]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_10_11
      var       - node[10]
      name      - Relu_Relu_10
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[10], VSI_NN_OP_RELU, 1, 1, 11);

    /*-----------------------------------------
      lid       - Conv_Conv_11_10
      var       - node[11]
      name      - Conv_Conv_11
      operation - convolution
      input     - [8, 8, 50, 1]
      filter    - [1, 1, 50, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[11], VSI_NN_OP_CONV2D, 3, 1, 10);
    node[11]->nn_param.conv2d.ksize[0] = 1;
    node[11]->nn_param.conv2d.ksize[1] = 1;
    node[11]->nn_param.conv2d.weights = 50;
    node[11]->nn_param.conv2d.stride[0] = 1;
    node[11]->nn_param.conv2d.stride[1] = 1;
    node[11]->nn_param.conv2d.pad[0] = 0;
    node[11]->nn_param.conv2d.pad[1] = 0;
    node[11]->nn_param.conv2d.pad[2] = 0;
    node[11]->nn_param.conv2d.pad[3] = 0;
    node[11]->nn_param.conv2d.group = 1;
    node[11]->nn_param.conv2d.dilation[0] = 1;
    node[11]->nn_param.conv2d.dilation[1] = 1;
    node[11]->nn_param.conv2d.multiplier = 0;
    node[11]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[11]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[11]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_12_9
      var       - node[12]
      name      - Relu_Relu_12
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[12], VSI_NN_OP_RELU, 1, 1, 9);

    /*-----------------------------------------
      lid       - Conv_Conv_13_8
      var       - node[13]
      name      - Conv_Conv_13
      operation - convolution
      input     - [8, 8, 50, 1]
      filter    - [1, 1, 50, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[13], VSI_NN_OP_CONV2D, 3, 1, 8);
    node[13]->nn_param.conv2d.ksize[0] = 1;
    node[13]->nn_param.conv2d.ksize[1] = 1;
    node[13]->nn_param.conv2d.weights = 50;
    node[13]->nn_param.conv2d.stride[0] = 1;
    node[13]->nn_param.conv2d.stride[1] = 1;
    node[13]->nn_param.conv2d.pad[0] = 0;
    node[13]->nn_param.conv2d.pad[1] = 0;
    node[13]->nn_param.conv2d.pad[2] = 0;
    node[13]->nn_param.conv2d.pad[3] = 0;
    node[13]->nn_param.conv2d.group = 1;
    node[13]->nn_param.conv2d.dilation[0] = 1;
    node[13]->nn_param.conv2d.dilation[1] = 1;
    node[13]->nn_param.conv2d.multiplier = 0;
    node[13]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[13]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[13]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_14_7
      var       - node[14]
      name      - Relu_Relu_14
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[14], VSI_NN_OP_RELU, 1, 1, 7);

    /*-----------------------------------------
      lid       - MaxPool_MaxPool_15_6
      var       - node[15]
      name      - MaxPool_MaxPool_15
      operation - pooling
      input     - [8, 8, 50, 1]
      output    - [4, 4, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[15], VSI_NN_OP_POOL, 1, 1, 6);
    node[15]->nn_param.pool.ksize[0] = 2;
    node[15]->nn_param.pool.ksize[1] = 2;
    node[15]->nn_param.pool.stride[0] = 2;
    node[15]->nn_param.pool.stride[1] = 2;
    node[15]->nn_param.pool.pad[0] = 0;
    node[15]->nn_param.pool.pad[1] = 0;
    node[15]->nn_param.pool.pad[2] = 0;
    node[15]->nn_param.pool.pad[3] = 0;
    node[15]->nn_param.pool.type = VX_CONVOLUTIONAL_NETWORK_POOLING_MAX;
    node[15]->nn_param.pool.round_type = VSI_NN_ROUND_FLOOR;
    node[15]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Reshape_Reshape_18_5
      var       - node[16]
      name      - Reshape_Reshape_18
      operation - reshape
      input     - [4, 4, 50, 1]
      output    - [800, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[16], VSI_NN_OP_RESHAPE, 1, 1, 5);
    node[16]->nn_param.reshape.size = shape_1;
    node[16]->nn_param.reshape.dim_num = 2;

    /*-----------------------------------------
      lid       - Gemm_Gemm_19_4
      var       - node[17]
      name      - Gemm_Gemm_19
      operation - fullconnect
      input     - [800, 1]
      filter    - [800, 500]
      output    - [500, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[17], VSI_NN_OP_FCL, 3, 1, 4);
    node[17]->nn_param.fcl.weights = 500;
    node[17]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[17]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[17]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_20_3
      var       - node[18]
      name      - Relu_Relu_20
      operation - relu
      input     - [500, 1]
      output    - [500, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[18], VSI_NN_OP_RELU, 1, 1, 3);

    /*-----------------------------------------
      lid       - Gemm_Gemm_21_2
      var       - node[19]
      name      - Gemm_Gemm_21
      operation - fullconnect
      input     - [500, 1]
      filter    - [500, 10]
      output    - [10, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[19], VSI_NN_OP_FCL, 3, 1, 2);
    node[19]->nn_param.fcl.weights = 10;
    node[19]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[19]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_ZERO;
    node[19]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - LogSoftmax_LogSoftmax_22_1
      var       - node[20]
      name      - LogSoftmax_LogSoftmax_22
      operation - log_softmax
      input     - [10, 1]
      output    - [10, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[20], VSI_NN_OP_LOG_SOFTMAX, 1, 1, 1);
    node[20]->nn_param.log_softmax.betaValue = 1.0;
    node[20]->nn_param.log_softmax.axis = 0;


/*-----------------------------------------
  Tensor initialize
 -----------------------------------------*/
    attr.dtype.fmt = VSI_NN_DIM_FMT_NCHW;
    /* @attach_LogSoftmax_LogSoftmax_22/out0_0:out0 */
    attr.size[0] = 10;
    attr.size[1] = 1;
    attr.dim_num = 2;
    attr.dtype.scale = 0.08149270713329315;
    attr.dtype.zero_point = 255;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_NORM_TENSOR(norm_tensor[0], attr, VSI_NN_TYPE_UINT8);

    /* @input_22:out0 */
    attr.size[0] = 28;
    attr.size[1] = 28;
    attr.size[2] = 3;
    attr.size[3] = 1;
    attr.dim_num = 4;
    attr.dtype.scale = 0.003921568859368563;
    attr.dtype.zero_point = 127;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_NORM_TENSOR(norm_tensor[1], attr, VSI_NN_TYPE_UINT8);



    /* @Conv_Conv_0_21:weight */
    attr.size[0] = 5;
    attr.size[1] = 5;
    attr.size[2] = 3;
    attr.size[3] = 20;
    attr.dim_num = 4;
    attr.dtype.scale = 0.0016784174367785454;
    attr.dtype.zero_point = 132;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[0], attr, VSI_NN_TYPE_UINT8, 80, 1500);

    /* @Conv_Conv_0_21:bias */
    attr.size[0] = 20;
    attr.dim_num = 1;
    attr.dtype.scale = 6.582029553091947e-06;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[1], attr, VSI_NN_TYPE_INT32, 0, 80);

    /* @Conv_Conv_3_18:weight */
    attr.size[0] = 5;
    attr.size[1] = 5;
    attr.size[2] = 20;
    attr.size[3] = 20;
    attr.dim_num = 4;
    attr.dtype.scale = 0.0011014711344614625;
    attr.dtype.zero_point = 139;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[2], attr, VSI_NN_TYPE_UINT8, 7060, 10000);

    /* @Conv_Conv_3_18:bias */
    attr.size[0] = 20;
    attr.dim_num = 1;
    attr.dtype.scale = 8.31885086867559e-06;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[3], attr, VSI_NN_TYPE_INT32, 6980, 80);

    /* @Conv_Conv_5_16:weight */
    attr.size[0] = 1;
    attr.size[1] = 1;
    attr.size[2] = 20;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.scale = 0.002342360559850931;
    attr.dtype.zero_point = 116;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[4], attr, VSI_NN_TYPE_UINT8, 17260, 1000);

    /* @Conv_Conv_5_16:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.scale = 3.103645157099535e-05;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[5], attr, VSI_NN_TYPE_INT32, 17060, 200);

    /* @Conv_Conv_7_14:weight */
    attr.size[0] = 1;
    attr.size[1] = 1;
    attr.size[2] = 50;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.scale = 0.001773388241417706;
    attr.dtype.zero_point = 103;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[6], attr, VSI_NN_TYPE_UINT8, 18460, 2500);

    /* @Conv_Conv_7_14:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.scale = 2.361995559459543e-05;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[7], attr, VSI_NN_TYPE_INT32, 18260, 200);

    /* @Conv_Conv_9_12:weight */
    attr.size[0] = 1;
    attr.size[1] = 1;
    attr.size[2] = 50;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.scale = 0.0018922025337815285;
    attr.dtype.zero_point = 98;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[8], attr, VSI_NN_TYPE_UINT8, 21160, 2500);

    /* @Conv_Conv_9_12:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.scale = 3.654186269701036e-05;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[9], attr, VSI_NN_TYPE_INT32, 20960, 200);

    /* @Conv_Conv_11_10:weight */
    attr.size[0] = 1;
    attr.size[1] = 1;
    attr.size[2] = 50;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.scale = 0.0021649564150720835;
    attr.dtype.zero_point = 82;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[10], attr, VSI_NN_TYPE_UINT8, 1780, 2500);

    /* @Conv_Conv_11_10:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.scale = 8.613074552974841e-05;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[11], attr, VSI_NN_TYPE_INT32, 1580, 200);

    /* @Conv_Conv_13_8:weight */
    attr.size[0] = 1;
    attr.size[1] = 1;
    attr.size[2] = 50;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.scale = 0.0021818666718900204;
    attr.dtype.zero_point = 89;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[12], attr, VSI_NN_TYPE_UINT8, 4480, 2500);

    /* @Conv_Conv_13_8:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.scale = 0.00013251128786338773;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[13], attr, VSI_NN_TYPE_INT32, 4280, 200);

    /* @Gemm_Gemm_19_4:weight */
    attr.size[0] = 800;
    attr.size[1] = 500;
    attr.dim_num = 2;
    attr.dtype.scale = 0.0009464719332754612;
    attr.dtype.zero_point = 130;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[14], attr, VSI_NN_TYPE_UINT8, 25660, 400000);

    /* @Gemm_Gemm_19_4:bias */
    attr.size[0] = 500;
    attr.dim_num = 1;
    attr.dtype.scale = 0.00010556137427150669;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[15], attr, VSI_NN_TYPE_INT32, 23660, 2000);

    /* @Gemm_Gemm_21_2:weight */
    attr.size[0] = 500;
    attr.size[1] = 10;
    attr.dim_num = 2;
    attr.dtype.scale = 0.0013942949008196592;
    attr.dtype.zero_point = 138;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[16], attr, VSI_NN_TYPE_UINT8, 425700, 5000);

    /* @Gemm_Gemm_21_2:bias */
    attr.size[0] = 10;
    attr.dim_num = 1;
    attr.dtype.scale = 4.959440388794655e-05;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_CONST_TENSOR(const_tensor[17], attr, VSI_NN_TYPE_INT32, 425660, 40);



    /* @Conv_Conv_0_21:out0 */
    attr.dtype.scale = 0.007552491035312414;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[0]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_1_20:out0 */
    attr.dtype.scale = 0.007552491035312414;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[1]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @MaxPool_MaxPool_2_19:out0 */
    attr.dtype.scale = 0.007552491035312414;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[2]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_3_18:out0 */
    attr.dtype.scale = 0.013250074349343777;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[3]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_4_17:out0 */
    attr.dtype.scale = 0.013250074349343777;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[4]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_5_16:out0 */
    attr.dtype.scale = 0.013319111429154873;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[5]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_6_15:out0 */
    attr.dtype.scale = 0.013319111429154873;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[6]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_7_14:out0 */
    attr.dtype.scale = 0.01931181363761425;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[7]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_8_13:out0 */
    attr.dtype.scale = 0.01931181363761425;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[8]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_9_12:out0 */
    attr.dtype.scale = 0.03978405520319939;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[9]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_10_11:out0 */
    attr.dtype.scale = 0.03978405520319939;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[10]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_11_10:out0 */
    attr.dtype.scale = 0.06073299050331116;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[11]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_12_9:out0 */
    attr.dtype.scale = 0.06073299050331116;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[12]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Conv_Conv_13_8:out0 */
    attr.dtype.scale = 0.11153143644332886;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[13]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_14_7:out0 */
    attr.dtype.scale = 0.11153143644332886;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[14]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @MaxPool_MaxPool_15_6:out0 */
    attr.dtype.scale = 0.11153143644332886;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[15]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Reshape_Reshape_18_5:out0 */
    attr.dtype.scale = 0.11153143644332886;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[16]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Gemm_Gemm_19_4:out0 */
    attr.dtype.scale = 0.03556952252984047;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[17]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Relu_Relu_20_3:out0 */
    attr.dtype.scale = 0.03556952252984047;
    attr.dtype.zero_point = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[18]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);

    /* @Gemm_Gemm_21_2:out0 */
    attr.dtype.scale = 0.08149214833974838;
    attr.dtype.zero_point = 95;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_AFFINE_ASYMMETRIC;
    NEW_VIRTUAL_TENSOR(node[19]->output.tensors[0], attr, VSI_NN_TYPE_UINT8);



/*-----------------------------------------
  Connection initialize
 -----------------------------------------*/
    node[0]->input.tensors[0] = norm_tensor[1];
    node[20]->output.tensors[0] = norm_tensor[0];

    /* Conv_Conv_0_21 */
    node[0]->input.tensors[1] = const_tensor[0]; /* data_weight */
    node[0]->input.tensors[2] = const_tensor[1]; /* data_bias */

    /* Relu_Relu_1_20 */
    node[1]->input.tensors[0] = node[0]->output.tensors[0];

    /* MaxPool_MaxPool_2_19 */
    node[2]->input.tensors[0] = node[1]->output.tensors[0];

    /* Conv_Conv_3_18 */
    node[3]->input.tensors[0] = node[2]->output.tensors[0];
    node[3]->input.tensors[1] = const_tensor[2]; /* data_weight */
    node[3]->input.tensors[2] = const_tensor[3]; /* data_bias */

    /* Relu_Relu_4_17 */
    node[4]->input.tensors[0] = node[3]->output.tensors[0];

    /* Conv_Conv_5_16 */
    node[5]->input.tensors[0] = node[4]->output.tensors[0];
    node[5]->input.tensors[1] = const_tensor[4]; /* data_weight */
    node[5]->input.tensors[2] = const_tensor[5]; /* data_bias */

    /* Relu_Relu_6_15 */
    node[6]->input.tensors[0] = node[5]->output.tensors[0];

    /* Conv_Conv_7_14 */
    node[7]->input.tensors[0] = node[6]->output.tensors[0];
    node[7]->input.tensors[1] = const_tensor[6]; /* data_weight */
    node[7]->input.tensors[2] = const_tensor[7]; /* data_bias */

    /* Relu_Relu_8_13 */
    node[8]->input.tensors[0] = node[7]->output.tensors[0];

    /* Conv_Conv_9_12 */
    node[9]->input.tensors[0] = node[8]->output.tensors[0];
    node[9]->input.tensors[1] = const_tensor[8]; /* data_weight */
    node[9]->input.tensors[2] = const_tensor[9]; /* data_bias */

    /* Relu_Relu_10_11 */
    node[10]->input.tensors[0] = node[9]->output.tensors[0];

    /* Conv_Conv_11_10 */
    node[11]->input.tensors[0] = node[10]->output.tensors[0];
    node[11]->input.tensors[1] = const_tensor[10]; /* data_weight */
    node[11]->input.tensors[2] = const_tensor[11]; /* data_bias */

    /* Relu_Relu_12_9 */
    node[12]->input.tensors[0] = node[11]->output.tensors[0];

    /* Conv_Conv_13_8 */
    node[13]->input.tensors[0] = node[12]->output.tensors[0];
    node[13]->input.tensors[1] = const_tensor[12]; /* data_weight */
    node[13]->input.tensors[2] = const_tensor[13]; /* data_bias */

    /* Relu_Relu_14_7 */
    node[14]->input.tensors[0] = node[13]->output.tensors[0];

    /* MaxPool_MaxPool_15_6 */
    node[15]->input.tensors[0] = node[14]->output.tensors[0];

    /* Reshape_Reshape_18_5 */
    node[16]->input.tensors[0] = node[15]->output.tensors[0];

    /* Gemm_Gemm_19_4 */
    node[17]->input.tensors[0] = node[16]->output.tensors[0];
    node[17]->input.tensors[1] = const_tensor[14]; /* data_weight */
    node[17]->input.tensors[2] = const_tensor[15]; /* data_bias */

    /* Relu_Relu_20_3 */
    node[18]->input.tensors[0] = node[17]->output.tensors[0];

    /* Gemm_Gemm_21_2 */
    node[19]->input.tensors[0] = node[18]->output.tensors[0];
    node[19]->input.tensors[1] = const_tensor[16]; /* data_weight */
    node[19]->input.tensors[2] = const_tensor[17]; /* data_bias */

    /* LogSoftmax_LogSoftmax_22_1 */
    node[20]->input.tensors[0] = node[19]->output.tensors[0];


    graph->output.tensors[0] = norm_tensor[0];
    graph->input.tensors[0] = norm_tensor[1];


    if( enable_pre_post_process )
    {
        sort = TRUE;
        if( pre_process_map_count > 0 )
        {
            for( i = 0; i < pre_process_map_count; i++ )
            {
                status = vsi_nn_AddGraphPreProcess(graph, pre_process_map[i].graph_input_idx,
                                                   pre_process_map[i].preprocesses,
                                                   pre_process_map[i].preprocess_count);
                TEST_CHECK_STATUS( status, error );
            }
        }

        if( post_process_map_count > 0 )
        {
            for( i = 0; i < post_process_map_count; i++ )
            {
                 status = vsi_nn_AddGraphPostProcess(graph, post_process_map[i].graph_output_idx,
                                                     post_process_map[i].postprocesses,
                                                     post_process_map[i].postprocess_count);
                 TEST_CHECK_STATUS( status, error );
            }
        }
    }

    status = vsi_nn_SetupGraph( graph, sort );
    TEST_CHECK_STATUS( status, error );


    if( VSI_FAILURE == status )
    {
        goto error;
    }

    fclose( fp );

    return graph;

error:
    if( NULL != fp )
    {
        fclose( fp );
    }

    release_ctx = ( NULL == in_ctx );
    vsi_nn_DumpGraphToJson( graph );
    vnn_ReleaseMnist( graph, release_ctx );

    return NULL;
} /* vsi_nn_CreateMnist() */

void vnn_ReleaseMnist
    (
    vsi_nn_graph_t * graph,
    vsi_bool release_ctx
    )
{
    vsi_nn_context_t ctx;
    if( NULL != graph )
    {
        ctx = graph->ctx;
        vsi_nn_ReleaseGraph( &graph );

        /*-----------------------------------------
        Unregister client ops
        -----------------------------------------*/
        

        if( release_ctx )
        {
            vsi_nn_ReleaseContext( &ctx );
        }
    }
} /* vsi_nn_ReleaseMnist() */

