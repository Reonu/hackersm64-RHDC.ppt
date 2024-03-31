#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "puppyprint.h"
#include "office_tutorial.h"
#include "level_update.h"
#include "game_init.h"
#include "course_table.h"
#include "object_list_processor.h"
#include "debug.h"
#include "engine/math_util.h"
#include "game/fpv_player.h"

#define MSG_LEN (2*30)
#define MSG_FADE (1*30)

char sFirstMessageForConsoleChads[] = "Welcome! Move the analog stick to\n"
                                      "move the cursor around, and press A\n"
                                      "to make Mario move to that spot.\n"
                                      "B opens the right click menu.\n"
                                      "Holding Z makes the cursor move slowly.\n"
                                      "R resets the cursor to the center.";

char sFirstMessageForN64MouseGigachads[] = "N64 mouse detected! Welcome, you gigachad.\n"
                                           "Use the mouse to move the cursor around,\n"
                                           "and try left clicking somewhere.";

struct TutorialMessage sTutorialMessages[] = {
    { // 00
        .title = "Welcome to RHDC.ppt",
        .body = "Use control stick to move.\n"
                "Use the C-Buttons to look around.\n"
                "Press START to view the controls.\n"
                "Press Dpad down if you want to hide\n"
                "all future tutorial messages.",
    },
    { // 01
        .title = "",
        .body = "Press B to interact with the coffee machine\n"
                "and get your first cup of coffee",
    },
    { // 02
        .title = "",
        .body = "Now, head to the conference room.\n"
                "Try tapping A repeatedly to run.",
    },
    { // 03
        .title = "",
        .body = "Press B to watch the presentation, and\n"
                "R to sip from the coffee to regain energy.\n"
                "If you fall asleep, you'll get fired! You can\n"
                "always get up to get more coffee with L",

    },
    { // 04
        .title = "",
        .body = "Grab a coffee first.",
    },
    { // 05
        .title = "",
        .body = "You're low on energy!\n"
                "Press R to drink coffee. If you run out,\n"
                "head to the kitchen and grab more."
    },
    {
        .title = "",
        .body = "All tutorials hidden.\n"
                "Restart the game if you didn't mean to do this.",
    }
};


s32 gDidFirstClick = 0;

void run_tut(void) {
    static s32 curMsg = -1;
    static s32 msgTimer = 0;
    static s32 alpha = 0;
    static s32 init = FALSE;

    if (gTutorialFinished || (gFPVPlayer.currentTutorial < 0) || gOfficeState.paused == PAUSE_STATE_END) 
        return;

    if (gFPVPlayer.currentTutorial != curMsg) {
        msgTimer = MSG_LEN+MSG_FADE;
        curMsg = gFPVPlayer.currentTutorial;
        aggress(curMsg < ARRAY_COUNT(sTutorialMessages), "Reonu forgor the message");
    }

    if (gFPVPlayer.curTutorialDone)
        msgTimer = MAX(msgTimer-1, 0);

    //struct ContextMenuState *ctxMenuState = get_context_menu_state();

    if (curMsg > -1 && msgTimer) {
        if (msgTimer >= MSG_FADE) {
            alpha = approach_s32_symmetric(alpha, 255, 30);
        } else {
            alpha = remap(msgTimer, MSG_FADE, 0, 255, 0);
        }
        
        s32 tmpAlpha = alpha;
        //if (menuIsShowing) tmpAlpha /= 3;

        // drop shadow.. might destroy the rdp
        // print_set_envcolor(&gfx, 0, 0, 0, tmpAlpha / 2);
        // print_small_text_g(&gfx, SCREEN_WIDTH/2+1, 150-16+1, sTutorialMessages[curMsg].title, PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_PIXEL_BOLD);
        // print_set_envcolor(&gfx, 0, 0, 0, tmpAlpha / 2);
        // print_small_text_g(&gfx, SCREEN_WIDTH/2+1, 150+1,    sTutorialMessages[curMsg].body,  PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_PIXEL_BOLD);
    
        print_set_envcolour(255, 255, 255, tmpAlpha);
        print_small_text(SCREEN_WIDTH/2, 150-16, sTutorialMessages[curMsg].title, PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
        print_set_envcolour(255, 255, 255, tmpAlpha);
        print_small_text(SCREEN_WIDTH/2, 150,    sTutorialMessages[curMsg].body,  PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
        //*head = gfx;
    }
}
