if [ $# == 2 ]; then
    g++ --std=c++17 $1 -ljunction -lturf -o $2
else
    g++ --std=c++17 $1 -ljunction -lturf
fi