#include <ultra64.h>
#include <PR/os_internal_reg.h>
#include <PR/R4300.h>

#include "game/save_file.h"

#include "reboot.h"

u8 rebootStack[256];
N64Game gCurrentGame = GAME_SM64;
void *keepTheGameFromCrashing = __osPiRawReadIo; // Why???
void *keepTheGameFromCrashing2 = osPiGetStatus; // Why???

// 0x200 bytes
ALIGNED8 struct SaveBuffer gSaveBuffer;

u8 isSoftReset = FALSE;

#define WAIT_ON_IOBUSY(stat)                                \
    stat = IO_READ(PI_STATUS_REG);                          \
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) \
        stat = IO_READ(PI_STATUS_REG);

/**
 * Disables interrupts
 */
static __inline__ void reboot_disable_interrupts()
{
    register u32 flags;
    __asm__ __volatile__("mfc0 %0, $%1" : "=r" (flags) : "i" (C0_SR));
    flags &= ~SR_IE;
    __asm__ __volatile__("mtc0 %0, $%1" : : "r" (flags), "i" (C0_SR));
}

static __inline__ s32 reboot_read_word(u32 devAddr, u32 *data)
{
    register u32 stat;
    WAIT_ON_IOBUSY(stat);
    *data = IO_READ((u32)osRomBase | devAddr);
    return 0;
}

static __inline__ s32 reboot_dma_read(u32 devAddr, void *dramAddr, u32 size)
{
    register u32 stat;
    WAIT_ON_IOBUSY(stat);
    IO_WRITE(PI_DRAM_ADDR_REG, K0_TO_PHYS(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS((u32)osRomBase | devAddr));
    IO_WRITE(PI_WR_LEN_REG, size - 1);
    return 0;
}

static __inline__ u32 reboot_get_pi_status()
{
    return IO_READ(PI_STATUS_REG);
}

u8 rebootStack[256];

void reboot_game_internal(u32 devAddr)
{
    u32 entryPoint;
    OSThread *curThread;
    // Disable all interrupts
    reboot_disable_interrupts();

    isSoftReset = TRUE;

    curThread = *gActiveQueueLocations[gCurrentGame];
    // Destroy all other threads
    while (curThread && (curThread != curThread->tlnext))
    {
        OSThread *next = curThread->tlnext;
        if (curThread != *gRunningThreadLocations[gCurrentGame])
        {
            gDestroyThreadLocations[gCurrentGame](curThread);
        }
        curThread = next;
    }
    // Get the entry point of the pointed to rom header
    reboot_read_word(devAddr + 8, &entryPoint);

    // Imitate initial startup DMA
    reboot_dma_read(devAddr + 0x1000, (void*)entryPoint, 0x100000);

    // Wait for DMA to finish
    while (reboot_get_pi_status() & (PI_STATUS_DMA_BUSY | PI_STATUS_ERROR));

    osResetType = 2;

    // Jump to the game's entry point
    __asm__ __volatile__("jr %0" : : "r" (entryPoint));
}
