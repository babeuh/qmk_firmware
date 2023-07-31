#include QMK_KEYBOARD_H
#include "keymap.h"
#include "os_detection.h"


static void handle_bkc(uint16_t keycode, keyrecord_t *record, bool osm) {
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

uint16_t accent_deadkey_pressed = KC_NO;
static void trigger_accent_deadkey_result(uint16_t keycode) {
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
    switch (keycode) {
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
        case _NUM:
            rgb_matrix_sethsv_noeeprom(HSV_CHARTREUSE);
            break;

    }
  return state;
}

const uint16_t PROGMEM number_layer_combo[] = {MO(_EXT), MO(_SYM), COMBO_END};
combo_t key_combos[] = {
    COMBO(number_layer_combo, MO(_NUM)),
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
          KC_Q, KC_W, KC_F, KC_P, KC_B,    KC_J, KC_L, KC_U,    KC_Y,   KC_SCLN,
          KC_A, KC_R, KC_S, KC_T, KC_G,    KC_M, KC_N, KC_E,    KC_I,   KC_O,
          KC_Z, KC_X, KC_C, KC_D, KC_V,    KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH,
               MO(_EXT), OSM(MOD_LSFT),    KC_SPC, MO(_SYM)
    ),

    [_GAM] = LAYOUT(
        KC_Q,    KC_R, KC_X, KC_E, KC_T,    KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
        KC_LSFT, KC_A, KC_W, KC_D, KC_G,    KC_M, KC_N, KC_B, KC_I, KC_O,
        KC_LCTL, KC_Z, KC_S, KC_F, KC_C,    KC_K, KC_H, KC_P, KC_V, KC_W,
                       KC_SPC, MO(_EXT),    KC_SLSH, MO(_SYM)
    ),

    [_EXT] = LAYOUT(
        KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_MPRV,    QK_BOOT,  KC_LGUI, BKC_CTL, BKC_ALT, BKC_GUI,
        KC_ESC, KC_TAB, KC_ENT, KC_BSPC, KC_MPLY,    TO(_GAM), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        KC_NO,  KC_NO,  KC_NO,  KC_DEL,  KC_MNXT,    AC_SPEC,  AC_AIGU, AC_GRAV, AC_TREM, AC_CIRC,
                                    KC_NO, KC_NO,    KC_NO, KC_NO
    ),

    [_SYM] = LAYOUT(
        KC_CIRC, KC_EXLM, KC_QUES, KC_AT,   KC_PERC,    KC_ASTR, KC_UNDS, KC_PIPE, KC_DQUO, KC_NO,
        KC_LABK, KC_COLN, KC_LCBR, KC_LPRN, KC_LBRC,    KC_HASH, KC_MINS, KC_BSLS, KC_QUOT, KC_DLR,
        KC_RABK, KC_SCLN, KC_RCBR, KC_RPRN, KC_RBRC,    KC_AMPR, KC_TILD, KC_NO,   KC_GRV,  KC_NO,
                                      KC_EQL, KC_NO,    KC_NO, KC_NO
    ),

    [_NUM] = LAYOUT(
        KC_NO, KC_7, KC_8, KC_9, KC_PLUS,    KC_NO, KC_F9, KC_F10, KC_F11, KC_F12,
        KC_NO, KC_4, KC_5, KC_6, KC_PERC,    KC_NO, KC_F5, KC_F6,  KC_F7,  KC_F8,
        KC_0,  KC_1, KC_2, KC_3, KC_NO,      KC_NO, KC_F1, KC_F2,  KC_F3,  KC_F4,
                        TO(_BASE), KC_NO,    KC_NO, TO(_BASE)
    ),
};
