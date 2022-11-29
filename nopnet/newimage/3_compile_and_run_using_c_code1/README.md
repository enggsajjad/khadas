## About
This is the source code to run and verify the input and output for the quantized network

## Howto run

1. Copy nopnet.nb file from conversion or use the committed one
2. scp this folder to khadas board
4. compile using ```bash build_vx.sh```
5. run using
   1. ```./bin_r_cv4/nopnet nopnet.nb 28x28.jpg``` and
   2. ``` ./bin_r_cv4/nopnet nopnet.nb black.jpg```
6. Observe that the input always has 127 added to the input then everything is working (the network is simply adding this value)