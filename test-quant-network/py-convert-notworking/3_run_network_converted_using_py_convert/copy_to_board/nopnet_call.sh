for i in bmp/0.bmp;
do
echo "Testing: $i"
    python3 nopnet.py --model ./model/nopnet.nb --library ./model/libnn_nopnet.so --picture $i --level 0
done;
