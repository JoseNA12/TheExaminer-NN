// Fuente: https://stackoverflow.com/questions/54173733/convert-rgb-to-grayscale-in-bare-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
   1. sudo apt-get install python3.7-dev
   2. locate Python.h
      - copiar la ruta del archivo .h en "exec.sh"
*/
#define PY_SSIZE_T_CLEAN
#include <Python.h>

/* Documentación de la liga de Python en C
   https://docs.python.org/3.7/extending/embedding.html

   Nota:
      Se deben agregar las siguientes 2 lineas justo despues de "Py_Initialize()":
      
         PyRun_SimpleString("import sys");
         PyRun_SimpleString("sys.path.append(\".\")");

      O bien, agregar esta linea al principio del main:

         setenv("PYTHONPATH",".",1);

      Fuente: https://stackoverflow.com/questions/24492327/python-embedding-in-c-importerror-no-module-named-pyfunction
*/

/*
   Sino reconoce las librerias de Python instalar:
      pip3 install opencv-python
*/

char *file_name = "util";


void funcionPrueba_con_parametros() {
   char *function_name = "multiply";

   PyObject *pName, *pModule, *pFunc;
   PyObject *pArgs, *pValue;

   int i;

   Py_Initialize();

   pName = PyUnicode_DecodeFSDefault(file_name);
   // Error checking of pName left out

   pModule = PyImport_Import(pName);
   Py_DECREF(pName);

   if (pModule != NULL) {
      pFunc = PyObject_GetAttrString(pModule, function_name);
      // pFunc is a new reference

      if (pFunc && PyCallable_Check(pFunc)) {
         
         pArgs = PyTuple_New(2);
         for (i = 0; i < 2; ++i) {
               pValue = PyLong_FromLong(atoi("19"));
               if (!pValue) {
                  Py_DECREF(pArgs);
                  Py_DECREF(pModule);
                  fprintf(stderr, "Cannot convert argument\n");
                  return;
               }
               // pValue reference stolen here:
               PyTuple_SetItem(pArgs, i, pValue);
         }
         pValue = PyObject_CallObject(pFunc, pArgs);
         Py_DECREF(pArgs);
         if (pValue != NULL) {
               printf("Result of call: %ld\n", PyLong_AsLong(pValue));
               Py_DECREF(pValue);
         }
         else {
               Py_DECREF(pFunc);
               Py_DECREF(pModule);
               PyErr_Print();
               fprintf(stderr,"Call failed\n");
               return;
         }
      }
      else {
         if (PyErr_Occurred())
               PyErr_Print();
         fprintf(stderr, "Cannot find function \"%s\"\n", function_name);
      }
      Py_XDECREF(pFunc);
      Py_DECREF(pModule);
   }
   else {
      PyErr_Print();
      fprintf(stderr, "Failed to load \"%s\"\n", file_name);
      return;
   }
   if (Py_FinalizeEx() < 0) {
      return;
   }
}

void funcionPrueba_sin_parametros() {
   char *function_name = "resize_img";

   PyObject *pName, *pModule, *pFunc;
   PyObject *pValue;

   int i;

   Py_Initialize();

   pName = PyUnicode_DecodeFSDefault(file_name);
   // Error checking of pName left out

   pModule = PyImport_Import(pName);
   Py_DECREF(pName);

   if (pModule != NULL) {
      pFunc = PyObject_GetAttrString(pModule, function_name);

      if (pFunc && PyCallable_Check(pFunc)) {
         pValue = PyObject_CallObject(pFunc, NULL);

         if (pValue != NULL) {
               printf("Result of call: %ld\n", PyLong_AsLong(pValue));
               Py_DECREF(pValue);
         }
         else {
               Py_DECREF(pFunc);
               Py_DECREF(pModule);
               PyErr_Print();
               fprintf(stderr,"Call failed\n");
               return;
         }
      }
      else {
         if (PyErr_Occurred())
               PyErr_Print();
         fprintf(stderr, "Cannot find function \"%s\"\n", function_name);
      }
      Py_XDECREF(pFunc);
      Py_DECREF(pModule);
   }
   else {
      PyErr_Print();
      fprintf(stderr, "Failed to load \"%s\"\n", file_name);
      return;
   }
   if (Py_FinalizeEx() < 0) {
      return;
   }
}


int main(int argc, char *argv[]) {
   setenv("PYTHONPATH",".",1);
   
   //funcionPrueba_con_parametros();

   funcionPrueba_sin_parametros();

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