#include <Python.h>

PyMODINIT_FUNC PyInit_versions(PyModuleDef *pybiomes) {
    PyObject *mod = PyModule_Create(pybiomes);

    PyModule_AddIntMacro(mod, MC_UNDEF);
    PyModule_AddIntMacro(mod, MC_B1_7);
    PyModule_AddIntMacro(mod, MC_B1_8);
    PyModule_AddIntMacro(mod, MC_1_0_0);  
    PyModule_AddIntMacro(mod, MC_1_0);
    PyModule_AddIntMacro(mod, MC_1_1_0);  
    PyModule_AddIntMacro(mod, MC_1_2_5);  
    PyModule_AddIntMacro(mod, MC_1_3_2);  
    PyModule_AddIntMacro(mod, MC_1_4_7);  
    PyModule_AddIntMacro(mod, MC_1_5_2);  
    PyModule_AddIntMacro(mod, MC_1_6_4);  
    PyModule_AddIntMacro(mod, MC_1_7_10); 
    PyModule_AddIntMacro(mod, MC_1_8_9 ); 
    PyModule_AddIntMacro(mod, MC_1_9_4 ); 
    PyModule_AddIntMacro(mod, MC_1_10_2);
    PyModule_AddIntMacro(mod, MC_1_11_2);
    PyModule_AddIntMacro(mod, MC_1_12_2);
    PyModule_AddIntMacro(mod, MC_1_13_2);
    PyModule_AddIntMacro(mod, MC_1_14_4);
    PyModule_AddIntMacro(mod, MC_1_15_2);
    PyModule_AddIntMacro(mod, MC_1_16_1);
    PyModule_AddIntMacro(mod, MC_1_16_5);
    PyModule_AddIntMacro(mod, MC_1_17_1);
    PyModule_AddIntMacro(mod, MC_1_18_2);
    PyModule_AddIntMacro(mod, MC_1_18);
    PyModule_AddIntMacro(mod, MC_1_19_2);
    PyModule_AddIntMacro(mod, MC_1_19_4);
    PyModule_AddIntMacro(mod, MC_1_20_6);
    PyModule_AddIntMacro(mod, MC_1_21_1);
    PyModule_AddIntMacro(mod, MC_1_21_3);
    PyModule_AddIntMacro(mod, MC_1_21_WD);
    PyModule_AddIntMacro(mod, MC_1_21_4);
    PyModule_AddIntMacro(mod, MC_1_21_5);
    PyModule_AddIntMacro(mod, MC_1_21_6);
    PyModule_AddIntMacro(mod, MC_1_21_9);
    PyModule_AddIntMacro(mod, MC_1_21_11);
    PyModule_AddIntMacro(mod, MC_1_21);
    PyModule_AddIntMacro(mod, MC_26_1);
    PyModule_AddIntMacro(mod, MC_26_2);
    PyModule_AddIntMacro(mod, MC_26_3);
    PyModule_AddIntMacro(mod, MC_NEWEST);

    return mod;
}