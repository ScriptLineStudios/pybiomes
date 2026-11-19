#include <stdio.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

// Have to manually declare the function prototype here since it's not declared in
// finders.h, and causes issues when Python has to guess the return type.
uint64_t getPopulationSeed(int mc, uint64_t ws, int x, int z);
int isStrongholdBiome(int mc, int id);

#include "../external/cubiomes/finders.h"
#include "../external/cubiomes/features/stronghold.h"
#include "../external/cubiomes/features/fortress.h"
#include "../external/cubiomes/features/mineshaft.h"
#include "../external/cubiomes/features/end_city.h"
#include "../external/cubiomes/features/abandoned_camp.h"

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

static PyObject *Finder_get_structure_salt_config(FinderObject *self, PyObject *args) {
    int structureType, biomeID;

    if (!PyArg_ParseTuple(args, "ii", &structureType, &biomeID)) {
        return NULL;
    }

    StructureSaltConfig ssc;
    if (!getStructureSaltConfig(structureType, self->version, biomeID, &ssc)) {
        Py_RETURN_NONE;
    }

    PyObject *dict = PyDict_New();
    if (!dict) {
        return PyErr_NoMemory();
    }

    PyDict_SetItemString(dict, "generationStep", PyLong_FromLong(ssc.generationStep));
    PyDict_SetItemString(dict, "decoratorIndex", PyLong_FromLong(ssc.decoratorIndex));

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

	int valid = nextStronghold(&sh, &generator_obj->generator);
	
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
	
    Pos spawn_pos = getSpawn(&generator_obj->generator);
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

    Pos spawn_pos = estimateSpawn(&generator_obj->generator, NULL);
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

static PyObject *Finder_get_fixed_end_gateways(FinderObject *self, PyObject *args) {
    uint64_t seed;

    if (!PyArg_ParseTuple(args, "K", &seed)) {
        return NULL;
    }

    if (self->version < MC_1_9) {
        Py_RETURN_NONE;
    }

    Pos gateways[20];
    getFixedEndGateways(self->version, seed, gateways);

    PyObject *list = PyList_New(20);
    if (!list) {
        return PyErr_NoMemory();
    }

    for (int i = 0; i < 20; i++) {
        PosObject *p = Pos_new(&PosType, NULL, NULL);
        if (!p) {
            Py_DECREF(list);
            return NULL;
        }
        p->pos.x = gateways[i].x;
        p->pos.z = gateways[i].z;
        PyList_SetItem(list, i, (PyObject *)p);
    }

    return list;
}

static PyObject *Finder_get_linked_gateway_pos(FinderObject *self, PyObject *args) {
    PyObject *gen_obj;
    PyObject *arg2;
    int src_x, src_z;
    PyObject *sn_obj = NULL;

    if (PyArg_ParseTuple(args, "O!ii|O", &GeneratorType, &gen_obj, &src_x, &src_z, &sn_obj)) {
        // (generator, src_x, src_z, [surface_noise])
    } else {
        PyErr_Clear();
        if (PyArg_ParseTuple(args, "O!O!|O", &GeneratorType, &gen_obj, &PosType, &arg2, &sn_obj)) {
            PosObject *pos = (PosObject *)arg2;
            src_x = pos->pos.x;
            src_z = pos->pos.z;
        } else {
            return NULL;
        }
    }

    if (self->version < MC_1_9) {
        Py_RETURN_NONE;
    }

    GeneratorObject *gen = (GeneratorObject *)gen_obj;
    SurfaceNoise sn_storage;
    SurfaceNoise *sn = NULL;

    if (sn_obj && sn_obj != Py_None) {
        if (!PyObject_TypeCheck(sn_obj, &SurfaceNoiseType)) {
            PyErr_SetString(PyExc_TypeError, "Parameter must be a SurfaceNoise object");
            return NULL;
        }
        sn = &((SurfaceNoiseObject *)sn_obj)->noise;
    } else {
        initSurfaceNoise(&sn_storage, DIM_END, gen->generator.seed);
        sn = &sn_storage;
    }

    Pos src = {src_x, src_z};
    Pos dst = getLinkedGatewayPos(&gen->generator.en, sn, gen->generator.seed, src);

    PosObject *ret = Pos_new(&PosType, NULL, NULL);
    ret->pos.x = dst.x;
    ret->pos.z = dst.z;
    return (PyObject *)ret;
}

static PyObject *Finder_is_end_chunk_empty(FinderObject *self, PyObject *args) {
    PyObject *gen_obj;
    int chunkX, chunkZ;
    PyObject *sn_obj = NULL;

    if (!PyArg_ParseTuple(args, "O!ii|O", &GeneratorType, &gen_obj, &chunkX, &chunkZ, &sn_obj)) {
        return NULL;
    }

    if (self->version < MC_1_9) {
        Py_RETURN_NONE;
    }

    GeneratorObject *gen = (GeneratorObject *)gen_obj;
    SurfaceNoise sn_storage;
    SurfaceNoise *sn = NULL;

    if (sn_obj && sn_obj != Py_None) {
        if (!PyObject_TypeCheck(sn_obj, &SurfaceNoiseType)) {
            PyErr_SetString(PyExc_TypeError, "Parameter must be a SurfaceNoise object");
            return NULL;
        }
        sn = &((SurfaceNoiseObject *)sn_obj)->noise;
    } else {
        initSurfaceNoise(&sn_storage, DIM_END, gen->generator.seed);
        sn = &sn_storage;
    }

    int empty = isEndChunkEmpty(&gen->generator.en, sn, gen->generator.seed, chunkX, chunkZ);
    return PyBool_FromLong(empty);
}

static PyObject *Finder_get_end_islands(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;

    if (!PyArg_ParseTuple(args, "Kii", &seed, &chunkX, &chunkZ)) {
        return NULL;
    }

    if (self->version < MC_1_9) {
        Py_RETURN_NONE;
    }

    EndIsland islands[2];
    int count = getEndIslands(islands, self->version, seed, chunkX, chunkZ);

    PyObject *list = PyList_New(count);
    if (!list) {
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyObject *dict = PyDict_New();
        PyDict_SetItemString(dict, "x", PyLong_FromLong(islands[i].x));
        PyDict_SetItemString(dict, "z", PyLong_FromLong(islands[i].z));
        PyDict_SetItemString(dict, "r", PyLong_FromLong(islands[i].r));
        PyList_SetItem(list, i, dict);
    }

    return list;
}

static PyObject *piece_to_dict(const Piece *p) {
    PyObject *dict = PyDict_New();
    if (!dict) return NULL;

    if (p->name) {
        PyDict_SetItemString(dict, "name", PyUnicode_FromString(p->name));
    } else {
        Py_INCREF(Py_None);
        PyDict_SetItemString(dict, "name", Py_None);
    }

    Pos3Object *pos = Pos3_new_from_pos3(p->pos);
    PyDict_SetItemString(dict, "pos", (PyObject *)pos);
    Py_DECREF(pos);

    Pos3Object *bb0 = Pos3_new_from_pos3(p->bb0);
    PyDict_SetItemString(dict, "bb0", (PyObject *)bb0);
    Py_DECREF(bb0);

    Pos3Object *bb1 = Pos3_new_from_pos3(p->bb1);
    PyDict_SetItemString(dict, "bb1", (PyObject *)bb1);
    Py_DECREF(bb1);

    PyDict_SetItemString(dict, "rot", PyLong_FromLong(p->rot));
    PyDict_SetItemString(dict, "depth", PyLong_FromLong(p->depth));
    PyDict_SetItemString(dict, "type", PyLong_FromLong(p->type));

    int chestCount = p->chestCount;
    if (chestCount > 4) chestCount = 4;
    if (chestCount > 0) {
        PyObject *chests = PyList_New(chestCount);
        for (int i = 0; i < chestCount; i++) {
            PyObject *cdict = PyDict_New();
            PosObject *cp = (PosObject *)Pos_new(&PosType, NULL, NULL);
            if (cp) {
                cp->pos = p->chestPoses[i];
            }
            PyDict_SetItemString(cdict, "pos", (PyObject *)cp);
            Py_DECREF(cp);
            if (p->lootTables[i]) {
                PyDict_SetItemString(cdict, "loot_table", PyUnicode_FromString(p->lootTables[i]));
            } else {
                Py_INCREF(Py_None);
                PyDict_SetItemString(cdict, "loot_table", Py_None);
            }
            PyDict_SetItemString(cdict, "loot_seed", PyLong_FromUnsignedLongLong(p->lootSeeds[i]));
            PyList_SetItem(chests, i, cdict);
        }
        PyDict_SetItemString(dict, "chests", chests);
        Py_DECREF(chests);
    } else {
        PyObject *empty = PyList_New(0);
        PyDict_SetItemString(dict, "chests", empty);
        Py_DECREF(empty);
    }

    PyDict_SetItemString(dict, "additional_data", PyLong_FromLong(p->additionalData));

    return dict;
}

static PyObject *Finder_get_stronghold_pieces(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;
    int n = 400;

    if (!PyArg_ParseTuple(args, "Kii|i", &seed, &chunkX, &chunkZ, &n)) {
        return NULL;
    }

    if (self->version < MC_B1_8 || n <= 0) {
        Py_RETURN_NONE;
    }

    Piece *pieces = (Piece *)calloc(n, sizeof(Piece));
    if (!pieces) {
        return PyErr_NoMemory();
    }

    int count = getStrongholdPieces(pieces, n, self->version, seed, chunkX, chunkZ);
    if (count < 0) {
        free(pieces);
        Py_RETURN_NONE;
    }

    PyObject *list = PyList_New(count);
    if (!list) {
        free(pieces);
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyList_SetItem(list, i, piece_to_dict(&pieces[i]));
    }

    free(pieces);
    return list;
}

static PyObject *Finder_get_fortress_pieces(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;
    int n = 400;

    if (!PyArg_ParseTuple(args, "Kii|i", &seed, &chunkX, &chunkZ, &n)) {
        return NULL;
    }

    if (self->version < MC_1_0_0 || n <= 0) {
        Py_RETURN_NONE;
    }

    Piece *pieces = (Piece *)calloc(n, sizeof(Piece));
    if (!pieces) {
        return PyErr_NoMemory();
    }

    int count = getFortressPieces(pieces, n, self->version, seed, chunkX, chunkZ);
    if (count < 0) {
        free(pieces);
        Py_RETURN_NONE;
    }

    PyObject *list = PyList_New(count);
    if (!list) {
        free(pieces);
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyList_SetItem(list, i, piece_to_dict(&pieces[i]));
    }

    free(pieces);
    return list;
}

static PyObject *Finder_get_mineshaft_pieces(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;
    int n = 400;

    if (!PyArg_ParseTuple(args, "Kii|i", &seed, &chunkX, &chunkZ, &n)) {
        return NULL;
    }

    if (self->version < MC_B1_8 || n <= 0) {
        Py_RETURN_NONE;
    }

    Piece *pieces = (Piece *)calloc(n, sizeof(Piece));
    if (!pieces) {
        return PyErr_NoMemory();
    }

    int count = getMineshaftPieces(pieces, n, self->version, seed, chunkX, chunkZ);
    if (count < 0) {
        free(pieces);
        Py_RETURN_NONE;
    }

    PyObject *list = PyList_New(count);
    if (!list) {
        free(pieces);
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyList_SetItem(list, i, piece_to_dict(&pieces[i]));
    }

    free(pieces);
    return list;
}

static PyObject *Finder_get_end_city_pieces(FinderObject *self, PyObject *args) {
    uint64_t seed;
    int chunkX, chunkZ;

    if (!PyArg_ParseTuple(args, "Kii", &seed, &chunkX, &chunkZ)) {
        return NULL;
    }

    if (self->version < MC_1_9) {
        Py_RETURN_NONE;
    }

    Piece *pieces = (Piece *)calloc(END_CITY_PIECES_MAX, sizeof(Piece));
    if (!pieces) {
        return PyErr_NoMemory();
    }

    int count = getEndCityPieces(pieces, seed, chunkX, chunkZ);
    if (count < 0) {
        free(pieces);
        Py_RETURN_NONE;
    }

    PyObject *list = PyList_New(count);
    if (!list) {
        free(pieces);
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyList_SetItem(list, i, piece_to_dict(&pieces[i]));
    }

    free(pieces);
    return list;
}

static PyObject *Finder_get_structure_pieces(FinderObject *self, PyObject *args) {
    int structType;
    uint64_t seed;
    int posX, posZ;
    PyObject *salt_obj = NULL;
    PyObject *variant_obj = NULL;
    int n = 400;

    if (!PyArg_ParseTuple(args, "iKii|OOi", &structType, &seed, &posX, &posZ, &salt_obj, &variant_obj, &n)) {
        return NULL;
    }

    StructureConfig sc;
    if (!getStructureConfig(structType, self->version, &sc)) {
        Py_RETURN_NONE;
    }

    StructureSaltConfig ssc = {0};
    if (salt_obj && salt_obj != Py_None) {
        if (!PyDict_Check(salt_obj)) {
            PyErr_SetString(PyExc_TypeError, "salt_config must be a dict or None");
            return NULL;
        }
        PyObject *gs = PyDict_GetItemString(salt_obj, "generationStep");
        PyObject *di = PyDict_GetItemString(salt_obj, "decoratorIndex");
        if (gs && PyLong_Check(gs)) ssc.generationStep = PyLong_AsLong(gs);
        if (di && PyLong_Check(di)) ssc.decoratorIndex = PyLong_AsLong(di);
    } else {
        getStructureSaltConfig(structType, self->version, -1, &ssc);
    }

    StructureVariant sv = {0};
    if (variant_obj && variant_obj != Py_None) {
        if (!PyDict_Check(variant_obj)) {
            PyErr_SetString(PyExc_TypeError, "variant must be a dict or None");
            return NULL;
        }
        PyObject *item;
        if ((item = PyDict_GetItemString(variant_obj, "abandoned")) && PyBool_Check(item)) sv.abandoned = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "giant")) && PyBool_Check(item)) sv.giant = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "underground")) && PyBool_Check(item)) sv.underground = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "airpocket")) && PyBool_Check(item)) sv.airpocket = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "basement")) && PyBool_Check(item)) sv.basement = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "cracked")) && PyBool_Check(item)) sv.cracked = (item == Py_True);
        if ((item = PyDict_GetItemString(variant_obj, "size")) && PyLong_Check(item)) sv.size = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "start")) && PyLong_Check(item)) sv.start = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "biome")) && PyLong_Check(item)) sv.biome = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "rotation")) && PyLong_Check(item)) sv.rotation = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "mirror")) && PyLong_Check(item)) sv.mirror = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "x")) && PyLong_Check(item)) sv.x = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "y")) && PyLong_Check(item)) sv.y = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "z")) && PyLong_Check(item)) sv.z = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "sx")) && PyLong_Check(item)) sv.sx = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "sy")) && PyLong_Check(item)) sv.sy = PyLong_AsLong(item);
        if ((item = PyDict_GetItemString(variant_obj, "sz")) && PyLong_Check(item)) sv.sz = PyLong_AsLong(item);
    } else {
        getVariant(&sv, structType, self->version, seed, posX, posZ, -1);
    }

    if (n <= 0) n = 400;

    Piece *pieces = (Piece *)calloc(n, sizeof(Piece));
    if (!pieces) {
        return PyErr_NoMemory();
    }

    int count = getStructurePieces(pieces, n, structType, ssc, &sv, self->version, seed, posX, posZ);
    if (count <= 0) {
        free(pieces);
        return PyList_New(0);
    }

    PyObject *list = PyList_New(count);
    if (!list) {
        free(pieces);
        return PyErr_NoMemory();
    }

    for (int i = 0; i < count; i++) {
        PyList_SetItem(list, i, piece_to_dict(&pieces[i]));
    }

    free(pieces);
    return list;
}

static PyMemberDef Finder_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Finder_methods[] = {
    {"set_attempt_seed", (PyCFunction)Finder_set_attempt_seed, METH_VARARGS, "Sets an attempt seed from a population seed and coordinates"},
    {"get_population_seed", (PyCFunction)Finder_get_population_seed, METH_VARARGS, "Generates a population seed from a world seed and coordinates."},
    {"get_structure_config", (PyCFunction)Finder_get_structure_config, METH_VARARGS, "Finds a structure's configuration parameters"},
    {"get_structure_salt_config", (PyCFunction)Finder_get_structure_salt_config, METH_VARARGS, "Gets structure generation step and decorator index"},
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
    {"get_fixed_end_gateways", (PyCFunction)Finder_get_fixed_end_gateways, METH_VARARGS, "Generates positions of the 20 central End return gateways"},
    {"get_linked_gateway_pos", (PyCFunction)Finder_get_linked_gateway_pos, METH_VARARGS, "Calculates teleportation destination from an End gateway"},
    {"is_end_chunk_empty", (PyCFunction)Finder_is_end_chunk_empty, METH_VARARGS, "Tests if an End chunk is empty void"},
    {"get_end_islands", (PyCFunction)Finder_get_end_islands, METH_VARARGS, "Gets small End island features in a chunk"},
    {"get_stronghold_pieces", (PyCFunction)Finder_get_stronghold_pieces, METH_VARARGS, "Generates rooms and corridors for a Stronghold"},
    {"get_fortress_pieces", (PyCFunction)Finder_get_fortress_pieces, METH_VARARGS, "Generates walkways and rooms for a Nether Fortress"},
    {"get_mineshaft_pieces", (PyCFunction)Finder_get_mineshaft_pieces, METH_VARARGS, "Generates corridors and rooms for a Mineshaft"},
    {"get_end_city_pieces", (PyCFunction)Finder_get_end_city_pieces, METH_VARARGS, "Generates towers and rooms for an End City"},
    {"get_structure_pieces", (PyCFunction)Finder_get_structure_pieces, METH_VARARGS, "Generates 3D structure pieces and chest loot for a structure"},
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
