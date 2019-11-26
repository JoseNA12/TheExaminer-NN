import cv2, glob
import os
from ctypes import *
from urllib.parse import urlparse
from enum import Enum
import re

#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2


class AnnotationLabel(Enum):
    LABEL_A = "A"
    LABEL_B = "B"
    LABEL_C = "C"
    LABEL_D = "D"
    LABEL_E = "E"
    LABEL_F = "F"
    LABEL_X = "X"
    LABEL_S = "S"
    #
    REGEX_LABEL_A = "A{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_B = "B{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_C = "C{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_D = "D{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_E = "E{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_F = "F{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_X = "X{1}[0-9]+\s-\sJN\sJS"
    REGEX_LABEL_S = "S{1}[0-9]+\s-\sJN\sJS"


class Util:
    def __init__(self):
        super().__init__()
        self.height = 28
        self.width = 28
        self.count_img = 0

        self.path_images = "dataset/images/"
        self.path_images_set = "/dataset/images_set/"
        self.path_pixels = "dataset/pixels/"
        self.path_annotations = "dataset/annotations/"

        self.so_file = "./dataset_controller.so"

    def get_count_img(self):
        return self.count_img

    def process_images(self):
        images = glob.glob(self.path_images + "*.jpeg")
        images.extend(glob.glob(self.path_images + '*.png'))

        for image in images:
            # Leer imagen y pasarla a BN indicando 0
            img=cv2.imread(image,0)

            # Variable donde quedará la imagen redimensionada
            re = cv2.resize(img,(self.height, self.width))

            # Revisar si la imagen ha sido redimensionada
            #cv2.imshow("Checking",re)

            # Mostrar imgs en pantalla (Se puede quitar)
            #cv2.waitKey(500)
            #cv2.destroyAllWindows()

            # Escribir archivo modificado en el directorio
            #images_pixel_array.append(re)

            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)

            cv2.imwrite(os.getcwd() + self.path_images_set + filename + file_extension, re)
            self.count_img += 1

    def create_txt_files_pixels_imgs(self):
        images = glob.glob(self.path_images_set[1:] + "*.jpeg")
        images.extend(glob.glob(self.path_images_set[1:] + '*.png'))

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)

            f = open(self.path_pixels + filename + ".txt","w+")

            img = cv2.imread(image)
            
            #f.write(str(self.height) + "\n")
            #f.write(str(self.width) + "\n")

            for i in range(self.height):
                for j in range(self.width):
                    k = img[i,j]
                    pon = (int(k[0]) + int(k[1]) + int(k[2])) / 3
                    f.write(str(pon) + "\n")
            f.close()

    def create_txt_files_annotations(self):
        images = glob.glob(self.path_images_set[1:] + '*.jpeg')
        images.extend(glob.glob(self.path_images_set[1:] + '*.png'))

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)

            vector_text = "NONE"

            if (re.search(AnnotationLabel.REGEX_LABEL_A.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_A.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_B.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_B.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_C.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_C.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_D.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_D.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_E.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_E.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_F.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_F.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_X.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_X.value
            elif (re.search(AnnotationLabel.REGEX_LABEL_S.value, filename) != None):
                vector_text = AnnotationLabel.LABEL_S.value

            f = open(self.path_annotations + filename + ".txt","w+")
            f.write(vector_text)
            f.close()

    def call_C_function(self): 
        my_functions = CDLL(self.so_file)
        my_functions.list_directory_files(self.count_img) # le envio la cantidad de imagenes procesadas


def main():
    util = Util()
    util.process_images()
    util.create_txt_files_pixels_imgs()
    util.create_txt_files_annotations()

    #util.call_C_function()

main()
