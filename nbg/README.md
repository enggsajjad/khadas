# README #

nbg/minimal code

### What is this repository for? ###

* nbg code generated and extended for various detection

### Index ###
###### 1. sdk_generated_nbg_original
* nbg code produced for yolov3, yolotiny, lenet, nopnet
###### 2. sdk_generated_nbg_compilable
* nbg code produced for yolov3, yolotiny, lenet, nopnet: extended by adding makefile and build_vx.sh to compile the code
###### 3. sdk_generated_nbg_detectable
* nbg code produced for yolov3, yolotiny: extended by adding makefile and build_vx.sh and adding yolov3 and/or yolotiny detection pre/post-processing c/h files to work it for detection with minimal code.
###### 4. log
* differences for various files to generate sdk_generated_nbg_compilable and sdk_generated_nbg_detectable code
* steps for the above

#       Details  #
######      Minimal /NBG Code  
* This is the code produced from SDK.
* Used the recent SDK with newer Khadas image
* Modified Makefile to compile it with build_vx.sh

###### nbg_unify_yolov3
* minimal example using the old conversion scripts
```
khadas@Khadas-teco:~/sajjad/minimal_nn_example/nbg_unify_yolov3$ ./bin_r_cv4/yolov3test ./yolov3.nb 1080p-608x608.jpg
```

###### nbg_unify_lenet
* minimal example using the old conversion scripts
```
khadas@Khadas-teco:~/sajjad/minimal_nn_example/nbg_unify_lenet$ ./bin_r_cv4/lenettest ./lenet.nb ./bmp/1.jpg
```

###### yolov3_nbg_unify_608_hussainA
* yolov3_nbg_unify minimal example using the new conversion scripts
* darknet cfg and weights are used from the website

```
khadas@Khadas-teco:~/sajjad/minimal_nn_example/yolov3_nbg_unify_608_hussainA/bin_r_cv4$ ./yolov3test ../yolov3_88.nb ../1080p-608x608.jpg
Create Neural Network: 2428ms or 2428619us 
Verify... Verify Graph: 11ms or 11375us 
Start run graph [1] times... 
Run the 1 time: 339.00ms or 339429.00us 
vxProcessGraph execution time: 
Total 339.00ms or 339496.00us 
Average 339.50ms or 339496.00us 
--- Top5 --- 
32638: 524288.000000 
1549: 491520.000000 
1763: 491520.000000 
50779: 491520.000000 
1742: 483328.000000
```

### yolov3_nbg_unify_416_hussainA 
* yolov3_nbg_unify minimal example using the new conversion scripts
* darknet cfg and weights are recommended by Frank (Khadas Community)
*

