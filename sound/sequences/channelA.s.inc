
.channelA_table:
# lead ins
sound_ref .sound_cathy_lead_in_get_a_load_of_this
sound_ref .sound_cathy_lead_in_happy_hour
sound_ref .sound_cathy_lead_in_kelly_was_like
sound_ref .sound_cathy_lead_in_mitchell
sound_ref .sound_cathy_lead_in_youre_never_gonna_believe
# mumbles
sound_ref .sound_cathy_mumble_and_ill_be_nice
sound_ref .sound_cathy_mumble_foo_foo_cachoo
sound_ref .sound_cathy_mumble_paddle_drunk
sound_ref .sound_cathy_mumble_ugh_right
# questions
sound_ref .sound_cathy_question_plan_stan
sound_ref .sound_cathy_question_wait_shh_hes_coming
sound_ref .sound_cathy_question_what_about_you
sound_ref .sound_cathy_question_what_say_you
# coffee steal
sound_ref .sound_cathy_coffee_steal_howd_you_know
sound_ref .sound_cathy_coffee_steal_ill_take_that
sound_ref .sound_cathy_coffee_steal_such_a_sweetheart
# coffee sip
sound_ref .sound_cathy_coffee_sip_ahhh
sound_ref .sound_cathy_coffee_sip_mmm
sound_ref .sound_cathy_coffee_sip_good_mornin
# intro
sound_ref .sound_cathy_intro_hay_wake_up
sound_ref .sound_cathy_intro_you_look_terrible


.layer_lead_in:
layer_note1 39, 240, 64
layer_end

.layer_mumble:
layer_note1 39, 432, 64
layer_end

.layer_question:
layer_note1 39, 180, 64
layer_end

.layer_intro1:
layer_note1 39, 261, 64
layer_end

.layer_intro2:
layer_note1 39, 533, 64
layer_end

# LEAD INS
.sound_cathy_lead_in_get_a_load_of_this:
chan_setbank 0xB
chan_setinstr 0
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_lead_in
chan_end

.sound_cathy_lead_in_happy_hour:
chan_setbank 0xB
chan_setinstr 1
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_lead_in
chan_end

.sound_cathy_lead_in_kelly_was_like:
chan_setbank 0xB
chan_setinstr 2
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_lead_in
chan_end

.sound_cathy_lead_in_mitchell:
chan_setbank 0xB
chan_setinstr 3
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_lead_in
chan_end

.sound_cathy_lead_in_youre_never_gonna_believe:
chan_setbank 0xB
chan_setinstr 4
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_lead_in
chan_end

# MUMBLES
.sound_cathy_mumble_and_ill_be_nice:
chan_setbank 0xB
chan_setinstr 5
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_mumble
chan_end

.sound_cathy_mumble_foo_foo_cachoo:
chan_setbank 0xB
chan_setinstr 6
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_mumble
chan_end

.sound_cathy_mumble_paddle_drunk:
chan_setbank 0xB
chan_setinstr 7
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_mumble
chan_end

.sound_cathy_mumble_ugh_right:
chan_setbank 0xB
chan_setinstr 8
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_mumble
chan_end

# QUESTIONS
.sound_cathy_question_plan_stan:
chan_setbank 0xB
chan_setinstr 9
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_question
chan_end

.sound_cathy_question_wait_shh_hes_coming:
chan_setbank 0xB
chan_setinstr 10
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_question
chan_end

.sound_cathy_question_what_about_you:
chan_setbank 0xB
chan_setinstr 11
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_question
chan_end

.sound_cathy_question_what_say_you:
chan_setbank 0xB
chan_setinstr 12
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_question
chan_end

.layer_cathy_coffee:
layer_note1 39, 328, 64
layer_end

# coffee steal
.sound_cathy_coffee_steal_howd_you_know:
chan_setbank 0xB
chan_setinstr 13
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

.sound_cathy_coffee_steal_ill_take_that:
chan_setbank 0xB
chan_setinstr 14
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

.sound_cathy_coffee_steal_such_a_sweetheart:
chan_setbank 0xB
chan_setinstr 15
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

# coffee sip
.sound_cathy_coffee_sip_ahhh:
chan_setbank 0xB
chan_setinstr 16
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

.sound_cathy_coffee_sip_mmm:
chan_setbank 0xB
chan_setinstr 17
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

.sound_cathy_coffee_sip_good_mornin:
chan_setbank 0xB
chan_setinstr 18
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_cathy_coffee
chan_end

# intro
.sound_cathy_intro_hay_wake_up:
chan_setbank 0xB
chan_setinstr 19
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_intro1
chan_end

.sound_cathy_intro_you_look_terrible:
chan_setbank 0xB
chan_setinstr 20
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_intro2
chan_end
