import cv2, glob
#Si Python no contiene las libs, se instalan con estos comandos
# pip install opencv-python
# pip install glob2

def multiply(a,b):
    print("con parametros")
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def multiply_2():
    print("sin parametros")
    return 2016254241


def resize_img():
    # Lista de imágenes que leerá
    images = glob.glob("*.jpeg")

    height = 28
    width = 28

    for image in images:
        # Leer imagen y pasarla a BN indicando 0
        img=cv2.imread(image,0)

        # Variable donde quedará la imagen redimensionada
        re = cv2.resize(img,(height, width))

        # Revisar si la imagen ha sido redimensionada
        cv2.imshow("Checking",re)

        # Mostrar imgs en pantalla (Se puede quitar)
        #cv2.waitKey(500)
        #cv2.destroyAllWindows()

        # Escribir archivo modificado en el directorio
        cv2.imwrite("resized_"+image, re)
    return 2312