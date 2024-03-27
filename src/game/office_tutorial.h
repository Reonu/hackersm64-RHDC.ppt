#ifndef MPAC_TUTORIAL_H
#define MPAC_TUTORIAL_H

#include "types.h"

struct TutorialMessage {
    char *title;
    char *body;
};

extern s32 gDidFirstClick;

void run_tut(void);

#endif
