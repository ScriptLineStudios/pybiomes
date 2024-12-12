#include <stdio.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "../external/cubiomes/finders.h"

typedef struct {
    PyObject_HEAD
    int version;
} FinderObject;

static int Finder_traverse(FinderObject *self, visitproc visit, void *arg) {
    return 0;
}

static int Finder_clear(FinderObject *self) {
    return 0;
}

static void Finder_dealloc(FinderObject *self) {
    PyObject_GC_UnTrack(self);
    Finder_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Finder_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    FinderObject *self;
    self = (FinderObject *) type->tp_alloc(type, 0);

    return (PyObject *)self;
}

static int Finder_init(FinderObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"version", NULL};

    int version;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, &version)) {
        return -1;
    }

    self->version = version;

    return 0;
}

static PyObject *Finder_get_structure_pos(FinderObject *self, PyObject *args) { 
    int structure, reg_x, reg_z;
    uint64_t seed;

    if (!PyArg_ParseTuple(args, "iKii", &structure, &seed, &reg_x, &reg_z)) {
        return NULL;
    }

    Pos p;
    int success = getStructurePos(structure, self->version, seed, reg_x, reg_z, &p);
    
    if (success == 0) {
        Py_RETURN_NONE;
    }

    PosObject *ret = Pos_new(&PosType, NULL, NULL);
    
    ret->pos.x = p.x;
    ret->pos.z = p.z;

    return (PyObject *)ret;
}

static PyMemberDef Finder_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Finder_methods[] = {
    {"get_structure_pos", (PyCFunction)Finder_get_structure_pos, METH_VARARGS, "Finds a structures position within the given region"},
    {NULL}  /* Sentinel */
};

static PyTypeObject FinderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.Finder",
    .tp_doc = "Finder objects",
    .tp_basicsize = sizeof(FinderObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Finder_new,
    .tp_init = (initproc) Finder_init,
    .tp_dealloc = (destructor) Finder_dealloc,
    .tp_traverse = (traverseproc) Finder_traverse,
    .tp_clear = (inquiry) Finder_clear,
    .tp_members = Finder_members,
    .tp_methods = Finder_methods,
};
