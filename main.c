// Fuente: https://stackoverflow.com/questions/54173733/convert-rgb-to-grayscale-in-bare-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
   1. sudo apt-get install python3.7-dev
   2. locate Python.h
      - copiar la ruta del archivo .h en "exec.sh"
*/
#include <Python.h>


int main(int argc, char *argv[]) {

   PyObject *pName, *pModule, *pDict, *pFunc, *pValue;

   // Initialize the Python Interpreter
   Py_Initialize();

   // Build the name object
   pName = PyUnicode_FromString("Python/util.py");

   // Load the module object
   pModule = PyImport_Import(pName);

   // pDict is a borrowed reference 
   pDict = PyModule_GetDict(pModule);

   // pFunc is also a borrowed reference 
   pFunc = PyDict_GetItemString(pDict, "multiply");

   if (PyCallable_Check(pFunc)) {
      PyObject_CallObject(pFunc, NULL);
   }
   else {
      PyErr_Print();
   }

   // Clean up
   Py_DECREF(pModule);
   Py_DECREF(pName);

   // Finish the Python Interpreter
   Py_Finalize();
   
   return 0;
}


void caca() {
   FILE *fIn = fopen("Examen/tiger.bmp", "rb");
   FILE *fOut = fopen("tiger_gray.bmp", "wb");
   if (!fIn || !fOut) {
      printf("File error.\n");
      return;
   }

   unsigned char header[54];
   fread(header, sizeof(unsigned char), 54, fIn);
   fwrite(header, sizeof(unsigned char), 54, fOut);

   int width = *(int*)&header[18];
   int height = abs(*(int*)&header[22]);
   int stride = (width * 3 + 3) & ~3;
   int padding = stride - width * 3;

   printf("width: %d (%d)\n", width, width * 3);
   printf("height: %d\n", height);
   printf("stride: %d\n", stride);
   printf("padding: %d\n", padding);

   unsigned char pixel[3];

   for (int y = 0; y < height; ++y)
   {
      for (int x = 0; x < width; ++x)
      {
         fread(pixel, 3, 1, fIn);
         unsigned char gray = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
         memset(pixel, gray, sizeof(pixel));
         
         fwrite(&pixel, 3, 1, fOut);
      }
      fread(pixel, padding, 1, fIn);
      fwrite(pixel, padding, 1, fOut);
   }
   fclose(fOut);
   fclose(fIn);
}