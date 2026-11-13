#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

// Include the cubiomes header files directly
#include "../external/cubiomes/noise.h"

// Forward declare PyTypeObject since these types reference each other
extern PyTypeObject PerlinNoiseType;
extern PyTypeObject OctaveNoiseType;
extern PyTypeObject DoublePerlinNoiseType;

/******************************************************************************
 * PerlinNoise Object
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    PerlinNoise noise;
} PerlinNoiseObject;

static int PerlinNoise_init(PerlinNoiseObject *self, PyObject *args, PyObject *kwds) {
    self->noise = (PerlinNoise){0};
    return 0;
}

static void PerlinNoise_dealloc(PerlinNoiseObject *self) {
    PyObject_GC_UnTrack(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef PerlinNoise_members[] = {
    {"amplitude", T_DOUBLE, offsetof(PerlinNoiseObject, noise.amplitude), 0, "amplitude value"},
    {"lacunarity", T_DOUBLE, offsetof(PerlinNoiseObject, noise.lacunarity), 0, "lacunarity value"},
    {"d2", T_DOUBLE, offsetof(PerlinNoiseObject, noise.d2), 0, "d2 value"},
    {"t2", T_DOUBLE, offsetof(PerlinNoiseObject, noise.t2), 0, "t2 value"},
    {NULL}  /* Sentinel */
};

static PyMethodDef PerlinNoise_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject PerlinNoiseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.PerlinNoise",
    .tp_doc = "PerlinNoise object",
    .tp_basicsize = sizeof(PerlinNoiseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PerlinNoise_init,
    .tp_dealloc = (destructor)PerlinNoise_dealloc,
    .tp_members = PerlinNoise_members,
    .tp_methods = PerlinNoise_methods,
};


/******************************************************************************
 * OctaveNoise Object
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    OctaveNoise noise;
} OctaveNoiseObject;

static int OctaveNoise_init(OctaveNoiseObject *self, PyObject *args, PyObject *kwds) {
    self->noise = (OctaveNoise){0};
    return 0;
}

static void OctaveNoise_dealloc(OctaveNoiseObject *self) {
    PyObject_GC_UnTrack(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef OctaveNoise_members[] = {
    {"oct_count", T_INT, offsetof(OctaveNoiseObject, noise.octcnt), 0, "octave count"},
    {NULL}  /* Sentinel */
};

static PyMethodDef OctaveNoise_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject OctaveNoiseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.OctaveNoise",
    .tp_doc = "OctaveNoise object",
    .tp_basicsize = sizeof(OctaveNoiseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)OctaveNoise_init,
    .tp_dealloc = (destructor)OctaveNoise_dealloc,
    .tp_members = OctaveNoise_members,
    .tp_methods = OctaveNoise_methods,
};


/******************************************************************************
 * DoublePerlinNoise Object
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    DoublePerlinNoise noise;
} DoublePerlinNoiseObject;

static int DoublePerlinNoise_init(DoublePerlinNoiseObject *self, PyObject *args, PyObject *kwds) {
    self->noise = (DoublePerlinNoise){0};
    return 0;
}

static void DoublePerlinNoise_dealloc(DoublePerlinNoiseObject *self) {
    PyObject_GC_UnTrack(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef DoublePerlinNoise_members[] = {
    {"amplitude", T_DOUBLE, offsetof(DoublePerlinNoiseObject, noise.amplitude), 0, "amplitude value"},
    {NULL}  /* Sentinel */
};

static PyMethodDef DoublePerlinNoise_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject DoublePerlinNoiseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.DoublePerlinNoise",
    .tp_doc = "DoublePerlinNoise object",
    .tp_basicsize = sizeof(DoublePerlinNoiseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)DoublePerlinNoise_init,
    .tp_dealloc = (destructor)DoublePerlinNoise_dealloc,
    .tp_members = DoublePerlinNoise_members,
    .tp_methods = DoublePerlinNoise_methods,
};
