import cv2, glob
import os
from ctypes import *
from urllib.parse import urlparse

#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2

height = 28
width = 28


def resize_img():
    image_path = "images/"
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
        #images_pixel_array.append(re)
        cv2.imwrite(image_path+"resized_"+image, re)
    #print(images_pixel_array)

def create_txt_files_reading_imgs():
    image_path = "images/"
    images = glob.glob(image_path+"*.jpeg")
    images.extend(glob.glob(image_path+'*.png'))

    for image in images:
        filename_w_ext = os.path.basename(image)
        filename, file_extension = os.path.splitext(filename_w_ext)

        f = open("images_txt/" + filename + ".txt","w+")

        img = cv2.imread(image)
        
        f.write(str(height) + "\n")
        f.write(str(width) + "\n")

        for i in range(height):
            for j in range(width):
                k = img[i,j]
                pon = (int(k[0]) + int(k[1]) + int(k[2])) / 3
                f.write(str(pon) + "\n")
        f.close()

def call_C_function():
    so_file = "./dataset_controller.so"
    my_functions = CDLL(so_file)
    print(my_functions.list_directory_files())

def main():
    resize_img()
    #call_C_function()
    create_txt_files_reading_imgs()

main()
