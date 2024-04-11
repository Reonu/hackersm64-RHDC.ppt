#include "reboot.h"

.section .text
.global reboot_game
.type reboot_game @function 
reboot_game:
    la $sp, rebootStack + REBOOT_STACK_SIZE
    j reboot_game_internal
