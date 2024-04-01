#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/math_util.h"
#include "confroom.h"

enum ConversationState {
    CONVO_INACTIVE,
    CONVO_TALKING,
    CONVO_QTE,
};

enum QTEStatus {
    QTE_PENDING,
    QTE_FAIL,
    QTE_SUCCESS,
};

enum QTEButtonPrompt {
    QTE_PROMPT_A,
    QTE_PROMPT_B,
    QTE_PROMPT_Z,
    QTE_PROMPT_NUM,
};

typedef struct {
    Vec3f speakerPos;
    s32 coolDownTimer;
    s32 timer;
    s32 qteTriggerTime;
    struct Object *conversationalist;
    u8 points;
    u8 pointsReq;
    u8 state;
    u8 qteStatus;
    u8 prompt;
} Conversation;

#define seconds(s) ((s32)(s * 30))

#define CONVO_TALK_LENGTH  seconds(7)
#define CONVO_QTE_LENGTH   seconds(1.2f)
#define CONVO_QTE_COOLDOWN seconds(4.7f)

#define CONVO_QTE_X_MIN  (16)
#define CONVO_QTE_X_GOAL (90)
#define CONVO_QTE_X_MAX  (SCREEN_WIDTH - 32 - 16)
#define CONVO_QTE_Y  (SCREEN_HEIGHT - 64)

// in px how far before or after the icons match up
#define CONVO_QTE_GOAL_LEEWAY 16

extern Conversation gCurConvo;

s32 start_convo(f32 *speakerPos);
void render_convo(Gfx **head);
void update_convo(void);
