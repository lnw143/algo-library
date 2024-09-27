for ((i=1; i<=100000; ++i))
do
./data > .in
./tester < .in > .out
python -u checker.py < .in > .ans
diff .out .ans
if (($?!=0)); then break; fi
done