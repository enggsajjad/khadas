#!/bin/bash

echo "-------------------- CHANGE MAKEFILE"

name_nbg_folder=$(ls -1|grep nbg_unify)
name=${name_nbg_folder:10}
echo "TARGET_NAME = $name" > makefiles/makefile.target_name
cp makefiles/* $name_nbg_folder/