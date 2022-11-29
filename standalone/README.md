# README #

Standalone Detection Code

### What is this repository for? ###

* Combined **source_code**, **detect_yolo_v3** and **sample_demo_x11** into single application to avoid **libnn_yolo_V3** and **libnn_detect**
* and similar procedure for detect_yolo_v3_tiny

### Index ###
###### 1. newscripts
* using the newer conversion scripts
* worked
* also work when new c/h and .nb files are copied from sdk converted models.
###### 1.1. first_working
* cleaned and initial versions for both yolov3 and yolov3tiny
###### 1.1.1 sample_demo_x11_yolov3
* stand-alone application for yolov3, we need to copy c/h and .nb files here after sdk model conversion.
###### 1.1.2 sample_demo_x11_tiny
* stand-alone application for yolov3tiny, we need to copy c/h and .nb files here after sdk model conversion.
###### 1.1.3 some logs
* differences and steps to acheive this stand-alone applications
###### 1.2. sample_demo_x11_yolov3
* with additional debugging statements for yolov3

###### 2. oldscripts
* using old sdk conversion scripts
* worked
* but did not work when new c/h and .nb files are copied from sdk converted models.



###   Details:    Standalone Yolo-v3 Code
* Using detect_demo_x11, need following steps earlier:
    1. Should have libnn_detect.so from source_code folder
    2. Generate export files and .nb file and copy them to relevant directories.
    3. Should have libnn_yolo_V3.so from the detect_yolo_v3 folder
    4. Copy libnn_yolo_v3.so and .nd to demo_detect_picture folder and do ./INSTALL
* Now:
    * Just copy export files and .nb file into the new stand-alone folder and compile it, and use it.
* For this, I did the following:
    1. Modified some of the c files
    2. Changed the Makefile to work successfully
    3. Used build.sh to compile the code
    4. Quick summary Avoiding libnn_yolo_v3.so and libnn_detect.so combining the code from: detect_yolo_v3 (libnn_yolo_v3.so), source_code (code for libnn_detect.so) , and sample_demo_x11 (actual code for detection)
    5. `sample_demo_x11B/bin_r_cv4$ ./detect_demo_x11 -m 2 -p ../1080p.bmp`
    6. images are saved in the img folder for different sizes
    7. we can use -w 416 -h 416 paramters for lesser sizes.


