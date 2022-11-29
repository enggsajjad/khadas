README
This README would normally document whatever steps are necessary to get your application up and running.

What is this repository for?
Example to get own network running on khadas board
Pipeline for creating traffic detection network on khadas
Index
example-network ... Example for creating an on network for Khadas



README.md
Example for creating an on network for Khadas
This folder contains an example how to create, export and run a pyTorch network on the khadas board

Structure of this example
1_create_and_export_network
a juypter notebook which uses pyTorch to train a lenet and export it
this should be run on an amd64 linux or windows computer / docker
2_convert_network
a simple shell script and example exported network to be converted to khadas .nb format also images used for quantization
this should be run an amd64 linux host/docker which has the aml_npu_sdk installed and the convert executable
3_run_network
this contains a python script / demo data and the converted example network and shell script running the demo
it should be run on a khadas board




README.md
About
Jupyter notebook which exemplifies the creation and export of a pyTorch model to onnx for conversion to the khadas board.

How how to run
use python3.9 and above / tested on windows 10
initialize virtual env
python -m virtualenv venv
activate in windows: venv\Scripts\activate.bat
install requirements in virtual environment
pip install -r requirements.txt
NOTE: if you want to have pyTorch with CUDA use requirements_cuda.txt
pip install -r requirements_cuda.txt (it has the added extra index line)
create link to install venv kernel
python -m ipykernel install --user --name py39-teconet
run jupyter notebook
jupyter notebook
in jupyter open the file "pytorch_to_khadas_.."
after opening select the correct kernel under Kernel->Change Kernel->py39-teconet



README.md
About
In this folder are the files which need to be copied to the khadas board in order to run the converted network on khadas board.

Prepare
copy the lenet.nb and libnn_lenet.so from the previous step to the folder copy_to_board/model
if not done already: install ksnn on the khadas board, ie.
git clone https://github.com/khadas/ksnn
pip3 install ksnn/ksnn-1.2-py3-none-any.whl
transfer the folder "copy_to_board" to the khadas board
Run the network
ssh into the board
cd to the "copy_to_board" folder
run bash lenet_simple.sh


README.md
About
In this folder are the files which need to be copied to the khadas board in order to run the converted network on khadas board.

Prepare
copy the lenet.nb and libnn_lenet.so from the previous step to the folder copy_to_board/model
if not done already: install ksnn on the khadas board, ie.
git clone https://github.com/khadas/ksnn
pip3 install ksnn/ksnn-1.2-py3-none-any.whl
transfer the folder "copy_to_board" to the khadas board
Run the network
ssh into the board
cd to the "copy_to_board" folder
run bash lenet_simple.sh
