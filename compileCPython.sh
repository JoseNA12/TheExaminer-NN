cc -fPIC -shared -o neural_network_numbers.so neural_network_numbers.c
cc -fPIC -shared -o neural_network_letters.so neural_network_letters.c
python3 util.py