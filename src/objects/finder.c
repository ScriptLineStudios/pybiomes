#include <stdio.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

// Have to manually declare the function prototype here since it's not declared in
// finders.h, and causes issues when Python has to guess the return type.
uint64_t getPopulationSeed(int mc, uint64_t ws, int x, int z);

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

static PyObject *Finder_set_attempt_seed(FinderObject *self, PyObject *args) {
    uint64_t s;
    int cx, cz;
    
    if (!PyArg_ParseTuple(args, "Kii", &s, &cx, &cz)) {
        return NULL;
    }
    setAttemptSeed(&s, cx, cz);
    return PyLong_FromUnsignedLongLong(s);
}

static PyObject *Finder_get_population_seed(FinderObject *self, PyObject *args) {
    long long ws;
    int x, z;
    
    if (!PyArg_ParseTuple(args, "Kii", &ws, &x, &z)) {
        return NULL;
    }
    uint64_t result = getPopulationSeed(self->version, ws, x, z);
    return PyLong_FromUnsignedLongLong(result);
}

static PyObject *Finder_get_structure_config(FinderObject *self, PyObject *args) {
	int structureType;

    if (!PyArg_ParseTuple(args, "i", &structureType)) {
        return NULL;
    }
	StructureConfig sc;
	if (!getStructureConfig(structureType, self->version, &sc)) {
		Py_RETURN_NONE;
	}
	
    PyObject *dict = PyDict_New();

    // Add struct fields to dictionary
    PyDict_SetItemString(dict, "salt", PyLong_FromLong(sc.salt));
    PyDict_SetItemString(dict, "regionSize", PyLong_FromLong(sc.regionSize));
    PyDict_SetItemString(dict, "chunkRange", PyLong_FromLong(sc.chunkRange));
    PyDict_SetItemString(dict, "structType", PyLong_FromLong(sc.structType));
    PyDict_SetItemString(dict, "dim", PyLong_FromLong(sc.dim));
    PyDict_SetItemString(dict, "rarity", PyFloat_FromDouble(sc.rarity));
	
	return dict;
	
}

static PyObject *Finder_is_stronghold_biome(FinderObject *self, PyObject *args) {
	int id;

    if (!PyArg_ParseTuple(args, "i", &id)) {
        return NULL;
    }
	
	return PyBool_FromLong(isStrongholdBiome(self->version, id));
	
}

static PyObject *Finder_is_viable_feature_biome(FinderObject *self, PyObject *args) {
    int structType, biomeID;

    if (!PyArg_ParseTuple(args, "ii", &structType, &biomeID)) {
        return NULL;
    }

    return PyBool_FromLong(isViableFeatureBiome(self->version, structType, biomeID));
}

static PyObject *Finder_init_first_stronghold(FinderObject *self, PyObject *args) {
    uint64_t s48;

    if (!PyArg_ParseTuple(args, "K", &s48)) {
        return NULL;
    }
	StrongholdIter sh;
	initFirstStronghold(&sh, self->version, s48);
	
    PosObject *pos = Pos_new(&PosType, NULL, NULL);
    pos->pos.x = sh.pos.x;
    pos->pos.z = sh.pos.z;
	
    PosObject *nextapprox = Pos_new(&PosType, NULL, NULL);
  
    nextapprox->pos.x = sh.nextapprox.x;
    nextapprox->pos.z = sh.nextapprox.z;
	
	
    PyObject *dict = PyDict_New();

    // Add struct fields to dictionary
    PyDict_SetItemString(dict, "pos", (PyObject *)pos);
    PyDict_SetItemString(dict, "nextapprox", (PyObject *)nextapprox);
    PyDict_SetItemString(dict, "index", PyLong_FromLong(sh.index));
    PyDict_SetItemString(dict, "ringnum", PyLong_FromLong(sh.ringnum));
    PyDict_SetItemString(dict, "ringmax", PyLong_FromLong(sh.ringmax));
    PyDict_SetItemString(dict, "ringidx", PyLong_FromLong(sh.ringidx));
    PyDict_SetItemString(dict, "angle", PyFloat_FromDouble(sh.angle));
    PyDict_SetItemString(dict, "dist", PyFloat_FromDouble(sh.dist));
    PyDict_SetItemString(dict, "rnds", PyLong_FromLongLong(sh.rnds));
	

    PyObject *py_tuple = PyTuple_New(2);
	PyTuple_SetItem(py_tuple, 0, nextapprox);  // Steals a reference to PosObject
    PyTuple_SetItem(py_tuple, 1, dict);  // Steals a reference to py_dict
	
	return py_tuple;
}


static PyObject *Finder_next_stronghold(FinderObject *self, PyObject *args) {
    PyObject *dict_obj;
    PyObject *gen_obj;

    // Parse the arguments as a PyObject (dictionary) and GENERATOR_OBJECT_TYPE
    if (!PyArg_ParseTuple(args, "O" GENERATOR_OBJECT_TYPE, &dict_obj, &GeneratorType, &gen_obj)) {
        return NULL;
    }

    // Check if the first argument is a dictionary
    if (!PyDict_Check(dict_obj)) {
        PyErr_SetString(PyExc_TypeError, "First parameter must be a dictionary");
        return NULL;
    }

    // Check if the second argument is a GeneratorObject
    if (!PyObject_TypeCheck(gen_obj, &GeneratorType)) {
        PyErr_SetString(PyExc_TypeError, "Second parameter must be a GeneratorObject");
        return NULL;
    }

    // Extract the 'pos' value from the dictionary
    PyObject *pos_obj = PyDict_GetItemString(dict_obj, "pos");
    if (!pos_obj || !PyObject_TypeCheck(pos_obj, &PosType)) {
        PyErr_SetString(PyExc_TypeError, "Dictionary must contain 'pos' key with a PosObject");
        return NULL;
    }

    // Extract other values from the dictionary
    PyObject *nextapprox_obj = PyDict_GetItemString(dict_obj, "nextapprox");
    if (!nextapprox_obj || !PyObject_TypeCheck(nextapprox_obj, &PosType)) {
        PyErr_SetString(PyExc_TypeError, "Dictionary must contain 'nextapprox' key with a PosObject");
        return NULL;
    }

    long index = PyLong_AsLong(PyDict_GetItemString(dict_obj, "index"));
    long ringnum = PyLong_AsLong(PyDict_GetItemString(dict_obj, "ringnum"));
    long ringmax = PyLong_AsLong(PyDict_GetItemString(dict_obj, "ringmax"));
    long ringidx = PyLong_AsLong(PyDict_GetItemString(dict_obj, "ringidx"));
    double angle = PyFloat_AsDouble(PyDict_GetItemString(dict_obj, "angle"));
    double dist = PyFloat_AsDouble(PyDict_GetItemString(dict_obj, "dist"));
    unsigned long long rnds = PyLong_AsUnsignedLongLong(PyDict_GetItemString(dict_obj, "rnds"));

    // Access the C 'Pos' structures
    PosObject *pos = (PosObject *)pos_obj;
    PosObject *nextapprox = (PosObject *)nextapprox_obj;
	
	
	StrongholdIter sh;
	sh.pos = pos->pos;
	sh.nextapprox = nextapprox->pos;
	sh.index = index;
	sh.ringnum = ringnum;
	sh.ringmax = ringmax;
	sh.ringidx = ringidx;
	sh.angle = angle;
	sh.dist = dist;
	sh.rnds = rnds;
	sh.mc = self->version;
	
	GeneratorObject *generator_obj = (GeneratorObject *)gen_obj;
	Generator g = generator_obj->generator;

	
	int valid = nextStronghold(&sh, &g);
	
    PosObject *posOut = Pos_new(&PosType, NULL, NULL);
    posOut->pos.x = sh.pos.x;
    posOut->pos.z = sh.pos.z;
	
    PosObject *nextapproxOut = Pos_new(&PosType, NULL, NULL);
  
    nextapproxOut->pos.x = sh.nextapprox.x;
    nextapproxOut->pos.z = sh.nextapprox.z;
	
    PyObject *dict = PyDict_New();

    // Add struct fields to dictionary
    PyDict_SetItemString(dict, "pos", (PyObject *)posOut);
    PyDict_SetItemString(dict, "nextapprox", (PyObject *)nextapproxOut);
    PyDict_SetItemString(dict, "index", PyLong_FromLong(sh.index));
    PyDict_SetItemString(dict, "ringnum", PyLong_FromLong(sh.ringnum));
    PyDict_SetItemString(dict, "ringmax", PyLong_FromLong(sh.ringmax));
    PyDict_SetItemString(dict, "ringidx", PyLong_FromLong(sh.ringidx));
    PyDict_SetItemString(dict, "angle", PyFloat_FromDouble(sh.angle));
    PyDict_SetItemString(dict, "dist", PyFloat_FromDouble(sh.dist));
    PyDict_SetItemString(dict, "rnds", PyLong_FromLongLong(sh.rnds));
	
	PyObject *py_bool = PyBool_FromLong((long)valid);
    if (!py_bool) {
        return NULL;  // Return NULL to indicate an error
    }
	
    PyObject *py_tuple = PyTuple_New(2);
	PyTuple_SetItem(py_tuple, 0, py_bool);  // Steals a reference to py_bool
    PyTuple_SetItem(py_tuple, 1, dict);  // Steals a reference to py_dict
	
	return py_tuple;
}


static PyObject *Finder_get_spawn(FinderObject *self, PyObject *args) {
    PyObject *gen_obj;

    if (!PyArg_ParseTuple(args, "O!", &GeneratorType, &gen_obj)) {
        return NULL;
    }
    // Check if the argument is a GeneratorObject
    if (!PyObject_TypeCheck(gen_obj, &GeneratorType)) {
        PyErr_SetString(PyExc_TypeError, "Parameter must be a GeneratorObject");
        return NULL;
    }
	
	GeneratorObject *generator_obj = (GeneratorObject *)gen_obj;
	Generator g = generator_obj->generator;
	
    Pos spawn_pos = getSpawn(&g);
    PosObject *ret = Pos_new(&PosType, NULL, NULL);
  
    ret->pos.x = spawn_pos.x;
    ret->pos.z = spawn_pos.z;
	return (PyObject *)ret;
}

static PyObject *Finder_estimate_spawn(FinderObject *self, PyObject *args) {
    PyObject *gen_obj;

    if (!PyArg_ParseTuple(args, "O!", &GeneratorType, &gen_obj)) {
        return NULL;
    }
    if (!PyObject_TypeCheck(gen_obj, &GeneratorType)) {
        PyErr_SetString(PyExc_TypeError, "Parameter must be a GeneratorObject");
        return NULL;
    }

    GeneratorObject *generator_obj = (GeneratorObject *)gen_obj;
    Generator g = generator_obj->generator;

    Pos spawn_pos = estimateSpawn(&g, NULL);
    PosObject *ret = Pos_new(&PosType, NULL, NULL);

    ret->pos.x = spawn_pos.x;
    ret->pos.z = spawn_pos.z;
    return (PyObject *)ret;
}


static PyObject *Finder_chunk_generate_rnd(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX;
	int chunkZ;

    if (!PyArg_ParseTuple(args, "Kii", &seed, &chunkX, &chunkZ)) {
        return NULL;
    }
	uint64_t rnd = chunkGenerateRnd(seed, chunkX, chunkZ);
    return PyLong_FromLongLong(rnd);
}

static PyObject *Finder_is_slime_chunk(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;

    if (!PyArg_ParseTuple(args, "Kii", &seed, &chunkX, &chunkZ)) {
        return NULL;
    }

    return PyBool_FromLong(isSlimeChunk(seed, chunkX, chunkZ));
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

static PyObject *Finder_get_variant(FinderObject *self, PyObject *args) {
    int structType;
    unsigned long long seed;
    int blockX, blockZ, biomeID;

    if (!PyArg_ParseTuple(args, "iKiii", &structType, &seed, &blockX, &blockZ, &biomeID)) {
        return NULL;
    }

    StructureVariant sv;
    int success = getVariant(&sv, structType, self->version, seed, blockX, blockZ, biomeID);

    if (success == 0) {
        Py_RETURN_NONE; 
    }

    PyObject *dict = PyDict_New();
    if (!dict) {
        return PyErr_NoMemory();
    }

    PyDict_SetItemString(dict, "abandoned", PyBool_FromLong(sv.abandoned));
    PyDict_SetItemString(dict, "giant", PyBool_FromLong(sv.giant));
    PyDict_SetItemString(dict, "underground", PyBool_FromLong(sv.underground));
    PyDict_SetItemString(dict, "airpocket", PyBool_FromLong(sv.airpocket));
    PyDict_SetItemString(dict, "basement", PyBool_FromLong(sv.basement));
    PyDict_SetItemString(dict, "cracked", PyBool_FromLong(sv.cracked));
    PyDict_SetItemString(dict, "size", PyLong_FromLong(sv.size));
    PyDict_SetItemString(dict, "start", PyLong_FromLong(sv.start));
    PyDict_SetItemString(dict, "biome", PyLong_FromLong(sv.biome));
    PyDict_SetItemString(dict, "rotation", PyLong_FromLong(sv.rotation));
    PyDict_SetItemString(dict, "mirror", PyLong_FromLong(sv.mirror));
    PyDict_SetItemString(dict, "x", PyLong_FromLong(sv.x));
    PyDict_SetItemString(dict, "y", PyLong_FromLong(sv.y));
    PyDict_SetItemString(dict, "z", PyLong_FromLong(sv.z));
    PyDict_SetItemString(dict, "sx", PyLong_FromLong(sv.sx));
    PyDict_SetItemString(dict, "sy", PyLong_FromLong(sv.sy));
    PyDict_SetItemString(dict, "sz", PyLong_FromLong(sv.sz));

    return dict;
}

static PyMemberDef Finder_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Finder_methods[] = {
    {"set_attempt_seed", (PyCFunction)Finder_set_attempt_seed, METH_VARARGS, "Sets an attempt seed from a population seed and coordinates"},
    {"get_population_seed", (PyCFunction)Finder_get_population_seed, METH_VARARGS, "Generates a population seed from a world seed and coordinates."},
    {"get_structure_config", (PyCFunction)Finder_get_structure_config, METH_VARARGS, "Finds a structure's configuration parameters"},
    {"is_stronghold_biome", (PyCFunction)Finder_is_stronghold_biome, METH_VARARGS, "Checks if the biome is valid for stronghold placement"},
    {"is_viable_feature_biome", (PyCFunction)Finder_is_viable_feature_biome, METH_VARARGS, "Checks if the biome is valid for feature placement"},
    {"init_first_stronghold", (PyCFunction)Finder_init_first_stronghold, METH_VARARGS, "Initialises first stronghold"},
    {"next_stronghold", (PyCFunction)Finder_next_stronghold, METH_VARARGS, "Finds next stronghold"},
    {"get_spawn", (PyCFunction)Finder_get_spawn, METH_VARARGS, "Gets world spawn position"},
    {"estimate_spawn", (PyCFunction)Finder_estimate_spawn, METH_VARARGS, "Estimates world spawn position"},
    {"chunk_generate_rnd", (PyCFunction)Finder_chunk_generate_rnd, METH_VARARGS, "Initialises and returns a random seed used in the chunk generation"},
    {"is_slime_chunk", (PyCFunction)Finder_is_slime_chunk, METH_VARARGS, "Checks if a chunk is a slime chunk"},
    {"get_structure_pos", (PyCFunction)Finder_get_structure_pos, METH_VARARGS, "Finds a structures position within the given region"},
	{"get_variant", (PyCFunction)Finder_get_variant, METH_VARARGS, "Gets a structures variant data (rotation, bounding box, etc.)"},
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
