## About
This is the second step of the conversion.
Here you must copy the export onnx (lenet.onnx)
from the previous step to the folder where the convert binary
of the aml sdk is located; you also need to copy the
convert-lenet-onnx-to-khadas.sh to this path and the data folder.
The data folder contains sample images used for the quantisation.

## Options
You can modify the conversion script to use uint8 instead of int16.
Then the speed will be doubled but the performance will be bad.

## Prerequisites
* install docker
* make sure you have the following files in the folder "network"
  * lenet.onnx
  * convert-lenet-onnx-to-khadas.sh
* make sure you have an empty outputs directory

## Run the conversion process

* run the conversion process as follows (WINDOWS)
  * go to the path 2_convert_network and use the following command (WINDOWS)
    * 
## Output
If the conversion was successful the output should be in outputs/lenet:
```
total 732
-rw-r--r-- 1 root root 584913 Mar 18 14:11 lenet.nb
-rwxr-xr-x 1 root root 163768 Mar 18 14:12 libnn_lenet.so
```
These two files must now be copied over to the khadas board
to test the functionality. See in the next step.

