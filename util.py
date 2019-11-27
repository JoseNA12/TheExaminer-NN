import cv2, glob
import os
from ctypes import *
from urllib.parse import urlparse
from enum import Enum
import re

#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2

class Labels(Enum):
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
        self.path_images_set_numbers = "/dataset/images/numbers/"
        self.path_pixels_numbers = "dataset/pixels/numbers/"
        self.path_labels_numbers = "dataset/annotations/numbers/"

        self.path_images_letters = "dataset/images/letters/"
        self.path_images_set_letters = "/dataset/images_set/letters/"
        self.path_pixels_letters = "dataset/pixels/letters/"
        self.path_labels_letters = "dataset/annotations/letters/"

        self.so_file_numbers = "./neural_network_numbers.so" 
        self.so_file_letters = "./neural_network_letters.so" 

        #Single Img
        self.path_images_numbers_compute = "/dataset/compute/numbers/"
        self.path_images_set_numbers_compute = "/dataset/compute_set/numbers/"
        self.path_pixels_numbers_compute = "dataset/pixels_compute/numbers/"
        self.path_labels_numbers_compute = "dataset/annotations_compute/numbers/"

        self.path_images_letters_compute = "/dataset/computet/letters/"
        self.path_images_set_letters_compute = "/dataset/compute_set/letters/"
        self.path_pixels_letters_compute = "dataset/pixels_compute/letters/"
        self.path_labels_letters_compute = "dataset/annotations_compute/letters/"

    def get_count_img(self):
        return self.count_img

    def process_images(self, dataType):
        path_images_set = ''
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_numbers + "*.jpg")
            images.extend(glob.glob(self.path_images_numbers + '*.png'))
            path_images_set = self.path_images_set_numbers
        else:
            images = glob.glob(self.path_images_letters + "*.jpeg")
            images.extend(glob.glob(self.path_images_letters + '*.png'))
            path_images_set = self.path_images_set_letters

        for image in images:
            # Leer imagen y pasarla a BN indicando 0
            img=cv2.imread(image,0)

            # Variable donde quedará la imagen redimensionada
            re = cv2.resize(img,(self.height, self.width))
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            cv2.imwrite(os.getcwd() + path_images_set + filename + file_extension, re)

    def process_single_image(self, dataType):
        path_images_set = ''
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_numbers_compute + "*.jpg")
            images.extend(glob.glob(self.path_images_numbers_compute + '*.png'))
            path_images_set = self.path_images_set_numbers_compute
        else:
            images = glob.glob(self.path_images_letters_compute + "*.jpeg")
            images.extend(glob.glob(self.path_images_letters_compute + '*.png'))
            path_images_set = self.path_images_set_letters_compute

        for image in images:
            # Leer imagen y pasarla a BN indicando 0
            img=cv2.imread(image,0)

            # Variable donde quedará la imagen redimensionada
            re = cv2.resize(img,(self.height, self.width))
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            cv2.imwrite(os.getcwd() + path_images_set + filename + file_extension, re)

    def create_txt_files_pixels_imgs(self, dataType):
        path_pixels= ''
        images = None 
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers[1:] + "*.jpg")
            images.extend(glob.glob(self.path_images_set_numbers[1:] + '*.png'))
            path_pixels = self.path_pixels_numbers
        else:
            images = glob.glob(self.path_images_set_letters[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters[1:] + '*.png'))
            path_pixels = self.path_pixels_letters

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            f = open(path_pixels + filename + ".txt","w+")
            img = cv2.imread(image)
            
            for i in range(self.height):
                for j in range(self.width):
                    k = img[i,j]
                    pon = (int(k[0]) + int(k[1]) + int(k[2])) / 3
                    f.write(str(pon) + ",\n")
            f.close()
            self.count_img += 1

    def create_txt_files_pixels_for_single_img(self, dataType):
        path_pixels= ''
        images = None 
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers_compute[1:] + "*.jpg")
            images.extend(glob.glob(self.path_images_set_numbers_compute[1:] + '*.png'))
            path_pixels = self.path_pixels_numbers_compute
        else:
            images = glob.glob(self.path_images_set_letters_compute[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters_compute[1:] + '*.png'))
            path_pixels = self.path_pixels_letters_compute

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            f = open(path_pixels + filename + ".txt","w+")
            img = cv2.imread(image)
            
            for i in range(self.height):
                for j in range(self.width):
                    k = img[i,j]
                    pon = (int(k[0]) + int(k[1]) + int(k[2])) / 3
                    f.write(str(pon) + ",\n")
            f.close()
            self.count_img += 1

    def create_txt_files_labels(self, dataType):
        path_labels = ''
        images = None 
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers[1:] + "*.jpg")
            images.extend(glob.glob(self.path_images_set_numbers[1:] + '*.png'))
            path_labels = self.path_labels_numbers
        else:
            images = glob.glob(self.path_images_set_letters[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters[1:] + '*.png'))
            path_labels = self.path_labels_letters

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            
            vector_text = "NONE"

            if (dataType == "numbers"):
                if (re.search(Labels.REGEX_LABEL_0.value, filename) != None):
                    vector_text = Labels.LABEL_0.value
                elif (re.search(Labels.REGEX_LABEL_1.value, filename) != None):
                    vector_text = Labels.LABEL_1.value
                elif (re.search(Labels.REGEX_LABEL_2.value, filename) != None):
                    vector_text = Labels.LABEL_2.value
                elif (re.search(Labels.REGEX_LABEL_3.value, filename) != None):
                    vector_text = Labels.LABEL_3.value
                elif (re.search(Labels.REGEX_LABEL_4.value, filename) != None):
                    vector_text = Labels.LABEL_4.value
                elif (re.search(Labels.REGEX_LABEL_5.value, filename) != None):
                    vector_text = Labels.LABEL_5.value
                elif (re.search(Labels.REGEX_LABEL_6.value, filename) != None):
                    vector_text = Labels.LABEL_6.value
                elif (re.search(Labels.REGEX_LABEL_7.value, filename) != None):
                    vector_text = Labels.LABEL_7.value
                elif (re.search(Labels.REGEX_LABEL_8.value, filename) != None):
                    vector_text = Labels.LABEL_8.value
                elif (re.search(Labels.REGEX_LABEL_9.value, filename) != None):
                    vector_text = Labels.LABEL_9.value
            else:
                if (re.search(Labels.REGEX_LABEL_A.value, filename) != None):
                    vector_text = Labels.LABEL_A.value
                elif (re.search(Labels.REGEX_LABEL_B.value, filename) != None):
                    vector_text = Labels.LABEL_B.value
                elif (re.search(Labels.REGEX_LABEL_C.value, filename) != None):
                    vector_text = Labels.LABEL_C.value
                elif (re.search(Labels.REGEX_LABEL_D.value, filename) != None):
                    vector_text = Labels.LABEL_D.value
                elif (re.search(Labels.REGEX_LABEL_E.value, filename) != None):
                    vector_text = Labels.LABEL_E.value
                elif (re.search(Labels.REGEX_LABEL_F.value, filename) != None):
                    vector_text = Labels.LABEL_F.value
                elif (re.search(Labels.REGEX_LABEL_X.value, filename) != None):
                    vector_text = Labels.LABEL_X.value
                elif (re.search(Labels.REGEX_LABEL_S.value, filename) != None):
                    vector_text = Labels.LABEL_S.value

            f = open(path_labels + filename + ".txt","w+")
            f.write(vector_text)
            f.close()
    

    def create_txt_files_labels_for_single_img(self, dataType):
        path_labels = ''
        images = None 
        if (dataType == "numbers"):
            images = glob.glob(self.path_images_set_numbers_compute[1:] + "*.jpg")
            images.extend(glob.glob(self.path_images_set_numbers_compute[1:] + '*.png'))
            path_labels = self.path_labels_numbers_compute
        else:
            images = glob.glob(self.path_images_set_letters_compute[1:] + "*.jpeg")
            images.extend(glob.glob(self.path_images_set_letters_compute[1:] + '*.png'))
            path_labels = self.path_labels_letters_compute

        for image in images:
            filename_w_ext = os.path.basename(image)
            filename, file_extension = os.path.splitext(filename_w_ext)
            
            vector_text = "NONE"

            if (dataType == "numbers"):
                if (re.search(Labels.REGEX_LABEL_0.value, filename) != None):
                    vector_text = Labels.LABEL_0.value
                elif (re.search(Labels.REGEX_LABEL_1.value, filename) != None):
                    vector_text = Labels.LABEL_1.value
                elif (re.search(Labels.REGEX_LABEL_2.value, filename) != None):
                    vector_text = Labels.LABEL_2.value
                elif (re.search(Labels.REGEX_LABEL_3.value, filename) != None):
                    vector_text = Labels.LABEL_3.value
                elif (re.search(Labels.REGEX_LABEL_4.value, filename) != None):
                    vector_text = Labels.LABEL_4.value
                elif (re.search(Labels.REGEX_LABEL_5.value, filename) != None):
                    vector_text = Labels.LABEL_5.value
                elif (re.search(Labels.REGEX_LABEL_6.value, filename) != None):
                    vector_text = Labels.LABEL_6.value
                elif (re.search(Labels.REGEX_LABEL_7.value, filename) != None):
                    vector_text = Labels.LABEL_7.value
                elif (re.search(Labels.REGEX_LABEL_8.value, filename) != None):
                    vector_text = Labels.LABEL_8.value
                elif (re.search(Labels.REGEX_LABEL_9.value, filename) != None):
                    vector_text = Labels.LABEL_9.value
            else:
                if (re.search(Labels.REGEX_LABEL_A.value, filename) != None):
                    vector_text = Labels.LABEL_A.value
                elif (re.search(Labels.REGEX_LABEL_B.value, filename) != None):
                    vector_text = Labels.LABEL_B.value
                elif (re.search(Labels.REGEX_LABEL_C.value, filename) != None):
                    vector_text = Labels.LABEL_C.value
                elif (re.search(Labels.REGEX_LABEL_D.value, filename) != None):
                    vector_text = Labels.LABEL_D.value
                elif (re.search(Labels.REGEX_LABEL_E.value, filename) != None):
                    vector_text = Labels.LABEL_E.value
                elif (re.search(Labels.REGEX_LABEL_F.value, filename) != None):
                    vector_text = Labels.LABEL_F.value
                elif (re.search(Labels.REGEX_LABEL_X.value, filename) != None):
                    vector_text = Labels.LABEL_X.value
                elif (re.search(Labels.REGEX_LABEL_S.value, filename) != None):
                    vector_text = Labels.LABEL_S.value

            f = open(path_labels + filename + ".txt","w+")
            f.write(vector_text)
            f.close()

    def call_C_function(self, dataType, so_file): 
        print(self.count_img)
        my_functions = CDLL(so_file)
        s = c_char_p(dataType.encode('utf-8'))
        my_functions.start(self.count_img, s) # le envio la cantidad de imagenes procesadas

    def compute_single_img(self, dataType, so_file): 
        my_functions = CDLL(so_file)
        s = c_char_p(dataType.encode('utf-8'))
        my_functions.compute_image(s) # le envio la cantidad de imagenes procesadas
    
    def menu(self):
        while(True):
            print("============================================")
            print("          Tecnológico de Costa Rica         ")
            print("============================================")
            print("           Josué Suárez Campos              ")
            print("           José Navarro Acuña               ")
            print("============================================")
            print(" *** Opciones ***                           ")
            print(" 1 - Entrenar NN Letras                     ")
            print(" 2 - Entrenar NN Números                    ")
            print(" 3 - Analizar imagen letra                  ")
            print(" 4 - Analizar imagen número                 ")
            print(" 5 - Salir                                  ")
            response = input(">> ")
            dataType = ''
            if(response == '1'):
                self.count_img = 0
                dataType = "letters"
                self.process_images(dataType)
                self.create_txt_files_pixels_imgs(dataType)
                self.create_txt_files_labels(dataType)
                self.call_C_function(dataType, self.so_file_letters)
            elif(response == '2'):
                self.count_img = 0
                dataType = "numbers"
                self.create_txt_files_pixels_imgs(dataType)
                self.create_txt_files_labels(dataType)
                self.call_C_function(dataType, self.so_file_numbers)
            elif(response == '3'):
                print("Ingrese la imagen a procesar en el directorio compute/letters")
                print("Presione Enter para continuar")
                input(">> ")
                dataType = "letters"
                self.process_single_image(dataType)
                self.create_txt_files_pixels_for_single_img(dataType)
                self.create_txt_files_labels_for_single_img(dataType)
                self.compute_single_img(dataType, self.so_file_letters)
            elif(response == '4'):
                print("Ingrese la imagen a procesar en el directorio compute/numbers")
                print("Presione Enter para continuar")
                input(">> ")
                dataType = "numbers"
                self.process_single_image(dataType)
                self.create_txt_files_pixels_for_single_img(dataType)
                self.create_txt_files_labels_for_single_img(dataType)
                self.compute_single_img(dataType, self.so_file_numbers)
            elif(response == '5'):
                break

def main():  
    util = Util()
    util.menu()

main()
