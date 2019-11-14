# TheExaminer-NN
Proyecto 4 - Inteligencia Artificial - Neural Network The Examiner - C

------------------------------------------------------------------------------------------------------------------
	IA - Proyecto 4 — Neural Network The Examiner
------------------------------------------------------------------------------------------------------------------

En este proyecto se desarrolla una versión básica de una red neuronal para el reconocimiento de las respuestas ingresadas en una plantilla de respuestas de un examen generado por The Examiner. 
Se adjuntan algunas imágenes de ejemplo. 

Toda la programación debe realizarse en C sobre Linux, únicamente. En particular, el proyecto debe ejecutar correctamente en los laboratorios de la Escuela. Si desea utilizar otras plataformas para correr su proyecto deberá consultar previamente con el profesor su uso. 

Es especial el proceso de ejecución de la red o redes neuronales requeridas debe estar escrito en C, así como los procesos para el entrenamiento y aprendizaje por retropropagación deben estar escritos en C. 

Para el procesamiento de las imágenes se pueden usar otras herramientas o lenguajes, bibliotecas y demás para facilitar este trabajo. Debe documentarse en detalle las herramientas usadas, las fuentes de donde son tomadas y cómo son usadas en su programa. Diagrame la integración de todos los componentes de su proyecto en su documentación y explíquelo. El software de terceros debe ser código open-source, no puede usarse código protegido sin consentimiento de sus dueños y así debe documentarse y justificarse.  

Para el usuario final del sistema, todo el proceso interno debe ser invisible. Es decir, el programa deberá hacer el tratamiento automático de las imágenes con las herramientas que requiera y finalmente ejecutar la red o redes neuronales necesarias para dar en un texto de salida las respuestas ingresadas en el examen a analizar. 

El programa recibirá una fotografía a colores que se asume contiene una plantilla de hoja de respuestas de un examen generado por The Examiner. 

No se debe asumir el tamaño o calidad de la foto esperada, tampoco la ubicación de la plantilla. Puede especificarse que la plantilla en la fotografía debe estar ubicada de forma correcta de frente con las casillas ordenadas de izquierda a derecha y de arriba a bajo, únicamente. 

La primera parte de lo que debe hacer su programa es distinguir la cuadrícula de la hoja de respuestas, seguidamente determinar el tamaño de dicha cuadrícula (cuantas filas y columnas hay) para poder procesar cada una de las respuestas ingresadas en el examen. 

El programa brindará la secuencia ordenada de las respuestas dadas por el usuario en letras mayúsculas separadas por espacios en blanco. Deberá ordenarlas en filas, en la misma distribución que la cuadrícula recibida.

En caso de haberse ingresado una letra diferente a las permitidas (A, B, C, D, E, F) podrán identificar de alguna manera el error de forma genérica por ejemplo con una X sin que sea necesario reconocer la letra errónea ingresada u otro símbolo. En caso de haber un espacio en blanco deberá también usarse algún caracter especial para señalarlo como una S por ejemplo. 
