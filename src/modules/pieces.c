#include <Python.h>
#include "../external/cubiomes/features/stronghold.h"
#include "../external/cubiomes/features/fortress.h"
#include "../external/cubiomes/features/mineshaft.h"
#include "../external/cubiomes/features/end_city.h"

PyMODINIT_FUNC PyInit_pieces(PyModuleDef *pybiomes) {
    PyObject *mod = PyModule_Create(pybiomes);

    // Stronghold pieces
    PyModule_AddIntMacro(mod, SH_STRAIGHT);
    PyModule_AddIntMacro(mod, SH_PRISON_HALL);
    PyModule_AddIntMacro(mod, SH_LEFT_TURN);
    PyModule_AddIntMacro(mod, SH_RIGHT_TURN);
    PyModule_AddIntMacro(mod, SH_ROOM_CROSSING);
    PyModule_AddIntMacro(mod, SH_STRAIGHT_STAIRS_DOWN);
    PyModule_AddIntMacro(mod, SH_STAIRS_DOWN);
    PyModule_AddIntMacro(mod, SH_FIVE_CROSSING);
    PyModule_AddIntMacro(mod, SH_CHEST_CORRIDOR);
    PyModule_AddIntMacro(mod, SH_LIBRARY);
    PyModule_AddIntMacro(mod, SH_PORTAL_ROOM);
    PyModule_AddIntMacro(mod, SH_FILLER_CORRIDOR);
    PyModule_AddIntMacro(mod, SH_PIECE_COUNT);

    // Fortress pieces
    PyModule_AddIntMacro(mod, FORTRESS_START);
    PyModule_AddIntMacro(mod, BRIDGE_STRAIGHT);
    PyModule_AddIntMacro(mod, BRIDGE_CROSSING);
    PyModule_AddIntMacro(mod, BRIDGE_FORTIFIED_CROSSING);
    PyModule_AddIntMacro(mod, BRIDGE_STAIRS);
    PyModule_AddIntMacro(mod, BRIDGE_SPAWNER);
    PyModule_AddIntMacro(mod, BRIDGE_CORRIDOR_ENTRANCE);
    PyModule_AddIntMacro(mod, CORRIDOR_STRAIGHT);
    PyModule_AddIntMacro(mod, CORRIDOR_CROSSING);
    PyModule_AddIntMacro(mod, CORRIDOR_TURN_RIGHT);
    PyModule_AddIntMacro(mod, CORRIDOR_TURN_LEFT);
    PyModule_AddIntMacro(mod, CORRIDOR_STAIRS);
    PyModule_AddIntMacro(mod, CORRIDOR_T_CROSSING);
    PyModule_AddIntMacro(mod, CORRIDOR_NETHER_WART);
    PyModule_AddIntMacro(mod, FORTRESS_END);
    PyModule_AddIntMacro(mod, FORTRESS_PIECE_COUNT);

    // Mineshaft pieces
    PyModule_AddIntMacro(mod, MS_CORRIDOR);
    PyModule_AddIntMacro(mod, MS_CROSSING);
    PyModule_AddIntMacro(mod, MS_ROOM);
    PyModule_AddIntMacro(mod, MS_STAIRS);
    PyModule_AddIntMacro(mod, MS_PIECE_COUNT);

    // End City pieces
    PyModule_AddIntMacro(mod, BASE_FLOOR);
    PyModule_AddIntMacro(mod, BASE_ROOF);
    PyModule_AddIntMacro(mod, BRIDGE_END);
    PyModule_AddIntMacro(mod, BRIDGE_GENTLE_STAIRS);
    PyModule_AddIntMacro(mod, BRIDGE_PIECE);
    PyModule_AddIntMacro(mod, BRIDGE_STEEP_STAIRS);
    PyModule_AddIntMacro(mod, FAT_TOWER_BASE);
    PyModule_AddIntMacro(mod, FAT_TOWER_MIDDLE);
    PyModule_AddIntMacro(mod, FAT_TOWER_TOP);
    PyModule_AddIntMacro(mod, SECOND_FLOOR_1);
    PyModule_AddIntMacro(mod, SECOND_FLOOR_2);
    PyModule_AddIntMacro(mod, SECOND_ROOF);
    PyModule_AddIntMacro(mod, END_SHIP);
    PyModule_AddIntMacro(mod, THIRD_FLOOR_1);
    PyModule_AddIntMacro(mod, THIRD_FLOOR_2);
    PyModule_AddIntMacro(mod, THIRD_ROOF);
    PyModule_AddIntMacro(mod, TOWER_BASE);
    PyModule_AddIntMacro(mod, TOWER_PIECE);
    PyModule_AddIntMacro(mod, TOWER_TOP);
    PyModule_AddIntMacro(mod, END_CITY_PIECE_COUNT);

    return mod;
}
