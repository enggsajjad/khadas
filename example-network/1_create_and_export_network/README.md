# About
Jupyter notebook which exemplifies the creation and export of a pyTorch model to onnx for conversion to the khadas board.

# How how to run
* use python3.9 64bit and above / tested on windows 10
* initialize virtual env
  * python -m virtualenv venv
  * activate in windows: venv\Scripts\activate.bat
* install requirements in virtual environment
  * pip install -r requirements.txt
  * NOTE: if you want to have pyTorch with CUDA use requirements_cuda.txt
    * pip install -r requirements_cuda.txt (it has the added extra index line)
* create link to install venv kernel
  * python -m ipykernel install --user --name py39-teconet
* run jupyter notebook
  * jupyter notebook
  * in jupyter open the file "pytorch_to_khadas_.."
  * after opening select the correct kernel under Kernel->Change Kernel->py39-teconet
  