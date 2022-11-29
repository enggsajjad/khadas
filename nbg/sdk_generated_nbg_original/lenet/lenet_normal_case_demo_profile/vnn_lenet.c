/****************************************************************************
*   Generated by ACUITY 6.0.12
*   Match ovxlib 1.1.34
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
#include "vnn_lenet.h"

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
        memset( _attr.size, 0, VSI_NN_MAX_DIM_NUM * sizeof(vsi_size_t));\
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

#define NET_NODE_NUM            (13)
#define NET_NORM_TENSOR_NUM     (2)
#define NET_CONST_TENSOR_NUM    (9)
#define NET_VIRTUAL_TENSOR_NUM  (13)
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

vsi_nn_graph_t * vnn_CreateLenet
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
    vsi_bool                inference_with_nbg = FALSE;
    char*                   pos = NULL;

    uint32_t   shape_1[] = { 28, 28, 1, -1 };
    uint32_t   shape_2[] = { 800, -1 };




    (void)(_infinity);
    ctx = NULL;
    graph = NULL;
    status = VSI_FAILURE;
    memset( &attr, 0, sizeof( attr ) );
    memset( &node, 0, sizeof( vsi_nn_node_t * ) * NET_NODE_NUM );

    fp = fopen( data_file_name, "rb" );
    if( NULL == fp )
    {
        VSILOGE( "Open file %s failed.", data_file_name );
        goto error;
    }

    pos = strstr(data_file_name, ".nb");
    if( pos && strcmp(pos, ".nb") == 0 )
    {
        inference_with_nbg = TRUE;
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
    if( !inference_with_nbg )
    {

    /*-----------------------------------------
      lid       - Constant_Cast_4_27_as_const_14
      var       - node[0]
      name      - Constant_Cast_4_27_as_const
      operation - variable
      input     - 
      output    - [1]
    -----------------------------------------*/
    NEW_VXNODE(node[0], VSI_NN_OP_VARIABLE, 1, 1, 14);

    /*-----------------------------------------
      lid       - Gather_Gather_5_12
      var       - node[1]
      name      - Gather_Gather_5
      operation - gather
      input     - [28, 28, 3, 1]
                  [1]
      output    - [28, 28, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[1], VSI_NN_OP_GATHER, 2, 1, 12);
    node[1]->nn_param.gather.axis = 2;

    /*-----------------------------------------
      lid       - Reshape_Reshape_8_11
      var       - node[2]
      name      - Reshape_Reshape_8
      operation - reshape
      input     - [28, 28, 1]
      output    - [28, 28, 1, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[2], VSI_NN_OP_RESHAPE, 1, 1, 11);
    node[2]->nn_param.reshape.size = shape_1;
    node[2]->nn_param.reshape.dim_num = 4;

    /*-----------------------------------------
      lid       - Conv_Conv_9_10
      var       - node[3]
      name      - Conv_Conv_9
      operation - convolution
      input     - [28, 28, 1, 1]
      filter    - [5, 5, 1, 20]
      output    - [24, 24, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[3], VSI_NN_OP_CONV2D, 3, 1, 10);
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
    node[3]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
    node[3]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_10_9
      var       - node[4]
      name      - Relu_Relu_10
      operation - relu
      input     - [24, 24, 20, 1]
      output    - [24, 24, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[4], VSI_NN_OP_RELU, 1, 1, 9);

    /*-----------------------------------------
      lid       - MaxPool_MaxPool_11_8
      var       - node[5]
      name      - MaxPool_MaxPool_11
      operation - pooling
      input     - [24, 24, 20, 1]
      output    - [12, 12, 20, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[5], VSI_NN_OP_POOL, 1, 1, 8);
    node[5]->nn_param.pool.ksize[0] = 2;
    node[5]->nn_param.pool.ksize[1] = 2;
    node[5]->nn_param.pool.stride[0] = 2;
    node[5]->nn_param.pool.stride[1] = 2;
    node[5]->nn_param.pool.pad[0] = 0;
    node[5]->nn_param.pool.pad[1] = 0;
    node[5]->nn_param.pool.pad[2] = 0;
    node[5]->nn_param.pool.pad[3] = 0;
    node[5]->nn_param.pool.type = VX_CONVOLUTIONAL_NETWORK_POOLING_MAX;
    node[5]->nn_param.pool.round_type = VSI_NN_ROUND_FLOOR;
    node[5]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Conv_Conv_12_7
      var       - node[6]
      name      - Conv_Conv_12
      operation - convolution
      input     - [12, 12, 20, 1]
      filter    - [5, 5, 20, 50]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[6], VSI_NN_OP_CONV2D, 3, 1, 7);
    node[6]->nn_param.conv2d.ksize[0] = 5;
    node[6]->nn_param.conv2d.ksize[1] = 5;
    node[6]->nn_param.conv2d.weights = 50;
    node[6]->nn_param.conv2d.stride[0] = 1;
    node[6]->nn_param.conv2d.stride[1] = 1;
    node[6]->nn_param.conv2d.pad[0] = 0;
    node[6]->nn_param.conv2d.pad[1] = 0;
    node[6]->nn_param.conv2d.pad[2] = 0;
    node[6]->nn_param.conv2d.pad[3] = 0;
    node[6]->nn_param.conv2d.group = 1;
    node[6]->nn_param.conv2d.dilation[0] = 1;
    node[6]->nn_param.conv2d.dilation[1] = 1;
    node[6]->nn_param.conv2d.multiplier = 0;
    node[6]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[6]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
    node[6]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_13_6
      var       - node[7]
      name      - Relu_Relu_13
      operation - relu
      input     - [8, 8, 50, 1]
      output    - [8, 8, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[7], VSI_NN_OP_RELU, 1, 1, 6);

    /*-----------------------------------------
      lid       - MaxPool_MaxPool_14_5
      var       - node[8]
      name      - MaxPool_MaxPool_14
      operation - pooling
      input     - [8, 8, 50, 1]
      output    - [4, 4, 50, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[8], VSI_NN_OP_POOL, 1, 1, 5);
    node[8]->nn_param.pool.ksize[0] = 2;
    node[8]->nn_param.pool.ksize[1] = 2;
    node[8]->nn_param.pool.stride[0] = 2;
    node[8]->nn_param.pool.stride[1] = 2;
    node[8]->nn_param.pool.pad[0] = 0;
    node[8]->nn_param.pool.pad[1] = 0;
    node[8]->nn_param.pool.pad[2] = 0;
    node[8]->nn_param.pool.pad[3] = 0;
    node[8]->nn_param.pool.type = VX_CONVOLUTIONAL_NETWORK_POOLING_MAX;
    node[8]->nn_param.pool.round_type = VSI_NN_ROUND_FLOOR;
    node[8]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Reshape_Reshape_17_4
      var       - node[9]
      name      - Reshape_Reshape_17
      operation - reshape
      input     - [4, 4, 50, 1]
      output    - [800, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[9], VSI_NN_OP_RESHAPE, 1, 1, 4);
    node[9]->nn_param.reshape.size = shape_2;
    node[9]->nn_param.reshape.dim_num = 2;

    /*-----------------------------------------
      lid       - Gemm_Gemm_18_3
      var       - node[10]
      name      - Gemm_Gemm_18
      operation - fullconnect
      input     - [800, 1]
      filter    - [800, 500]
      output    - [500, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[10], VSI_NN_OP_FCL, 3, 1, 3);
    node[10]->nn_param.fcl.weights = 500;
    node[10]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[10]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
    node[10]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    /*-----------------------------------------
      lid       - Relu_Relu_19_2
      var       - node[11]
      name      - Relu_Relu_19
      operation - relu
      input     - [500, 1]
      output    - [500, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[11], VSI_NN_OP_RELU, 1, 1, 2);

    /*-----------------------------------------
      lid       - Gemm_Gemm_20_1
      var       - node[12]
      name      - Gemm_Gemm_20
      operation - fullconnect
      input     - [500, 1]
      filter    - [500, 10]
      output    - [10, 1]
    -----------------------------------------*/
    NEW_VXNODE(node[12], VSI_NN_OP_FCL, 3, 1, 1);
    node[12]->nn_param.fcl.weights = 10;
    node[12]->vx_param.overflow_policy = VX_CONVERT_POLICY_SATURATE;
    node[12]->vx_param.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
    node[12]->vx_param.down_scale_size_rounding = VX_CONVOLUTIONAL_NETWORK_DS_SIZE_ROUNDING_FLOOR;

    }
    else
    {
    NEW_VXNODE(node[0], VSI_NN_OP_NBG, 1, 1, 0);
    node[0]->nn_param.nbg.type = VSI_NN_NBG_FILE;
    node[0]->nn_param.nbg.url = data_file_name;

    }

/*-----------------------------------------
  Tensor initialize
 -----------------------------------------*/
    attr.dtype.fmt = VSI_NN_DIM_FMT_NCHW;
    /* @attach_Gemm_Gemm_20/out0_0:out0 */
    attr.size[0] = 10;
    attr.size[1] = 1;
    attr.dim_num = 2;
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_NORM_TENSOR(norm_tensor[0], attr, VSI_NN_TYPE_INT16);

    /* @input_15:out0 */
    attr.size[0] = 28;
    attr.size[1] = 28;
    attr.size[2] = 3;
    attr.size[3] = 1;
    attr.dim_num = 4;
    attr.dtype.fl = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_NORM_TENSOR(norm_tensor[1], attr, VSI_NN_TYPE_INT16);



    if( !inference_with_nbg )
    {
    /* @Constant_Cast_4_27_as_const_14:data */
    attr.size[0] = 1;
    attr.dim_num = 1;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_NONE;
    NEW_CONST_TENSOR(const_tensor[0], attr, VSI_NN_TYPE_INT32, 0, 4);

    /* @Conv_Conv_9_10:weight */
    attr.size[0] = 5;
    attr.size[1] = 5;
    attr.size[2] = 1;
    attr.size[3] = 20;
    attr.dim_num = 4;
    attr.dtype.fl = 16;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[1], attr, VSI_NN_TYPE_INT16, 50564, 1000);

    /* @Conv_Conv_9_10:bias */
    attr.size[0] = 20;
    attr.dim_num = 1;
    attr.dtype.fl = 16;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[2], attr, VSI_NN_TYPE_INT64, 50404, 160);

    /* @Conv_Conv_12_7:weight */
    attr.size[0] = 5;
    attr.size[1] = 5;
    attr.size[2] = 20;
    attr.size[3] = 50;
    attr.dim_num = 4;
    attr.dtype.fl = 17;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[3], attr, VSI_NN_TYPE_INT16, 404, 50000);

    /* @Conv_Conv_12_7:bias */
    attr.size[0] = 50;
    attr.dim_num = 1;
    attr.dtype.fl = 15;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[4], attr, VSI_NN_TYPE_INT64, 4, 400);

    /* @Gemm_Gemm_18_3:weight */
    attr.size[0] = 800;
    attr.size[1] = 500;
    attr.dim_num = 2;
    attr.dtype.fl = 18;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[5], attr, VSI_NN_TYPE_INT16, 55564, 800000);

    /* @Gemm_Gemm_18_3:bias */
    attr.size[0] = 500;
    attr.dim_num = 1;
    attr.dtype.fl = 16;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[6], attr, VSI_NN_TYPE_INT64, 51564, 4000);

    /* @Gemm_Gemm_20_1:weight */
    attr.size[0] = 500;
    attr.size[1] = 10;
    attr.dim_num = 2;
    attr.dtype.fl = 16;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[7], attr, VSI_NN_TYPE_INT16, 855644, 10000);

    /* @Gemm_Gemm_20_1:bias */
    attr.size[0] = 10;
    attr.dim_num = 1;
    attr.dtype.fl = 15;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_CONST_TENSOR(const_tensor[8], attr, VSI_NN_TYPE_INT64, 855564, 80);



    /* @Constant_Cast_4_27_as_const_14:out0 */
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_NONE;
    NEW_VIRTUAL_TENSOR(node[0]->output.tensors[0], attr, VSI_NN_TYPE_INT32);

    /* @Gather_Gather_5_12:out0 */
    attr.dtype.fl = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[1]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Reshape_Reshape_8_11:out0 */
    attr.dtype.fl = 0;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[2]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Conv_Conv_9_10:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[3]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Relu_Relu_10_9:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[4]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @MaxPool_MaxPool_11_8:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[5]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Conv_Conv_12_7:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[6]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Relu_Relu_13_6:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[7]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @MaxPool_MaxPool_14_5:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[8]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Reshape_Reshape_17_4:out0 */
    attr.dtype.fl = -2;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[9]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Gemm_Gemm_18_3:out0 */
    attr.dtype.fl = -1;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[10]->output.tensors[0], attr, VSI_NN_TYPE_INT16);

    /* @Relu_Relu_19_2:out0 */
    attr.dtype.fl = -1;
    attr.dtype.qnt_type = VSI_NN_QNT_TYPE_DFP;
    NEW_VIRTUAL_TENSOR(node[11]->output.tensors[0], attr, VSI_NN_TYPE_INT16);



/*-----------------------------------------
  Connection initialize
 -----------------------------------------*/
    node[1]->input.tensors[0] = norm_tensor[1];
    node[12]->output.tensors[0] = norm_tensor[0];

    /* Constant_Cast_4_27_as_const_14 */
    node[0]->input.tensors[0] = const_tensor[0]; /* data_data */

    /* Gather_Gather_5_12 */
    node[1]->input.tensors[1] = node[0]->output.tensors[0];

    /* Reshape_Reshape_8_11 */
    node[2]->input.tensors[0] = node[1]->output.tensors[0];

    /* Conv_Conv_9_10 */
    node[3]->input.tensors[0] = node[2]->output.tensors[0];
    node[3]->input.tensors[1] = const_tensor[1]; /* data_weight */
    node[3]->input.tensors[2] = const_tensor[2]; /* data_bias */

    /* Relu_Relu_10_9 */
    node[4]->input.tensors[0] = node[3]->output.tensors[0];

    /* MaxPool_MaxPool_11_8 */
    node[5]->input.tensors[0] = node[4]->output.tensors[0];

    /* Conv_Conv_12_7 */
    node[6]->input.tensors[0] = node[5]->output.tensors[0];
    node[6]->input.tensors[1] = const_tensor[3]; /* data_weight */
    node[6]->input.tensors[2] = const_tensor[4]; /* data_bias */

    /* Relu_Relu_13_6 */
    node[7]->input.tensors[0] = node[6]->output.tensors[0];

    /* MaxPool_MaxPool_14_5 */
    node[8]->input.tensors[0] = node[7]->output.tensors[0];

    /* Reshape_Reshape_17_4 */
    node[9]->input.tensors[0] = node[8]->output.tensors[0];

    /* Gemm_Gemm_18_3 */
    node[10]->input.tensors[0] = node[9]->output.tensors[0];
    node[10]->input.tensors[1] = const_tensor[5]; /* data_weight */
    node[10]->input.tensors[2] = const_tensor[6]; /* data_bias */

    /* Relu_Relu_19_2 */
    node[11]->input.tensors[0] = node[10]->output.tensors[0];

    /* Gemm_Gemm_20_1 */
    node[12]->input.tensors[0] = node[11]->output.tensors[0];
    node[12]->input.tensors[1] = const_tensor[7]; /* data_weight */
    node[12]->input.tensors[2] = const_tensor[8]; /* data_bias */


    }
    else
    {
    node[0]->output.tensors[0] = norm_tensor[0];
    node[0]->input.tensors[0] = norm_tensor[1];

    }
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
    vnn_ReleaseLenet( graph, release_ctx );

    return NULL;
} /* vsi_nn_CreateLenet() */

void vnn_ReleaseLenet
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
} /* vsi_nn_ReleaseLenet() */

