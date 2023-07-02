#include QMK_KEYBOARD_H
#include "keymap.h"
#include "os_detection.h"


void handle_bkc(uint16_t keycode, keyrecord_t *record, bool osm) {
    if (record->event.pressed) {
        if (!osm) {
            tap_code16(keycode);
        } else {
            set_oneshot_mods(MOD_BIT(keycode));
        }
        uint8_t current_layer = get_highest_layer(layer_state);
        if (current_layer == _BASE) return;
        layer_off(current_layer);
    }
}

bool handle_lr(uint8_t layer, keyrecord_t *record) {
    if (record->tap.count && record->event.pressed) {
        uint8_t current_layer = get_highest_layer(layer_state);
        if (current_layer == layer) {
            layer_off(layer);
        } else {
            layer_on(layer);
        }
        return false;
    }
    return true;
}

bool altrep_change_is_forced_equal = false;
uint16_t final_keycode = KC_NO;
// CHANGE BETWEEN KEYS
static void process_altrep_change(uint16_t keycode, uint8_t mods) {
    bool actioned = false;
    switch (keycode) {
        // Quotes
        case KC_QUOT: actioned=true; final_keycode=KC_DQUO; break;
        case KC_DQUO: actioned=true; final_keycode=KC_GRV;  break;
        case KC_GRV:  actioned=true; final_keycode=KC_QUOT; break;
        // Bars
        case KC_BSLS: actioned=true; final_keycode=KC_PIPE; break;
        case KC_PIPE: actioned=true; final_keycode=KC_BSLS; break;
        // Dashes
        case KC_MINS: actioned=true; final_keycode=KC_UNDS; break;
        case KC_UNDS: actioned=true; final_keycode=KC_TILD; break;
        case KC_TILD: actioned=true; final_keycode=KC_MINS; break;
        // #*&
        case KC_HASH: actioned=true; final_keycode=KC_ASTR; break;
        case KC_ASTR: actioned=true; final_keycode=KC_AMPR; break;
        case KC_AMPR: actioned=true; final_keycode=KC_HASH; break;
        // ([
        case KC_LPRN: actioned=true; final_keycode=KC_LBRC; break;
        case KC_LBRC: actioned=true; final_keycode=KC_LPRN; break;
        // {<
        case KC_LCBR: actioned=true; final_keycode=KC_LABK; break;
        case KC_LABK: actioned=true; final_keycode=KC_LCBR; break;
        // !?
        case KC_EXLM: actioned=true; final_keycode=KC_QUES; break;
        case KC_QUES: actioned=true; final_keycode=KC_EXLM; break;

        // Function keys
        case KC_0:    actioned=true; final_keycode=KC_F10;  break;
        case KC_1:    actioned=true; final_keycode=KC_F1;   break;
        case KC_2:    actioned=true; final_keycode=KC_F2;   break;
        case KC_3:    actioned=true; final_keycode=KC_F3;   break;
        case KC_4:    actioned=true; final_keycode=KC_F4;   break;
        case KC_5:    actioned=true; final_keycode=KC_F5;   break;
        case KC_6:    actioned=true; final_keycode=KC_F6;   break;
        case KC_7:    actioned=true; final_keycode=KC_F7;   break;
        case KC_8:    actioned=true; final_keycode=KC_F8;   break;
        case KC_9:    actioned=true; final_keycode=KC_F9;   break;
    }

    if (!actioned || altrep_change_is_forced_equal) {
        if(final_keycode != KC_NO) final_keycode=KC_NO;
        tap_code16(KC_EQL);
    } else if (actioned) {
        set_last_keycode(final_keycode);
    }
}
// SWITCH DIRECTION
static void process_altrep_switch(uint16_t keycode, uint8_t mods) {
    bool actioned = false;
    switch (keycode) {
        case KC_LPRN: actioned=true; final_keycode=KC_RPRN; break;
        case KC_RPRN: actioned=true; final_keycode=KC_LPRN; break;

        case KC_LBRC: actioned=true; final_keycode=KC_RBRC; break;
        case KC_RBRC: actioned=true; final_keycode=KC_LBRC; break;

        case KC_LCBR: actioned=true; final_keycode=KC_RCBR; break;
        case KC_RCBR: actioned=true; final_keycode=KC_LCBR; break;

        case KC_LABK: actioned=true; final_keycode=KC_RABK; break;
        case KC_RABK: actioned=true; final_keycode=KC_LABK; break;
    }

    if (actioned) {
        set_last_keycode(final_keycode);
    }
}

bool altrep_triggered = false;
void trigger_altrep_result(uint16_t keycode) {
    if (keycode != ALTREP_C && keycode != ALTREP_S && final_keycode != KC_NO) {
        uint16_t keycode_to_press = final_keycode;
        final_keycode = KC_NO;
        altrep_triggered = true;
        tap_code16(keycode_to_press);
        set_last_keycode(KC_NO);
    }
}

uint16_t accent_deadkey_pressed = KC_NO;
void trigger_accent_deadkey_result(uint16_t keycode) {
    bool is_shifted = (get_mods() & MOD_MASK_SHIFT || get_oneshot_mods() & MOD_MASK_SHIFT);
    clear_oneshot_mods();
    switch (accent_deadkey_pressed) {
        case AC_GRAV:
            if (keycode == KC_A) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    tap_code16(KC_Z);
                    register_unicode(0x00C0);
                } else {
                    register_unicode(0x00E0);
                }
            } else if (keycode == KC_E) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C8);
                } else {
                    register_unicode(0x00E8);
                }
            }
            break;
        case AC_AIGU:
            if (keycode == KC_E) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C9);
                } else {
                    register_unicode(0x00E9);
                }
            }
            break;
        case AC_CIRC:
            if (keycode == KC_A) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C2);
                } else {
                    register_unicode(0x00E2);
                }
            } else if (keycode == KC_E) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00CA);
                } else {
                    register_unicode(0x00EA);
                }
            } else if (keycode == KC_I) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00CE);
                } else {
                    register_unicode(0x00EE);
                }
            } else if (keycode == KC_O) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00D4);
                } else {
                    register_unicode(0x00F4);
                }
            } else if (keycode == KC_U) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00DB);
                } else {
                    register_unicode(0x00FB);
                }
            }
            break;
        case AC_TREM:
            if (keycode == KC_A) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C4);
                } else {
                    register_unicode(0x00E4);
                }
            } else if (keycode == KC_E) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00CB);
                } else {
                    register_unicode(0x00EB);
                }
            } else if (keycode == KC_I) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00CF);
                } else {
                    register_unicode(0x00EF);
                }
            } else if (keycode == KC_O) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00D6);
                } else {
                    register_unicode(0x00F6);
                }
            } else if (keycode == KC_U) {
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00DC);
                } else {
                    register_unicode(0x00FC);
                }
            }
            break;
        case AC_SPEC:
            if (keycode == KC_A) { //ae ligature
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C6);
                } else {
                    register_unicode(0x00E6);
                }
            } else if (keycode == KC_C) { // Ç
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x00C7);
                } else {
                    register_unicode(0x00E7);
                }
            } else if (keycode == KC_O) { //oe ligature
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x0152);
                } else {
                    register_unicode(0x0153);
                }
            } else if (keycode == KC_S) { // ẞ
                accent_deadkey_pressed = KC_NO;
                if (is_shifted) {
                    register_unicode(0x1E9E);
                } else {
                    register_unicode(0x00DF);
                }
            } else if (keycode == KC_E) { // euro
                accent_deadkey_pressed = KC_NO;
                register_unicode(0x20AC);
            } else if (keycode == KC_D) { // degree
                accent_deadkey_pressed = KC_NO;
                register_unicode(0x00B0);
            }
            break;
    }
    if (accent_deadkey_pressed != KC_NO) {
        accent_deadkey_pressed = KC_NO;
        tap_code16(keycode);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!altrep_triggered) {
        trigger_altrep_result(keycode);
    } else {
        altrep_triggered = false;
        altrep_change_is_forced_equal = true;
    }
    if (altrep_change_is_forced_equal && keycode != ALTREP_C) altrep_change_is_forced_equal = false;
    switch (keycode) {
        case BKC_ESC:
            handle_bkc(KC_ESC, record, false);
            return false;
        case BKC_TAB:
            handle_bkc(KC_TAB, record, false);
            return false;
        case BKC_ENT:
            handle_bkc(KC_ENT, record, false);
            return false;
        case BKC_BSP:
            handle_bkc(KC_BSPC, record, false);
            return false;
        case BKC_SFT:
            handle_bkc(KC_LSFT, record, true);
            return false;
        case BKC_CTL:
            handle_bkc(KC_LCTL, record, true);
            return false;
        case BKC_ALT:
            handle_bkc(KC_LALT, record, true);
            return false;
        case BKC_GUI:
            handle_bkc(KC_LGUI, record, true);
            return false;
        case TO(_BASE):
            if (record->event.pressed) {
                layer_off(get_highest_layer(layer_state));
            }
            return false;
        case TO(_GAM):
            if (record->event.pressed) {
                uint8_t current_layer = get_highest_layer(layer_state);
                layer_off(current_layer);
                if (layer_state_is(_GAM)) {
                    layer_off(_GAM);
                } else {
                    layer_on(_GAM);
                }
            }
            return false;
        case LT(_GAM, KC_NO):
            return handle_lr(_GAM, record);
        case ALTREP_C:
            if (record->event.pressed) {
                process_altrep_change(get_last_keycode(), get_last_mods());
            }
            return false;
        case ALTREP_S:
            if (record->event.pressed) {
                process_altrep_switch(get_last_keycode(), get_last_mods());
            }
            return false;
        case KC_QUOT:
        case KC_BSLS:
        case KC_MINS:
        case KC_HASH:
        case KC_LPRN:
        case KC_LCBR:
        case KC_EXLM:
        case KC_0:
        case KC_1:
        case KC_2:
        case KC_3:
        case KC_4:
        case KC_5:
        case KC_6:
        case KC_7:
        case KC_8:
        case KC_9:
            if (record->event.pressed) return false;
            if (altrep_triggered) break;
            final_keycode = keycode;
            return false;
        case AC_GRAV:
        case AC_AIGU:
        case AC_TREM:
        case AC_CIRC:
        case AC_SPEC:
            if (record->event.pressed) accent_deadkey_pressed = keycode;
            return false;
    }

    if (accent_deadkey_pressed != KC_NO && IS_BASIC_KEYCODE(keycode) && record->event.pressed) {
        trigger_accent_deadkey_result(keycode);
        return false;
    }

    return true;
}

uint32_t startup_exec(uint32_t trigger_time, void *cb_arg) {
    os_variant_t os_type = detected_host_os();
    if (os_type) {
        switch (os_type) {
            case OS_MACOS:
                if (get_unicode_input_mode() != UNICODE_MODE_MACOS);
                set_unicode_input_mode(UNICODE_MODE_MACOS);
                break;
            case OS_WINDOWS:
                if (get_unicode_input_mode() != UNICODE_MODE_WINCOMPOSE);
                set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
                break;
            case OS_LINUX:
            default:
                if (get_unicode_input_mode() != UNICODE_MODE_LINUX);
                set_unicode_input_mode(UNICODE_MODE_LINUX);
                break;
        }
    }
    return os_type ? 0 : 500;
}

void keyboard_pre_init_user(void) {
    // Set our LED pin as output
    setPinOutput(24);
    // Turn the LED off
    writePinHigh(24);
}

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_WHITE);
    if (is_keyboard_master()) defer_exec(100, startup_exec, NULL);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _BASE:
            rgb_matrix_sethsv_noeeprom(HSV_WHITE);
            break;
        case _GAM:
            rgb_matrix_sethsv_noeeprom(HSV_RED);
            break;
        case _EXT:
            rgb_matrix_sethsv_noeeprom(HSV_GOLD);
            break;
        case _SYM:
            rgb_matrix_sethsv_noeeprom(HSV_TEAL);
            break;
    }
  return state;
}


bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    switch (keycode) {
        case ALTREP_C:
        case ALTREP_S:
            return false;
    }

    return true;
}
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
          KC_Q, KC_W, KC_F, KC_P, KC_B,    KC_J, KC_L, KC_U,    KC_Y,   KC_SCLN,
          KC_A, KC_R, KC_S, KC_T, KC_G,    KC_M, KC_N, KC_E,    KC_I,   KC_O,
          KC_Z, KC_X, KC_C, KC_D, KC_V,    KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH,
               MO(_EXT), OSM(MOD_LSFT),    KC_SPC, MO(_SYM)
    ),

    [_GAM] = LAYOUT(
        KC_Q,    KC_R, KC_X, KC_E, KC_T,    KC_Y, KC_U, KC_I, KC_O, KC_P,
        KC_LSFT, KC_A, KC_W, KC_D, KC_G,    KC_H, KC_K, KC_L, KC_P, KC_Q,
        KC_LCTL, KC_Z, KC_S, KC_F, KC_C,    KC_N, KC_H, KC_M, KC_V, KC_DOT,
                       KC_SPC, MO(_EXT),    KC_SPC, MO(_SYM)
    ),

    [_EXT] = LAYOUT(
        KC_ESC,  KC_TAB,  KC_ENT,  KC_BSPC,  KC_MPRV,    QK_BOOT,  KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
        BKC_ESC, BKC_TAB, BKC_ENT, BKC_BSP,  KC_MPLY,    TO(_GAM), BKC_SFT, BKC_CTL, BKC_ALT, BKC_GUI,
        KC_NO,   KC_NO,   KC_NO,   KC_DEL,   KC_MNXT,    KC_NO,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
                                    TO(_BASE), KC_NO,    KC_NO, KC_NO
    ),

    [_SYM] = LAYOUT(
        KC_F12, KC_7, KC_8, KC_9, KC_PLUS,    KC_AT,   KC_LPRN, KC_LCBR, KC_COLN, KC_EXLM,
        KC_0,   KC_1, KC_2, KC_3, KC_PERC,    KC_HASH, KC_MINS, KC_BSLS, KC_QUOT, KC_DLR,
        KC_F9,  KC_4, KC_5, KC_6, KC_NO,      AC_GRAV, AC_AIGU, AC_TREM, AC_CIRC, AC_SPEC,
                       ALTREP_C, ALTREP_S,    QK_REP, TO(_BASE)
    ),
};
