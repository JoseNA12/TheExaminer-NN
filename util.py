import cv2, glob
import os
from ctypes import *
from urllib.parse import urlparse
from enum import Enum
import re

#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2

class labelLabel(Enum):
    LABEL_0 = "0"
    LABEL_1 = "1"
    LABEL_2 = "2"
    LABEL_3 = "3"
    LABEL_4 = "4"
    LABEL_5 = "5"
    LABEL_6 = "6"
    LABEL_7 = "7"
    LABEL_8 = "8"
    LABEL_9 = "9"
    
    REGEX_LABEL_0 = "0{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_1 = "1{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_2 = "2{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_3 = "3{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_4 = "4{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_5 = "5{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_6 = "6{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_7 = "7{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_8 = "8{1}-[0-9]+-JN\sJS"
    REGEX_LABEL_9 = "9{1}-[0-9]+-JN\sJS"

    LABEL_A = "A"
    LABEL_B = "B"
    LABEL_C = "C"
    LABEL_D = "D"
    LABEL_E = "E"
    LABEL_F = "F"
    LABEL_X = "X"
    LABEL_S = "S"

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

        self.path_images_numbers = "dataset/images/numbers/"
        self.path_images_set_numbers = "/dataset/images_set/numbers/"
        self.path_pixels_numbers = "dataset/pixels/numbers/"
        self.path_labels_numbers = "dataset/annotations/numbers/"

        self.path_images_letters = "dataset/images/letters/"
        self.path_images_set_letters = "/dataset/images_set/letters/"
        self.path_pixels_letters = "dataset/pixels/letters/"
        self.path_annotations_letters = "dataset/annotations/letters/"

        self.so_file = "./neural_network.so" #"./dataset_controller.so"

    def get_count_img(self):
        return self.count_img

    def process_images(self, dataType):
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_numbers + "*.jpeg")
            images.extend(glob.glob(self.path_images_numbers + '*.png'))
        else:
            images = glob.glob(self.path_images_letters + "*.jpeg")
            images.extend(glob.glob(self.path_images_letters + '*.png'))

        for image in images:
            # Leer imagen y pasarla a BN indicando 0
            img=cv2.imread(image,0)

            # Variable donde quedará la imagen redimensionada
            re = cv2.resize(img,(self.height, self.width))
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            cv2.imwrite(os.getcwd() + self.path_images_set + filename + file_extension, re)
            self.count_img += 1

    def create_txt_files_pixels_imgs(self, dataType):
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_numbers[1:] + '*.png'))
        else:
            images = glob.glob(self.path_images_set_letters[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters[1:] + '*.png'))

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            f = open(self.path_pixels + filename + ".txt","w+")
            img = cv2.imread(image)
            
            for i in range(self.height):
                for j in range(self.width):
                    k = img[i,j]
                    pon = (int(k[0]) + int(k[1]) + int(k[2])) / 3
                    f.write(str(pon) + ",\n")
            f.close()
            self.count_img += 1

    def create_txt_files_labels(self, dataType):
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_numbers[1:] + '*.png'))
        else:
            images = glob.glob(self.path_images_set_letters[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters[1:] + '*.png'))

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)

            vector_text = "NONE"

            if (dataType == "numbers"):
                if (re.search(labelLabel.REGEX_LABEL_0.value, filename) != None):
                    vector_text = labelLabel.LABEL_0.value
                elif (re.search(labelLabel.REGEX_LABEL_1.value, filename) != None):
                    vector_text = labelLabel.LABEL_1.value
                elif (re.search(labelLabel.REGEX_LABEL_2.value, filename) != None):
                    vector_text = labelLabel.LABEL_2.value
                elif (re.search(labelLabel.REGEX_LABEL_3.value, filename) != None):
                    vector_text = labelLabel.LABEL_3.value
                elif (re.search(labelLabel.REGEX_LABEL_4.value, filename) != None):
                    vector_text = labelLabel.LABEL_4.value
                elif (re.search(labelLabel.REGEX_LABEL_5.value, filename) != None):
                    vector_text = labelLabel.LABEL_5.value
                elif (re.search(labelLabel.REGEX_LABEL_6.value, filename) != None):
                    vector_text = labelLabel.LABEL_6.value
                elif (re.search(labelLabel.REGEX_LABEL_7.value, filename) != None):
                    vector_text = labelLabel.LABEL_7.value
                elif (re.search(labelLabel.REGEX_LABEL_8.value, filename) != None):
                    vector_text = labelLabel.LABEL_8.value
                elif (re.search(labelLabel.REGEX_LABEL_9.value, filename) != None):
                    vector_text = labelLabel.LABEL_9.value
            else:
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

            f = open(self.path_labels + filename + ".txt","w+")
            f.write(vector_text)
            f.close()
        
    def call_C_function(self, dataType): 
        print(self.count_img)
        my_functions = CDLL(self.so_file)
        my_functions.start(self.count_img, dataType) # le envio la cantidad de imagenes procesadas
    
    def menu():
        while(True):
            print("==================================================================================================\n")
            print("                                   Tecnológico de Costa Rica                                      \n")
            print("==================================================================================================\n")
            print("                                      Josué Suárez Campos                                         \n")
            print("                                      José Navarro Acuña                                          \n")
            print("==================================================================================================\n")
            print(" *** Opciones ***                                                                                 \n")
            print(" 1 - Entrenar NN Letras                                                                           \n")
            print(" 2 - Entrenar NN Números                                                                          \n")
            print(" 3 - Analizar imagen letra                                                                        \n")
            print(" 4 - Analizar imagen número                                                                       \n")
            print(" 5 - Salir                                                                                        \n")
            response = input()
            dataType = ''
            if(response == '1'):
                self.count_img = 0
                dataType = "letters"
                process_images(dataType)
                create_txt_files_pixels_imgs(dataType)
                create_txt_files_labels(dataType)
                call_C_function(dataType)
            elif(response == '2'):
                self.count_img = 0
                dataType = "numbers"
                create_txt_files_pixels_imgs(dataType)
                create_txt_files_labels(dataType)
                call_C_function(dataType)
            elif(response == '3'):
                print("3")
            elif(response == '4'):
                print("4")
            elif(response == '5'):
                break

def main():  
    util = Util()
    util.menu()

main()
