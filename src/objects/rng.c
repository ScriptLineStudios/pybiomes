#include <stdio.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "../external/cubiomes/rng.h"

typedef struct {
    PyObject_HEAD
    uint64_t seed;
} RngObject;

typedef struct {
    PyObject_HEAD
    Xoroshiro state;
} XoroshiroObject;

static int Rng_traverse(RngObject *self, visitproc visit, void *arg) {
    return 0;
}

static int Rng_clear(RngObject *self) {
    return 0;
}

static void Rng_dealloc(RngObject *self) {
    PyObject_GC_UnTrack(self);
    Rng_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Rng_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    RngObject *self;
    self = (RngObject *) type->tp_alloc(type, 0);
	
    return (PyObject *)self;
}

static int Rng_init(RngObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"seed", NULL};
	
    uint64_t seed;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "K", kwlist, &seed)) {
        return -1;
    }

    self->seed = seed;
	
    return 0;
}

static PyObject *Rng_set_seed(RngObject *self, PyObject *args) {	
	uint64_t value;
    if (!PyArg_ParseTuple(args, "K", &(self->seed), &value)) {
        return NULL;
    }
	
    setSeed(&(self->seed), value);
	Py_RETURN_NONE;
}

static PyObject *Rng_next(RngObject *self, PyObject *args) {
	int bits;
    if (!PyArg_ParseTuple(args, "i", &bits)) {
        return NULL;
    }
    return PyLong_FromLong(next(&(self->seed), bits));
}

static PyObject *Rng_next_long(RngObject *self, PyObject *args) {
    return PyLong_FromLongLong(nextLong(&(self->seed)));
}

static PyObject *Rng_next_int(RngObject *self, PyObject *args) {
	int n;
    if (!PyArg_ParseTuple(args, "i", &n)) {
        return NULL;
    }
	
    return PyLong_FromLong(nextInt(&(self->seed), n));
}

static PyObject *Rng_next_float(RngObject *self, PyObject *args) {
    return PyFloat_FromDouble(nextFloat(&(self->seed)));
}

static PyObject *Rng_next_double(RngObject *self, PyObject *args) {
    return PyFloat_FromDouble(nextDouble(&(self->seed)));
}

static PyMethodDef Rng_methods[] = {
    {"set_seed", (PyCFunction)Rng_set_seed, METH_VARARGS, "Set the seed value"},
    {"next", (PyCFunction)Rng_next_long, METH_NOARGS, "Call the next() rng function"},
    {"next_long", (PyCFunction)Rng_next_long, METH_NOARGS, "Generate the next long random number"},
    {"next_int", (PyCFunction)Rng_next_int, METH_VARARGS, "Generate the next int random number up to n"},
    {"next_float", (PyCFunction)Rng_next_float, METH_NOARGS, "Generate the next float random number"},
    {"next_double", (PyCFunction)Rng_next_double, METH_NOARGS, "Generate the next double random number"},
    {NULL}  /* Sentinel */
};

static PyTypeObject RngType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.Rng",
    .tp_doc = "Rng objects",
    .tp_basicsize = sizeof(RngObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Rng_new,
    .tp_init = (initproc) Rng_init,
    .tp_dealloc = (destructor) Rng_dealloc,
    .tp_traverse = (traverseproc) Rng_traverse,
    .tp_clear = (inquiry) Rng_clear,
    .tp_methods = Rng_methods,
};


// --- Xoroshiro Object Methods ---

static int Xoroshiro_traverse(XoroshiroObject *self, visitproc visit, void *arg) {
    return 0;
}

static int Xoroshiro_clear(XoroshiroObject *self) {
    return 0;
}

static void Xoroshiro_dealloc(XoroshiroObject *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Xoroshiro_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    XoroshiroObject *self;
    self = (XoroshiroObject *) type->tp_alloc(type, 0);
    if (!self) { // A good practice is to check if allocation failed.
        return NULL;
    }
    return (PyObject *)self;
}

static int Xoroshiro_init(XoroshiroObject *self, PyObject *args, PyObject *kwds) {
    return 0;
}

static PyObject *Xoroshiro_set_seed(XoroshiroObject *self, PyObject *args) {
    uint64_t value;
    if (!PyArg_ParseTuple(args, "K", &value)) {
        return NULL;
    }
    xSetSeed(&(self->state), value);
    Py_RETURN_NONE;
}

static PyObject *Xoroshiro_next_long(XoroshiroObject *self, PyObject *args) {
    return PyLong_FromUnsignedLongLong(xNextLong(&(self->state)));
}

static PyObject *Xoroshiro_next_int(XoroshiroObject *self, PyObject *args) {
    uint32_t n;
    if (!PyArg_ParseTuple(args, "I", &n)) {
        return NULL;
    }
    return PyLong_FromLong(xNextInt(&(self->state), n));
}

static PyObject *Xoroshiro_next_float(XoroshiroObject *self, PyObject *args) {
    return PyFloat_FromDouble(xNextFloat(&(self->state)));
}

static PyObject *Xoroshiro_next_double(XoroshiroObject *self, PyObject *args) {
    return PyFloat_FromDouble(xNextDouble(&(self->state)));
}

static PyObject *Xoroshiro_next_long_j(XoroshiroObject *self, PyObject *args) {
    return PyLong_FromUnsignedLongLong(xNextLongJ(&(self->state)));
}

static PyObject *Xoroshiro_next_int_j(XoroshiroObject *self, PyObject *args) {
    uint32_t n;
    if (!PyArg_ParseTuple(args, "I", &n)) {
        return NULL;
    }
    return PyLong_FromLong(xNextIntJ(&(self->state), n));
}

static PyMethodDef Xoroshiro_methods[] = {
    {"set_seed", (PyCFunction)Xoroshiro_set_seed, METH_VARARGS, "Set the seed value"},
    {"next_long", (PyCFunction)Xoroshiro_next_long, METH_NOARGS, "Generate the next long random number"},
    {"next_int", (PyCFunction)Xoroshiro_next_int, METH_VARARGS, "Generate the next int random number up to n"},
    {"next_float", (PyCFunction)Xoroshiro_next_float, METH_NOARGS, "Generate the next float random number"},
    {"next_double", (PyCFunction)Xoroshiro_next_double, METH_NOARGS, "Generate the next double random number"},
    {"next_long_j", (PyCFunction)Xoroshiro_next_long_j, METH_NOARGS, "Generate the next long random number from two separate xNextLong calls"},
    {"next_int_j", (PyCFunction)Xoroshiro_next_int_j, METH_VARARGS, "Generate the next int random number up to n using the worldgenrandom method"},
    {NULL}  /* Sentinel */
};

static PyTypeObject XoroshiroType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.Xoroshiro",
    .tp_doc = "Xoroshiro random number generator",
    .tp_basicsize = sizeof(XoroshiroObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Xoroshiro_new,
    .tp_init = (initproc)Xoroshiro_init,
    .tp_dealloc = (destructor)Xoroshiro_dealloc,
    .tp_traverse = (traverseproc) Xoroshiro_traverse,
    .tp_clear = (inquiry) Xoroshiro_clear,
    .tp_methods = Xoroshiro_methods,
};
