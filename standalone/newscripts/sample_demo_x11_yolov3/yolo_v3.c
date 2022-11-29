#include "yolo_v3.h"
#include "vnn_yolov3.h"
#include "yolov3_process.h"
#include "vnn_pre_process.h"
#include "vnn_post_process.h"

vsi_nn_graph_t * g_graph = NULL;

const static vsi_nn_postprocess_map_element_t* postprocess_map = NULL;
const static vsi_nn_preprocess_map_element_t* preprocess_map = NULL;

const vsi_nn_preprocess_map_element_t * vnn_GetPrePorcessMap()
{
	return preprocess_map;
}

uint32_t vnn_GetPrePorcessMapCount()
{
	if (preprocess_map == NULL)
		return 0;
	else
		return sizeof(preprocess_map) / sizeof(vsi_nn_preprocess_map_element_t);
}

const vsi_nn_postprocess_map_element_t * vnn_GetPostPorcessMap()
{
	return postprocess_map;
}

uint32_t vnn_GetPostPorcessMapCount()
{
	if (postprocess_map == NULL)
		return 0;
	else
		return sizeof(postprocess_map) / sizeof(vsi_nn_postprocess_map_element_t);
}
     

det_status_t model_create(const char * data_file_path, dev_type type)
{
	det_status_t ret = DET_STATUS_ERROR;
	vsi_status status = VSI_SUCCESS;
	char model_path[48];
	MY_DBG("model_create1\n"); //Sajjad
	switch (type) {
		case DEV_REVA:
			sprintf(model_path, "%s%s", data_file_path, "/yolov3_7d.nb");
			MY_DBG("model_create1.1\n"); //Sajjad
			break;
		case DEV_REVB:
			sprintf(model_path, "%s%s", data_file_path, "/yolov3_88.nb");
			MY_DBG("model_create1.2\n"); //Sajjad
			break;
		case DEV_MS1:
			sprintf(model_path, "%s%s", data_file_path, "/yolov3_99.nb");
			MY_DBG("model_create1.3\n"); //Sajjad
			break;
		default:
			break;
	}
	MY_DBG("model_create2\n"); //Sajjad

	g_graph = vnn_CreateYolov3(model_path, NULL,
			vnn_GetPrePorcessMap(), vnn_GetPrePorcessMapCount(),
			vnn_GetPostPorcessMap(), vnn_GetPostPorcessMapCount());
	TEST_CHECK_PTR(g_graph, exit);
	MY_DBG("model_create3\n"); //Sajjad

	status = vsi_nn_VerifyGraph(g_graph);
	MY_DBG("model_create4\n"); //Sajjad

	TEST_CHECK_STATUS(status, exit);
	MY_DBG("model_create5\n"); //Sajjad

	ret = DET_STATUS_OK;
exit:
	return ret;
}
vsi_nn_graph_t * model_create_nbg(const char * data_file_path, dev_type type)
{
        det_status_t ret = DET_STATUS_ERROR;
        vsi_status status = VSI_SUCCESS;
        char model_path[48];
        MY_DBG("model_create1\n"); //Sajjad
	vsi_nn_graph_t * g_graph1;
        switch (type) {
                case DEV_REVA:
                        sprintf(model_path, "%s%s", data_file_path, "/yolov3_7d.nb");
                        MY_DBG("model_create1.1\n"); //Sajjad
                        break;
                case DEV_REVB:
                        sprintf(model_path, "%s%s", data_file_path, "/yolov3_88.nb");
                        MY_DBG("model_create1.2\n"); //Sajjad
                        break;
                case DEV_MS1:
                        sprintf(model_path, "%s%s", data_file_path, "/yolov3_99.nb");
                        MY_DBG("model_create1.3\n"); //Sajjad
                        break;
                default:
                        break;
        }
        MY_DBG("model_create2\n"); //Sajjad
	MY_DBG("model_path: %s\n", model_path); //Sajjad
        g_graph1 = vnn_CreateYolov3(model_path, NULL,
                        vnn_GetPrePorcessMap(), vnn_GetPrePorcessMapCount(),
                        vnn_GetPostPorcessMap(), vnn_GetPostPorcessMapCount());
	g_graph = g_graph1;

        TEST_CHECK_PTR(g_graph1, exit);
        MY_DBG("model_create3\n"); //Sajjad

        status = vsi_nn_VerifyGraph(g_graph1);
        MY_DBG("model_create4\n"); //Sajjad

        TEST_CHECK_STATUS(status, exit);
        MY_DBG("model_create5\n"); //Sajjad

        ret = DET_STATUS_OK;
exit:
        //return ret;
	return g_graph1;
}

void model_getsize(int *width, int *height, int *channel)
{
	if (g_graph) {
		vsi_nn_tensor_t *tensor = NULL;
		tensor = vsi_nn_GetTensor(g_graph, g_graph->input.tensors[0] );

		*width = tensor->attr.size[0];
		*height = tensor->attr.size[1];
		*channel = tensor->attr.size[2];
	}
}
void model_getsize_nbg(int *width, int *height, int *channel,vsi_nn_graph_t * g_graph)
{
        if (g_graph) {
                vsi_nn_tensor_t *tensor = NULL;
                tensor = vsi_nn_GetTensor(g_graph, g_graph->input.tensors[0] );

                *width = tensor->attr.size[0];
                *height = tensor->attr.size[1];
                *channel = tensor->attr.size[2];
        }
}


void model_setinput(input_image_t imageData, uint8_t* data)
{
	yolov3_preprocess(imageData, data);
}
void model_setinput_nbg(input_image_t imageData, uint8_t* data,vsi_nn_graph_t *graph)
{
        yolov3_preprocess_nbg(imageData, data,graph);
}

////Sajjad
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
}

static vsi_bool get_top
    (
    float *pfProb,
    float *pfMaxProb,
    vsi_size_t *pMaxClass,
    vsi_size_t outputCount,
    vsi_size_t topNum
    )
{
    vsi_size_t i, j, k;

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
    vsi_size_t i,sz,stride;
    float *buffer = NULL;
    uint8_t *tensor_data = NULL;
    vsi_size_t MaxClass[5];
    float fMaxProb[5];
    vsi_size_t topk = 5;

    sz = 1;
    for(i = 0; i < tensor->attr.dim_num; i++)
    {
        sz *= tensor->attr.size[i];
    }

    if(topk > sz)
        topk = sz;

    stride = (vsi_size_t)vsi_nn_TypeGetBytes(tensor->attr.dtype.vx_type);
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

vsi_status vnn_PostProcessYolov3(vsi_nn_graph_t *graph)
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
////Sajjad End
det_status_t model_getresult(pDetResult resultData, uint8_t* data)
{
	vsi_status status = VSI_FAILURE;
	det_status_t ret = DET_STATUS_ERROR;
	vsi_nn_tensor_t *tensor = vsi_nn_GetTensor(g_graph, g_graph->input.tensors[0]);

	status = vsi_nn_CopyDataToTensor(g_graph, tensor, data);
	TEST_CHECK_STATUS(status, exit);

	status = vsi_nn_RunGraph(g_graph);
	TEST_CHECK_STATUS(status, exit);

	yolov3_postprocess(g_graph, resultData);
	///Sajjad
	//vnn_PostProcessYolov3(g_graph);
	///Sajjad End
	ret = DET_STATUS_OK;
exit:
	return ret;
}
det_status_t model_getresult_nbg(pDetResult resultData, uint8_t* data,vsi_nn_graph_t *g_graph1)
{
        vsi_status status = VSI_FAILURE;
        det_status_t ret = DET_STATUS_ERROR;
        vsi_nn_tensor_t *tensor = vsi_nn_GetTensor(g_graph1, g_graph1->input.tensors[0]);

        status = vsi_nn_CopyDataToTensor(g_graph1, tensor, data);
        TEST_CHECK_STATUS(status, exit);

        status = vsi_nn_RunGraph(g_graph1);
        TEST_CHECK_STATUS(status, exit);

        yolov3_postprocess(g_graph1, resultData);
        ///Sajjad
        //vnn_PostProcessYolov3(g_graph);
        ///Sajjad End
        ret = DET_STATUS_OK;
exit:
        return ret;
}

void model_release(dev_type type)
{
	vnn_ReleaseYolov3(g_graph, TRUE);
	g_graph = NULL;
}
void model_release_nbg(dev_type type,vsi_nn_graph_t *g_graph1)
{
        vnn_ReleaseYolov3(g_graph1, TRUE);
        g_graph1 = NULL;
}

void myprint(void)
{
	MY_DBG("Hello from yolo_v3.c\n");
}
