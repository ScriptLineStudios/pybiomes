#include <Python.h>

PyMODINIT_FUNC PyInit_structures(PyModuleDef *pybiomes) {
    PyObject *mod = PyModule_Create(pybiomes);
    
    PyModule_AddIntMacro(mod, Desert_Pyramid);
    PyModule_AddIntMacro(mod, Jungle_Temple);
    PyModule_AddIntMacro(mod, Jungle_Pyramid);
    PyModule_AddIntMacro(mod, Swamp_Hut);
    PyModule_AddIntMacro(mod, Igloo);
    PyModule_AddIntMacro(mod, Village);
    PyModule_AddIntMacro(mod, Ocean_Ruin);
    PyModule_AddIntMacro(mod, Shipwreck);
    PyModule_AddIntMacro(mod, Monument);
    PyModule_AddIntMacro(mod, Mansion);
    PyModule_AddIntMacro(mod, Outpost);
    PyModule_AddIntMacro(mod, Ruined_Portal);
    PyModule_AddIntMacro(mod, Ruined_Portal_N);
    PyModule_AddIntMacro(mod, Ancient_City);
    PyModule_AddIntMacro(mod, Treasure);
    PyModule_AddIntMacro(mod, Mineshaft);
    PyModule_AddIntMacro(mod, Desert_Well);
    PyModule_AddIntMacro(mod, Geode);
    PyModule_AddIntMacro(mod, Fortress);
    PyModule_AddIntMacro(mod, Bastion);
    PyModule_AddIntMacro(mod, End_City);
    PyModule_AddIntMacro(mod, End_Gateway);
    PyModule_AddIntMacro(mod, End_Island);
    PyModule_AddIntMacro(mod, Trail_Ruins);
    PyModule_AddIntMacro(mod, Trial_Chambers);
    PyModule_AddIntMacro(mod, Abandoned_Camp);
    PyModule_AddIntMacro(mod, Nether_Fossil);
    PyModule_AddIntMacro(mod, Stronghold);

    return mod;
}
