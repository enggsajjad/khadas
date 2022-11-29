# Example for creating an on network for Khadas #

This folder contains an example how to create, export and run a pyTorch network on the khadas board

### Structure of this example ###
* 1_create_and_export_network
    * a juypter notebook which uses pyTorch to train a lenet and export it
    * this should be run on an amd64 linux or windows computer / docker
* 2_convert_network
    * a simple shell script and example exported network to be converted to khadas .nb format also images used for quantization
    * this should be run an amd64 linux host/docker which has the aml_npu_sdk installed and the convert executable
* 3_run_network
    * this contains a python script / demo data and the converted example network and shell script running the demo
    * it should be run on a khadas board

