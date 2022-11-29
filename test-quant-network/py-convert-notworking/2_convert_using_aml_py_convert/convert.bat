docker run --rm --name conv-test -it ^
 --mount type=bind,source=%CD%\data,target=/acuity-toolkit/python/data ^
 --mount type=bind,source=%CD%\network,target=/acuity-toolkit/python/network ^
  --mount type=bind,source=%CD%\outputs,target=/acuity-toolkit/python/outputs ^
 --entrypoint=/acuity-toolkit/python/network/convert-nopnet-onnx-to-khadas.sh ^
  ghcr.io/scholz/aml-container:0.0.1
