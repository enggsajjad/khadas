docker run --rm --name conv-test -it ^
 --mount type=bind,source=%CD%,target=/acuity-toolkit/demo ^
  --entrypoint=/acuity-toolkit/demo/convert-nopnet-onnx-to-khadas.sh ^
  ghcr.io/scholz/aml-container:0.0.1
