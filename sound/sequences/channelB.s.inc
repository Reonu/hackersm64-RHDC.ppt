
.channelB_table:
# lead ins
sound_ref .sound_matty_lead_in_bandwidth
sound_ref .sound_matty_lead_in_low_hangin_fruit
sound_ref .sound_matty_lead_in_run_past_you
sound_ref .sound_matty_lead_in_there_you_are
sound_ref .sound_matty_lead_in_wheelhouse
# mumbles
sound_ref .sound_matty_mumble_ducks_in_a_row
sound_ref .sound_matty_mumble_first_and_foremost
sound_ref .sound_matty_mumble_synergy
sound_ref .sound_matty_mumble_the_new_normal
# questions
sound_ref .sound_matty_question_circle_back
sound_ref .sound_matty_question_hit_me
sound_ref .sound_matty_question_hows_that_sound
sound_ref .sound_matty_question_trigger
# coffee steal
sound_ref .sound_matty_coffee_steal_got_my_back
sound_ref .sound_matty_coffee_steal_promotion
sound_ref .sound_matty_coffee_steal_i_love_you
# coffee sip
sound_ref .sound_matty_coffee_sip_mmm
sound_ref .sound_matty_coffee_sip_ooh
sound_ref .sound_matty_coffee_sip_smack


.layer_matty_lead_in:
layer_note1 39, 240, MATTY_VOLUME
layer_end

.layer_matty_mumble:
layer_note1 39, 432, MATTY_VOLUME
layer_end

.layer_matty_question:
layer_note1 39, 180, MATTY_VOLUME
layer_end

.layer_matty_coffee:
layer_note1 39, 328, MATTY_VOLUME
layer_end

# LEAD INS
.sound_matty_lead_in_bandwidth:
chan_setbank 0xC
chan_setinstr 0
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_lead_in
chan_end

.sound_matty_lead_in_low_hangin_fruit:
chan_setbank 0xC
chan_setinstr 1
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_lead_in
chan_end

.sound_matty_lead_in_run_past_you:
chan_setbank 0xC
chan_setinstr 2
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_lead_in
chan_end

.sound_matty_lead_in_there_you_are:
chan_setbank 0xC
chan_setinstr 3
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_lead_in
chan_end

.sound_matty_lead_in_wheelhouse:
chan_setbank 0xC
chan_setinstr 4
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_lead_in
chan_end

# MUMBLES
.sound_matty_mumble_ducks_in_a_row:
chan_setbank 0xC
chan_setinstr 5
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_mumble
chan_end

.sound_matty_mumble_first_and_foremost:
chan_setbank 0xC
chan_setinstr 6
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_mumble
chan_end

.sound_matty_mumble_synergy:
chan_setbank 0xC
chan_setinstr 7
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_mumble
chan_end

.sound_matty_mumble_the_new_normal:
chan_setbank 0xC
chan_setinstr 8
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_mumble
chan_end

# QUESTIONS
.sound_matty_question_circle_back:
chan_setbank 0xC
chan_setinstr 9
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_question
chan_end

.sound_matty_question_hit_me:
chan_setbank 0xC
chan_setinstr 10
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_question
chan_end

.sound_matty_question_hows_that_sound:
chan_setbank 0xC
chan_setinstr 11
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_question
chan_end

.sound_matty_question_trigger:
chan_setbank 0xC
chan_setinstr 12
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_question
chan_end

# coffee steal
.sound_matty_coffee_steal_got_my_back:
chan_setbank 0xC
chan_setinstr 13
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end

.sound_matty_coffee_steal_promotion:
chan_setbank 0xC
chan_setinstr 14
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end

.sound_matty_coffee_steal_i_love_you:
chan_setbank 0xC
chan_setinstr 15
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end

# coffee sip
.sound_matty_coffee_sip_mmm:
chan_setbank 0xC
chan_setinstr 16
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end

.sound_matty_coffee_sip_ooh:
chan_setbank 0xC
chan_setinstr 17
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end

.sound_matty_coffee_sip_smack:
chan_setbank 0xC
chan_setinstr 18
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_matty_coffee
chan_end
