for ((i=1; i<=100000; ++i))
do
./data > .in
echo running python
time python -u checker.py < .in > .ans
echo running c++
time ./tester < .in > .out
diff .out .ans
if (($?!=0)); then break; fi
done