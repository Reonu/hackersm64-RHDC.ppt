#include "reboot.h"

destroyThread_t *gDestroyThreadLocations[NUM_GAMES] = {
    osDestroyThread,   // GAME_SM64
    // pmosDestroyThread, // GAME_PM
    // ootosDestroyThread // GAME_OOT
};

OSThread **gActiveQueueLocations[NUM_GAMES] = {
    &__osActiveQueue,   // GAME_SM64
    // &pm__osActiveQueue, // GAME_PM
    // &oot__osActiveQueue // GAME_OOT
};

OSThread **gRunningThreadLocations[NUM_GAMES] = {
    &__osRunningThread,   // GAME_SM64
    // &pm__osRunningThread, // GAME_PM
    // &oot__osRunningThread // GAME_OOT
};
