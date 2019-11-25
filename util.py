import cv2, glob
from ctypes import *
#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2

def resize_img():
    height = 50
    width = 60
    image_path = "/home/josue/Documentos/Carrera/TEC/Inteligencia Artificial/TheExaminer-NN/images/"
    images = glob.glob(image_path+"*.jpeg")
    images.extend(glob.glob(image_path+'*.png'))

    for image in images:
        # Leer imagen y pasarla a BN indicando 0
        img=cv2.imread(image,0)

        # Variable donde quedará la imagen redimensionada
        re = cv2.resize(img,(height, width))

        # Revisar si la imagen ha sido redimensionada
        cv2.imshow("Checking",re)

        # Mostrar imgs en pantalla (Se puede quitar)
        cv2.waitKey(500)
        cv2.destroyAllWindows()

        # Escribir archivo modificado en el directorio
        images_pixel_array.append(re)
        cv2.imwrite("resized_"+image, re)
    #print(images_pixel_array)

def call_C_function():
    so_file = "./dataset_controller.so"
    my_functions = CDLL(so_file)
    print(my_functions.list_directory_files())

def main():
    #resize_img()
    call_C_function()

main()
