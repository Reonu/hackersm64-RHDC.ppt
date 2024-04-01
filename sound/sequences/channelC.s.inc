.channelC_table:
// office sfx
sound_ref .sound_office_sfx_mgs
sound_ref .sound_office_sfx_done_making_coffee
sound_ref .sound_office_sfx_ope_youre_back
sound_ref .sound_office_sfx_turn_the_lights_off
sound_ref .sound_office_sfx_walking_in
sound_ref .sound_office_sfx_you_need_coffee_now
sound_ref .sound_office_sfx_coffeemachine
sound_ref .sound_office_sfx_end_of_presentation

.layer_office_sfx:
layer_note1 39, 168, 64
layer_end
.layer_office_sfx_done_making_coffee:
layer_note1 39, 708, 64
layer_end
.layer_office_sfx_ope_youre_back:
layer_note1 39, 276, 64
layer_end
.layer_office_sfx_turn_the_lights_off:
layer_note1 39, 371, 64
layer_end
.layer_office_sfx_walking_in:
layer_note1 39, 1092, 64
layer_end
.layer_office_sfx_you_need_coffee_now:
layer_note1 39, 311, 64
layer_end
.layer_office_sfx_coffeemachine:
layer_note1 39, 192, 64
layer_end
.layer_office_sfx_end_of_presentation:
layer_note1 39, 1773, 64
layer_end

# sounds
.sound_office_sfx_mgs:
chan_setbank 0xD
chan_setinstr 0
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx
chan_end

.sound_office_sfx_done_making_coffee:
chan_setbank 0xD
chan_setinstr 1
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_done_making_coffee
chan_end

.sound_office_sfx_ope_youre_back:
chan_setbank 0xD
chan_setinstr 2
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_ope_youre_back
chan_end

.sound_office_sfx_turn_the_lights_off:
chan_setbank 0xD
chan_setinstr 3
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_turn_the_lights_off
chan_end

.sound_office_sfx_walking_in:
chan_setbank 0xD
chan_setinstr 4
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_walking_in
chan_end

.sound_office_sfx_you_need_coffee_now:
chan_setbank 0xD
chan_setinstr 5
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_you_need_coffee_now
chan_end

.sound_office_sfx_coffeemachine:
chan_setbank 0xD
chan_setinstr 6
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_coffeemachine
chan_end

.sound_office_sfx_end_of_presentation:
chan_setbank 0xD
chan_setinstr 7
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx_end_of_presentation
chan_end
