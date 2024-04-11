#ifndef __REBOOT_H__
#define __REBOOT_H__

#define REBOOT_STACK_SIZE 256

#ifdef _LANGUAGE_C

#include <ultra64.h>

typedef enum {
    GAME_SM64,
    NUM_GAMES
} N64Game;

typedef s32 (piRawReadIo_t)(u32, u32*);
typedef s32 (epiRawReadIo_t)(OSPiHandle *, u32, u32*);
typedef s32 (piRawStartDma_t)(s32, u32, void *, u32);
typedef void (destroyThread_t)(OSThread *);

extern OSThread **gActiveQueueLocations[NUM_GAMES];
extern OSThread **gRunningThreadLocations[NUM_GAMES];
extern destroyThread_t *gDestroyThreadLocations[NUM_GAMES];

extern N64Game gCurrentGame;

// Per game location symbols

// GAME_SM64
extern OSThread *__osActiveQueue;
extern OSThread *__osRunningThread;
extern destroyThread_t osDestroyThread;
extern piRawReadIo_t __osPiRawReadIo;
extern piRawStartDma_t __osPiRawStartDma;

// // GAME_PM
// extern epiRawReadIo_t pmosEPiRawReadIo;
// extern piRawStartDma_t pmosPiRawStartDma;
// extern OSPiHandle *pmcarthandle;
// extern destroyThread_t pmosDestroyThread;
// extern OSThread *pm__osActiveQueue;
// extern OSThread *pm__osRunningThread;

// // GAME_OOT
// extern epiRawReadIo_t oot__osEPiRawReadIo;
// extern piRawStartDma_t oot__osPiRawStartDma;
// extern OSPiHandle *ootgCartHandle;
// extern destroyThread_t ootosDestroyThread;
// extern OSThread *oot__osActiveQueue;
// extern OSThread *oot__osRunningThread;

extern u8 isSoftReset;

void swap_to_game(N64Game newGame);
N64Game get_running_game(void);

#endif

#endif
