#include <stdio.h>
#include <stdlib.h>

#include <Python.h>

#include "pybiomes.c"

#include "objects/range.c"
#include "objects/noise.c"
#include "objects/biomenoise.c"
#include "objects/generator.c"
#include "objects/position.c"
#include "objects/finder.c"
#include "objects/rng.c"

#include "modules/versions.c"
#include "modules/dimensions.c"
#include "modules/biomes.c"
#include "modules/structures.c"

static PyMethodDef base_methods[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pybiomes = {
    PyModuleDef_HEAD_INIT,
    "core", /* library name */
    "core module", /* module docs */
    -1,
    base_methods,
};

PyMODINIT_FUNC PyInit_pybiomes(void){
    if (PyType_Ready(&GeneratorType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&RangeType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&FinderType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&PosType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&RngType) < 0) {
        return NULL;
    }

    if (PyType_Ready(&XoroshiroType) < 0) {
        return NULL;
    }
    // Noise module objects
    if (PyType_Ready(&PerlinNoiseType) < 0) {
        return NULL;
    }
    if (PyType_Ready(&OctaveNoiseType) < 0) {
        return NULL;
    }
    if (PyType_Ready(&DoublePerlinNoiseType) < 0) {
        return NULL;
    }

    // Biome noise module objects
    if (PyType_Ready(&SurfaceNoiseType) < 0) {
        return NULL;
    }
    
    PyObject *base = PyModule_Create(&pybiomes);

    PyObject *versions = PyInit_versions(&pybiomes);
    PyObject *dimensions = PyInit_dimensions(&pybiomes);
    PyObject *biomes = PyInit_biomes(&pybiomes);
    PyObject *structures = PyInit_structures(&pybiomes);

    PyObject *moduleDict = PyImport_GetModuleDict();

    Py_INCREF(versions);
    PyDict_SetItemString(moduleDict, "pybiomes.versions", versions);
    PyModule_AddObject(base, "versions", versions);

    Py_INCREF(dimensions);
    PyDict_SetItemString(moduleDict, "pybiomes.dimensions", dimensions);
    PyModule_AddObject(base, "dimensions", dimensions);

    Py_INCREF(structures);
    PyDict_SetItemString(moduleDict, "pybiomes.structures", structures);
    PyModule_AddObject(base, "structures", structures);

    Py_INCREF(biomes);
    PyDict_SetItemString(moduleDict, "pybiomes.biomes", biomes);
    PyModule_AddObject(base, "biomes", biomes);

    Py_INCREF(&GeneratorType);
    PyModule_AddObject(base, "Generator", (PyObject *)&GeneratorType);

    Py_INCREF(&RangeType);
    PyModule_AddObject(base, "Range", (PyObject *)&RangeType);

    Py_INCREF(&FinderType);
    PyModule_AddObject(base, "Finder", (PyObject *)&FinderType);

    Py_INCREF(&PosType);
    PyModule_AddObject(base, "Pos", (PyObject *)&PosType);
	
    Py_INCREF(&RngType);
    PyModule_AddObject(base, "Rng", (PyObject *)&RngType);
	
    Py_INCREF(&XoroshiroType);
    PyModule_AddObject(base, "Xoroshiro", (PyObject *)&XoroshiroType);
	
    Py_INCREF(&PerlinNoiseType);
    PyModule_AddObject(base, "PerlinNoise", (PyObject *)&PerlinNoiseType);
    
    Py_INCREF(&OctaveNoiseType);
    PyModule_AddObject(base, "OctaveNoise", (PyObject *)&OctaveNoiseType);
    
    Py_INCREF(&DoublePerlinNoiseType);
    PyModule_AddObject(base, "DoublePerlinNoise", (PyObject *)&DoublePerlinNoiseType);
    
    Py_INCREF(&SurfaceNoiseType);
    PyModule_AddObject(base, "SurfaceNoise", (PyObject *)&SurfaceNoiseType);

    return base; 
}