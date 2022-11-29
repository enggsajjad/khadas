import numpy as np
import os
import argparse
import sys
from ksnn.api import KSNN
from ksnn.types import *
import cv2 as cv
import time
import sys



if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--library", help="Path to C static library file")
    parser.add_argument("--model", help="Path to nbg file")
    parser.add_argument("--picture", help="Path to input picture")
    parser.add_argument("--level", help="Information printer level: 0/1/2")

    args = parser.parse_args()
    if args.model :
        if os.path.exists(args.model) == False:
            sys.exit('Model \'{}\' not exist'.format(args.model))
        model = args.model
    else :
        sys.exit("NBG file not found !!! Please use format: --model")
    if args.picture :
        if os.path.exists(args.picture) == False:
            sys.exit('Input picture \'{}\' not exist'.format(args.picture))
        picture = args.picture
    else :
        sys.exit("Input picture not found !!! Please use format: --picture")
    if args.library :
        if os.path.exists(args.library) == False:
            sys.exit('C static library \'{}\' not exist'.format(args.library))
        library = args.library
    else :
        sys.exit("C static library not found !!! Please use format: --library")
    if args.level == '1' or args.level == '2' :
        level = int(args.level)
    else :
        level = 0

   
    nopnet = KSNN('VIM3')

    print(' |--- KSNN Version: {} +---| '.format(nopnet.get_nn_version()))

    print('Start init neural network ...')
    nopnet.nn_init(library="model/libnn_nopnet.so", model="model/nopnet.nb", level=0)

    print('Done.')

    print('Get input data ...')
    img = cv.imread(picture, cv.IMREAD_COLOR).astype(np.int8) - 127

    ## modify HWC to CHW since someth

    #img = img.astype(float)
    #img = (img - 127.5) / 127.5
    print("before network")
    #print(img.flatten()[0:300])
    ## test
    img2 = img.transpose(2,0,1).flatten().astype(np.int8).astype(float)
    img2[0:30] = 2.
    print(img2[0:300])

    img_arr = [ img  ]


    print('Done.')

    print('Start inference ...')
    start = time.time()

    '''
        default input_tensor is 1
        default output_tensor is 1
    '''
    output = nopnet.nn_inference(img_arr, platform = 'ONNX', output_tensor=1, output_format=output_format.OUT_FORMAT_FLOAT32)
    end = time.time()

    print('Done. inference time: ', end - start)


    # display image
    # note that we do not need to multiply 255 this done automatically by py convert

    print(output[0][:300])
    #frame = np.array(output[0].tolist()).reshape(28,28,3)

