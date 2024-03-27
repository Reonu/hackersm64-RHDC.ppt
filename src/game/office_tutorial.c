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

#define MSG_LEN (10*30)
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
        .body = "First of all, use the Control Stick to.\n"
                "to move, and the C-Buttons to look around\n",
    },
};


s32 gDidFirstClick = 0;

void run_tut(struct MarioState *m) {
    static s32 curMsg = -1;
    static s32 msgTimer = 0;
    static s32 alpha = 0;
    static s32 init = FALSE;

    if (gTutorialFinished) 
        return;

    /*if (!init && gIsConsole) {
        init = TRUE;
        if (gPlayer2Controller->isMouse) sTutorialMessages[0].body = sFirstMessageForN64MouseGigachads;
        else sTutorialMessages[0].body = sFirstMessageForConsoleChads;
    };*/

    msgTimer = MSG_LEN+MSG_FADE;
    curMsg = 0;
    aggress(curMsg < ARRAY_COUNT(sTutorialMessages), "Reonu forgor the message");

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