## About
In this folder are the files which need to be copied to
the khadas board in order to run the converted network on khadas board.

## Prepare
* copy the lenet.nb and libnn_lenet.so from the previous step to the folder copy_to_board/model
* if not done already: install ksnn on the khadas board, ie.
  * git clone https://github.com/khadas/ksnn
  *  pip3 install ksnn/ksnn-1.2-py3-none-any.whl
*  transfer the folder "copy_to_board" to the khadas board

## Run the network
* ssh into the board
* cd to the "copy_to_board" folder
* run bash lenet_simple.sh
