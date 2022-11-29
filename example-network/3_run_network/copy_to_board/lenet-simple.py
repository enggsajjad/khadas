import numpy as np
import os
import urllib.request
from matplotlib import gridspec
from matplotlib import pyplot as plt
from PIL import Image
import argparse
import sys
import math
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





    lenet = KSNN('VIM3')
    print(' |--- KSNN Version: {} +---| '.format(lenet.get_nn_version()))

    print('Start init neural network ...')
    lenet.nn_init(library=library, model=model, level=level)
    print('Done.')

    print('Get input data ...')
    img_raw = cv.imread(picture, cv.IMREAD_COLOR)
    img = [ img_raw  ]
    

    print('Done.')
    
    print('Start inference ...')
    start = time.time()

    '''
        default input_tensor is 1
        default output_tensor is 1
    '''
    output = lenet.nn_inference(img, platform = 'ONNX', output_tensor=1, output_format=output_format.OUT_FORMAT_FLOAT32)
    end = time.time()
    print('Done. inference time: ', end - start)

    print("Output:", output)
    print(np.max(output))
    print("Predicted class is:" , np.argmax( output[0] ))

    
