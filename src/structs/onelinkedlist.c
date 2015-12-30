#include <Python.h>
#include <structmember.h>


typedef struct OneLinkedList {
    PyObject *item;
    struct OneLinkedList *next;
} OneLinkedList;

typedef struct PyOneLinkedList {
    PyObject_HEAD
    OneLinkedList *first;
} PyOneLinkedList;


/**
----------------------------------
               Helpers
----------------------------------
**/


PyObject* list_to_pylist(PyOneLinkedList *self) {
    OneLinkedList *p = self->first;
    PyObject *pylist = PyList_New((Py_ssize_t)0);
    while((PyObject*)p != Py_None) {
        PyList_Append(pylist, p->item);
        p = p->next;
    }
    return pylist;
}

Py_ssize_t list_get_size(PyOneLinkedList *self) {
    OneLinkedList *p = self->first;
    Py_ssize_t size = 0;
    while((PyObject*)p != Py_None) {
        ++size;
        p = p->next;
    }
    return size;
}


void list_push(PyOneLinkedList *self, PyObject *item) {
    OneLinkedList *new_element = (OneLinkedList*)PyMem_Malloc(sizeof(OneLinkedList));
    Py_XINCREF(item);
    new_element->item = item;
    new_element->next = self->first;
    self->first = new_element;
}


OneLinkedList* list_pop(PyOneLinkedList *self) {
    OneLinkedList *l = self->first;
    self->first = l->next;
    return l;
}


OneLinkedList* list_get(PyOneLinkedList *self, Py_ssize_t pos) {
    OneLinkedList *l = self->first;
    int i;
    for(i=0; i != pos; ++i) {
        if((PyObject*)l == Py_None) break;
        l = l->next;
    }
    return l;
}

void list_insert(PyOneLinkedList *self, PyObject *item, Py_ssize_t pos) {
    if(pos == 0) return list_push(self, item);
    OneLinkedList* prev = list_get(self, pos-1);
    if((PyObject*)prev != Py_None) {
        OneLinkedList *elem = (OneLinkedList*)PyMem_Malloc(sizeof(OneLinkedList));
        elem->item = item;
        elem->next = prev->next;
        prev->next = elem;
    }
    // else ?
}

// OneLinkedList* list_take(PyOneLinkedList *self, Py_ssize_t pos) {
//     if(pos == 0) return list_pop(self);
//     OneLinkedList *elem;
//     OneLinkedList *prev = list_get(self, pos-1);
//     if((PyObject*)prev == Py_None) Py_RETURN_NONE;
//     elem = prev->next;
//     if((PyObject*)elem->next != Py_None) prev->next = elem->next;
//     return elem;
// }


/**
----------------------------------
               Methods
----------------------------------
**/


static PyObject*
PyOneLinkedList_Get(PyOneLinkedList *self, PyObject *args) {
    PyObject *num_obj;
    if(!PyArg_ParseTuple(args, "O", &num_obj)) return NULL;
    Py_ssize_t num = PyInt_AsSsize_t(num_obj);
    if(num < 0) return NULL; // add ex
    OneLinkedList *l = list_get(self, num);
    if((PyObject*)l == Py_None) Py_RETURN_NONE;
    return l->item;
}

static PyObject*
PyOneLinkedList_Pop(PyOneLinkedList *self) {
    OneLinkedList *l = list_pop(self);
    if((PyObject*)l == Py_None) Py_RETURN_NONE;
    return l->item;
}

static PyObject*
PyOneLinkedList_Push(PyOneLinkedList *self, PyObject *args) {
    PyObject *new_item;
    PyArg_ParseTuple(args, "O", &new_item);
    list_push(self, new_item);
    Py_RETURN_NONE;
}

static PyObject*
PyOneLinkedList_Insert(PyOneLinkedList *self, PyObject *args) {
    PyObject *new_item, *pos_obj;
    if(!PyArg_ParseTuple(args, "OO", &new_item, &pos_obj)) return NULL;
    Py_ssize_t pos = PyInt_AsSsize_t(pos_obj);
    if(pos < 0) return NULL; // add ex
    list_insert(self, new_item, pos);
    Py_RETURN_NONE;
}

static PyObject*
PyOneLinkedList_ToList(PyOneLinkedList *self) {
    return list_to_pylist(self);
}

static PyObject*
PyOneLinkedList_Repr(PyOneLinkedList *self) {
    PyObject *separator = PyString_FromString(", ");
    OneLinkedList *p = self->first;
    PyObject *seq = PyList_New((Py_ssize_t)0);
    while((PyObject*)p != Py_None) {
        PyList_Append(seq, PyObject_Repr(p->item));
        p = p->next;
    }
    return PyUnicode_FromFormat("OneLinkedList (size: %zd) [%U]",
        list_get_size(self), PyUnicode_Join(separator, seq));
}

static PyObject*
PyOneLinkedList_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyOneLinkedList *self = (PyOneLinkedList *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static void
PyOneLinkedList_Dealloc(PyOneLinkedList* self)
{
    printf("Q\n");
}

static int
PyOneLinkedList_Init(PyOneLinkedList *self, PyObject *args, PyObject *kwds)
{
    PyObject *resource = NULL;
    if(!PyArg_ParseTuple(args, "|O", &resource)) {
        printf("PyArg_ParseTuple\n");
    }
    Py_INCREF(Py_None);
    self->first = (OneLinkedList*)Py_None;
    if(resource == NULL) return 0;

    resource = PyObject_GetIter(resource);
    if (resource == NULL) {
        PyErr_SetString(PyExc_TypeError, "Object don't support the iterator protocol");
        return -1;
    }
    PyObject *item;
    while(item = PyIter_Next(resource)) {
        list_push(self, item);
    }
    return 0;
}


static PyMethodDef PyOneLinkedList_Methods[] = {
        {"get", (PyCFunction)PyOneLinkedList_Get, METH_VARARGS,
                "Get item"},
        {"pop", (PyCFunction)PyOneLinkedList_Pop, METH_NOARGS,
                "Pop item"},
        {"push", (PyCFunction)PyOneLinkedList_Push, METH_VARARGS,
                "Push item"},
        {"insert", (PyCFunction)PyOneLinkedList_Insert, METH_VARARGS,
                "Insert item"},
        {"to_list", (PyCFunction)PyOneLinkedList_ToList, METH_NOARGS,
                "Convert to list"},
        {NULL}
};


static PyMemberDef PyOneLinkedList_Members[] = {
        {NULL}  /* Sentinel */
};


static PyGetSetDef PyOneLinkedList_Getseters[] = {
        {NULL}  /* Sentinel */
};


PyTypeObject PyOneLinkedList_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "structs.PyOneLinkedList",   /*tp_name*/
    sizeof(PyOneLinkedList),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyOneLinkedList_Dealloc,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    (reprfunc)PyOneLinkedList_Repr, /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    (reprfunc)PyOneLinkedList_Repr, /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "PyOneLinkedList objects",   /* tp_doc */
    0,		                   /* tp_traverse */
    0,		                   /* tp_clear */
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    PyOneLinkedList_Methods,     /* tp_methods */
    PyOneLinkedList_Members,     /* tp_members */
    PyOneLinkedList_Getseters,   /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyOneLinkedList_Init,        /* tp_init */
    0,                         /* tp_alloc */
    PyOneLinkedList_New,                         /* tp_new */
};
