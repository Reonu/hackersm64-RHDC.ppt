.channelC_table:
// office sfx
sound_ref .sound_office_sfx_mgs

.layer_office_sfx:
layer_note1 39, 168, 64
layer_end

# sounds
.sound_office_sfx_mgs:
chan_setbank 0xD
chan_setinstr 0
chan_setval 20
chan_call .set_reverb
chan_setlayer 0, .layer_office_sfx
chan_end
