#include <Python.h>
#include "include/onelinkedlist.h"


static PyMethodDef StructsModule_Methods[] = {
        {NULL}
};

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
initstructs(void)
{
    if(PyType_Ready(&PyOneLinkedList_Type) != 0) return;
    PyObject* m = Py_InitModule3("structs", StructsModule_Methods, "Docstring here");
    if(m == NULL) return;
    Py_INCREF(&PyOneLinkedList_Type);
    if(PyModule_AddObject(m, "OneLinkedList", (PyObject *)&PyOneLinkedList_Type) != 0) return;
}
