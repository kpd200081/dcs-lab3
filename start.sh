cd build*
[ -f report.txt ] && rm report.txt
./pr1 -n 1 &
sleep 1
./pr2 &
sleep 1
./pr3 &
sleep 1
./pr4 &

sleep 1m
pkill -int pr1
cd ../
echo "DONE"
