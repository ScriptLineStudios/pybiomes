#include <stdio.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "../external/cubiomes/finders.h"

typedef struct {
    PyObject_HEAD
    Pos pos;
} PosObject;

static int Pos_traverse(PosObject *self, visitproc visit, void *arg) {
    return 0;
}

static int Pos_clear(PosObject *self) {
    return 0;
}

static void Pos_dealloc(PosObject *self) {
    PyObject_GC_UnTrack(self);
    Pos_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Pos_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PosObject *self;
    self = (PosObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->pos = (Pos){0};
    }
    return (PyObject *) self;
}

static int Pos_init(PosObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"x", "z", NULL};

    int x, z;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist, &x, &z)) {
        return -1;
    }

    self->pos.x = x;
    self->pos.z = z;

    return 0;
}

static PyObject *Pos_get_x(PosObject *self)  {
    return PyLong_FromLong(self->pos.x);
}

static PyObject *Pos_get_z(PosObject *self)  {
    return PyLong_FromLong(self->pos.z);
}

static int Pos_set_x(PosObject *self, PyObject *args)  {
    if (PyLong_Check(args)) {
        self->pos.x = PyLong_AsLong(args);
        return 0;
    }
    return -1;
}

static int Pos_set_z(PosObject *self, PyObject *args)  {
    if (PyLong_Check(args)) {
        self->pos.z = PyLong_AsLong(args);
        return 0;
    }
    return -1;
}

static PyObject *Pos_repr(PosObject *self)  {
    return PyUnicode_FromFormat("Pos(x=%d, z=%d)", self->pos.x, self->pos.z);
}

static PyMemberDef Pos_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Pos_methods[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef Pos_getsets[] = {
    {"x", (getter)Pos_get_x, (setter)Pos_set_x, NULL, NULL},
    {"z", (getter)Pos_get_z, (setter)Pos_set_z, NULL, NULL},
    {NULL, 0, NULL, NULL, NULL} /* Sentinel */
};

static PyTypeObject PosType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.Pos",
    .tp_doc = "Pos objects",
    .tp_basicsize = sizeof(PosObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Pos_new,
    .tp_init = (initproc) Pos_init,
    .tp_dealloc = (destructor) Pos_dealloc,
    .tp_repr = (reprfunc) Pos_repr,
    .tp_traverse = (traverseproc) Pos_traverse,
    .tp_clear = (inquiry) Pos_clear,
    .tp_members = Pos_members,
    .tp_methods = Pos_methods,
    .tp_getset = Pos_getsets,
};

typedef struct {
    PyObject_HEAD
    Pos3 pos;
} Pos3Object;

static int Pos3_traverse(Pos3Object *self, visitproc visit, void *arg) {
    return 0;
}

static int Pos3_clear(Pos3Object *self) {
    return 0;
}

static void Pos3_dealloc(Pos3Object *self) {
    PyObject_GC_UnTrack(self);
    Pos3_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Pos3_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Pos3Object *self;
    self = (Pos3Object *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->pos = (Pos3){0};
    }
    return (PyObject *) self;
}

static int Pos3_init(Pos3Object *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"x", "y", "z", NULL};

    int x, y, z;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "iii", kwlist, &x, &y, &z)) {
        return -1;
    }

    self->pos.x = x;
    self->pos.y = y;
    self->pos.z = z;

    return 0;
}

static PyObject *Pos3_get_x(Pos3Object *self)  {
    return PyLong_FromLong(self->pos.x);
}

static PyObject *Pos3_get_y(Pos3Object *self)  {
    return PyLong_FromLong(self->pos.y);
}

static PyObject *Pos3_get_z(Pos3Object *self)  {
    return PyLong_FromLong(self->pos.z);
}

static int Pos3_set_x(Pos3Object *self, PyObject *args)  {
    if (PyLong_Check(args)) {
        self->pos.x = PyLong_AsLong(args);
        return 0;
    }
    return -1;
}

static int Pos3_set_y(Pos3Object *self, PyObject *args)  {
    if (PyLong_Check(args)) {
        self->pos.y = PyLong_AsLong(args);
        return 0;
    }
    return -1;
}

static int Pos3_set_z(Pos3Object *self, PyObject *args)  {
    if (PyLong_Check(args)) {
        self->pos.z = PyLong_AsLong(args);
        return 0;
    }
    return -1;
}

static PyObject *Pos3_repr(Pos3Object *self)  {
    return PyUnicode_FromFormat("Pos3(x=%d, y=%d, z=%d)", self->pos.x, self->pos.y, self->pos.z);
}

static PyMemberDef Pos3_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Pos3_methods[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef Pos3_getsets[] = {
    {"x", (getter)Pos3_get_x, (setter)Pos3_set_x, NULL, NULL},
    {"y", (getter)Pos3_get_y, (setter)Pos3_set_y, NULL, NULL},
    {"z", (getter)Pos3_get_z, (setter)Pos3_set_z, NULL, NULL},
    {NULL, 0, NULL, NULL, NULL} /* Sentinel */
};

static PyTypeObject Pos3Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.Pos3",
    .tp_doc = "Pos3 objects",
    .tp_basicsize = sizeof(Pos3Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Pos3_new,
    .tp_init = (initproc) Pos3_init,
    .tp_dealloc = (destructor) Pos3_dealloc,
    .tp_repr = (reprfunc) Pos3_repr,
    .tp_traverse = (traverseproc) Pos3_traverse,
    .tp_clear = (inquiry) Pos3_clear,
    .tp_members = Pos3_members,
    .tp_methods = Pos3_methods,
    .tp_getset = Pos3_getsets,
};

static inline Pos3Object *Pos3_new_from_pos3(Pos3 p) {
    Pos3Object *ret = (Pos3Object *)Pos3_new(&Pos3Type, NULL, NULL);
    if (ret) {
        ret->pos = p;
    }
    return ret;
}
