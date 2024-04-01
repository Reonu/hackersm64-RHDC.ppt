#include <ultra64.h>

#include "sm64.h"
#include "confroom.h"
#include "confroom_conversation.h"
#include "object_fields.h"
#include "object_list_processor.h"
#include "engine/math_util.h"
#include "behavior_data.h"
#include "game_init.h"
#include "actors/group0.h"
#include "cozy_print.h"
#include "segment2.h"
#include "seq_ids.h"
#include "game/fpv_player.h"
#include "audio/external.h"

static const Conversation sInitConvo = {
    .speakerPos = {0, 0, 0},
    .conversationalist = NULL,
    .timer = 0,
    .coolDownTimer = 0,
    .qteTriggerTime = 0,
    .points = 0,
    .pointsReq = 0,
    .qteStatus = QTE_PENDING,
    .state = CONVO_INACTIVE,
    .prompt = QTE_PROMPT_A,
};

Conversation gCurConvo = {
    .speakerPos = {0, 0, 0},
    .conversationalist = NULL,
    .timer = 0,
    .coolDownTimer = 0,
    .qteTriggerTime = 0,
    .points = 0,
    .pointsReq = 0,
    .qteStatus = QTE_PENDING,
    .state = CONVO_INACTIVE,
    .prompt = QTE_PROMPT_A,
};

static u16 prompt_to_input[QTE_PROMPT_NUM] = {
    [QTE_PROMPT_A] = A_BUTTON,
    [QTE_PROMPT_B] = B_BUTTON,
    [QTE_PROMPT_Z] = Z_TRIG,
};

static Texture *prompt_to_texture[QTE_PROMPT_NUM] = {
    [QTE_PROMPT_A] = convo_qte_a,
    [QTE_PROMPT_B] = convo_qte_b,
    [QTE_PROMPT_Z] = convo_qte_z,
};
#define QTE_PROMPT_BUTTON_ANY (A_BUTTON | B_BUTTON | Z_TRIG)

void init_convo_state(void) {
    gCurConvo = sInitConvo;
}

void full_reset_convo_state(void) {
    Conversation *convo = &gCurConvo;
    convo->timer = 0;
    convo->qteTriggerTime = 0;
    convo->points = 0;
    convo->pointsReq = 0;
    convo->state = CONVO_INACTIVE;
    convo->qteStatus = QTE_PENDING;
    convo->prompt = QTE_PROMPT_A;
    convo->conversationalist = NULL;
}

void reset_convo(Conversation *convo, s32 req) {
    convo->timer = 0;
    convo->qteTriggerTime = 0;
    convo->points = 0;
    convo->pointsReq = req;
    convo->state = CONVO_TALKING;
    convo->qteStatus = QTE_PENDING;
    convo->prompt = QTE_PROMPT_A;
}

s32 start_convo(f32 *speakerPos) {
    Conversation *convo = &gCurConvo;
    s32 req = get_num_req_qtes();
#ifdef SLIDE_DEBUG
    if (gFPVPlayer.godMode) {
        return FALSE;
    }
#endif
    if (req) {
        reset_convo(convo, get_num_req_qtes());
        vec3f_copy(convo->speakerPos, speakerPos);
        play_secondary_music(SEQ_DSI, 0x70, 0x40, 0);
        convo->conversationalist = o;
        return TRUE;
    }
    return FALSE;
}

s32 get_convo_state(void) {
    return gCurConvo.state;
}

static s32 get_prompt_pos_from_time(s32 t) {
    return (s32)(remap(t, 0, CONVO_QTE_LENGTH, CONVO_QTE_X_MAX, CONVO_QTE_X_MIN));
}

void update_convo(void) {
    Conversation *convo = &gCurConvo;

    if (convo->state == CONVO_INACTIVE) {
        convo->coolDownTimer--;
        if (convo->coolDownTimer < 0) convo->coolDownTimer = 0; 
        return;
    }
    play_secondary_music(SEQ_DSI, 0x70, 0x40, 0);

    convo->timer++;
    switch (convo->state) {
        case CONVO_TALKING:
            if (convo->timer > CONVO_TALK_LENGTH) {
                convo->state = CONVO_QTE;
                convo->timer = 0;
                convo->qteTriggerTime = 0;
                convo->qteStatus = QTE_PENDING;
                convo->prompt = random_u16() % QTE_PROMPT_NUM;
            }
            break;
        case CONVO_QTE:
            if (convo->timer > CONVO_QTE_LENGTH) {
                if (convo->qteStatus == QTE_SUCCESS) {
                    convo->points++;
                    play_sound(SOUND_GENERAL2_RIGHT_ANSWER, gGlobalSoundSource);
                    if (convo->points >= convo->pointsReq) {
                        full_reset_convo_state();
                        convo->coolDownTimer = CONVO_QTE_COOLDOWN;
                        func_80321080(1);
                        break;
                    } else {
                        convo->state = CONVO_QTE;
                        convo->timer = 0;
                        convo->qteTriggerTime = 0;
                        convo->qteStatus = QTE_PENDING;
                        convo->prompt = random_u16() % QTE_PROMPT_NUM;
                        // before when it was restarting the convo
                        // convo->state = CONVO_TALKING;
                    }
                } else {
                    reset_convo(convo, convo->pointsReq);
                }
            } else if (convo->qteStatus != QTE_PENDING) {
                // noop
            } else if (gPlayer1Controller->buttonPressed & QTE_PROMPT_BUTTON_ANY) {
                u16 goalInput = prompt_to_input[convo->prompt];
                s32 isCorrect = (gPlayer1Controller->buttonPressed & goalInput) == goalInput;
                convo->qteTriggerTime = convo->timer;
                s32 triggerGoalDif = CONVO_QTE_X_GOAL - get_prompt_pos_from_time(convo->qteTriggerTime);

                if (isCorrect && absi(triggerGoalDif) < CONVO_QTE_GOAL_LEEWAY) {
                    convo->qteStatus = QTE_SUCCESS;
                } else {
                    convo->qteStatus = QTE_FAIL;
                    play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundSource);
                }
            } else if (get_prompt_pos_from_time(convo->timer) < CONVO_QTE_X_GOAL - CONVO_QTE_GOAL_LEEWAY) {
                convo->qteStatus = QTE_FAIL;
            }
            break;
    }
}

void print_convo_req(Conversation *convo) {
    static char textBuf[32]; 
    sprintf(textBuf, "%d / %d", convo->points, convo->pointsReq);
    if (convo->qteStatus == QTE_FAIL) {
        print_set_envcolour(255, 20, 20, 255);
    } else {
        print_set_envcolour(255, 255, 255, 255);
    }
    print_small_text_buffered(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32, textBuf, PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
}

void render_convo(Gfx **head) {
    Conversation *convo = &gCurConvo;
    if (convo->state == CONVO_INACTIVE) return;
    if (convo->state == CONVO_TALKING) {
        print_convo_req(convo);
        return;
    }

    Gfx *gfx = *head;

    Texture *tex = prompt_to_texture[convo->prompt];

    if (convo->qteStatus == QTE_FAIL) {
        render_rgba16_tex_rect_all(&gfx, 32, 32, CONVO_QTE_X_GOAL, CONVO_QTE_Y, tex, 127, 127, 32, 32);
    } else {
        render_rgba16_tex_rect_all(&gfx, 32, 32, CONVO_QTE_X_GOAL, CONVO_QTE_Y, tex, 127, 127, 127, 127);
    }

    s32 x = convo->qteStatus == QTE_SUCCESS
        ? CONVO_QTE_X_GOAL
        : get_prompt_pos_from_time(convo->timer);

    if (convo->qteStatus == QTE_FAIL) {
        render_rgba16_tex_rect_all(&gfx, 32, 32, x, CONVO_QTE_Y, tex, 255, 255, 64, 64);
    } else if (convo->qteStatus == QTE_SUCCESS) {
        render_rgba16_tex_rect_all(&gfx, 32, 32, x, CONVO_QTE_Y, tex, 255, 255, 255, 255);
    } else {
        render_rgba16_tex_rect_all(&gfx, 32, 32, x, CONVO_QTE_Y, tex, 255, 200, 200, 200);
    }

    *head = gfx;
    print_convo_req(convo);
}
