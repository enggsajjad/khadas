for i in bmp/*.bmp;
do
echo "Testing: $i"
res=$(python3 lenet-simple.py --model ./model/lenet.nb --library ./model/libnn_lenet.so --picture $i --level 0 | grep -i Predicted)
echo $res
done;
