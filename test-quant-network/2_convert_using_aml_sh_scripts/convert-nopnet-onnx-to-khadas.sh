#!/bin/bash
CONVERT_PATH="/acuity-toolkit/demo/"
cd $CONVERT_PATH
bash 0_import_model.sh && bash 1_quantize_model.sh && bash 2_export_case_code.sh && bash 3_change_makefile.sh
