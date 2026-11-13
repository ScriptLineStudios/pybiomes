#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

// Include the cubiomes header files directly
#include "../external/cubiomes/noise.h"
#include "../external/cubiomes/biomenoise.h"

// Forward declare the PyTypeObject defined in noise.c
extern PyTypeObject PerlinNoiseType;
extern PyTypeObject OctaveNoiseType;
extern PyTypeObject DoublePerlinNoiseType;

/******************************************************************************
 * SurfaceNoise Object
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    SurfaceNoise noise;
} SurfaceNoiseObject;

static int SurfaceNoise_init(SurfaceNoiseObject *self, PyObject *args, PyObject *kwds) {
    self->noise = (SurfaceNoise){0};
    return 0;
}

static void SurfaceNoise_dealloc(SurfaceNoiseObject *self) {
    PyObject_GC_UnTrack(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef SurfaceNoise_members[] = {
    {"xzScale", T_DOUBLE, offsetof(SurfaceNoiseObject, noise.xzScale), 0, "xzScale value"},
    {"yScale", T_DOUBLE, offsetof(SurfaceNoiseObject, noise.yScale), 0, "yScale value"},
    {"xzFactor", T_DOUBLE, offsetof(SurfaceNoiseObject, noise.xzFactor), 0, "xzFactor value"},
    {"yFactor", T_DOUBLE, offsetof(SurfaceNoiseObject, noise.yFactor), 0, "yFactor value"},
    {NULL}  /* Sentinel */
};

/*
 * Wrapper function to initialize the SurfaceNoise struct.
 * This function is now a method of the SurfaceNoise object, so 'self'
 * is a pointer to the object instance.
 */
static PyObject* SurfaceNoise_init_surface_noise(SurfaceNoiseObject *self, PyObject *args) {
    int dim;
    uint64_t seed;

    if (!PyArg_ParseTuple(args, "iK", &dim, &seed)) {
        return NULL;
    }

    initSurfaceNoise(&self->noise, dim, seed);
    Py_RETURN_NONE;
}

static PyMethodDef SurfaceNoise_methods[] = {
    {"init_surface_noise", (PyCFunction)SurfaceNoise_init_surface_noise, METH_VARARGS, "Initializes a SurfaceNoise struct with a seed."},
    {NULL}  /* Sentinel */
};

PyTypeObject SurfaceNoiseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pybiomes.SurfaceNoise",
    .tp_doc = "SurfaceNoise object",
    .tp_basicsize = sizeof(SurfaceNoiseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)SurfaceNoise_init,
    .tp_dealloc = (destructor)SurfaceNoise_dealloc,
    .tp_members = SurfaceNoise_members,
    .tp_methods = SurfaceNoise_methods,
};
