#cc -fPIC -shared -o dataset_controller.so dataset_controller.c
cc -fPIC -shared -o neural_network.so neural_network.c
python3 util.py