#include QMK_KEYBOARD_H
#include "keycodes.h"
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
            layer_off(get_highest_layer(layer_state));
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
        case LT(_EXT, KC_NO):
            return handle_lr(_EXT, record);
        case LT(_SYM, KC_NO):
            return handle_lr(_SYM, record);
        case LT(_NUM, KC_NO):
            return handle_lr(_NUM, record);
        case LT(_ACC, KC_NO):
            return handle_lr(_ACC, record);
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
    // TODO: set color
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
        case _ACC:
            rgb_matrix_sethsv_noeeprom(HSV_CORAL);
            break;
    }
  return state;
}

const uint32_t unicode_map[] PROGMEM = {
    // A
    [U_SAGRAV] = 0x00E0,
    [U_CAGRAV] = 0x00C0,
    [U_SACIRC] = 0x00E2,
    [U_CACIRC] = 0x00C2,
    [U_SATREM] = 0x00E4,
    [U_CATREM] = 0x00C4,

    // Ç
    [U_SCCEDI] = 0x00E7,
    [U_CCCEDI] = 0x00C7,

    // E
    [U_SEAIGU] = 0x00E9,
    [U_CEAIGU] = 0x00C9,
    [U_SEGRAV] = 0x00E8,
    [U_CEGRAV] = 0x00C8,
    [U_SECIRC] = 0x00EA,
    [U_CECIRC] = 0x00CA,
    [U_SETREM] = 0x00EB,
    [U_CETREM] = 0x00CB,

    // I
    [U_SITREM] = 0x00EF,
    [U_CITREM] = 0x00CF,

    // O
    [U_SOCIRC] = 0x00F4,
    [U_COCIRC] = 0x00D4,
    [U_SOTREM] = 0x00F6,
    [U_COTREM] = 0x00D6,

    // Œ
    [U_SODANS] = 0x0153,
    [U_CODANS] = 0x0152,

    // ẞ
    [U_SESZET] = 0x00DF,
    [U_CESZET] = 0x1E9E,

    // U
    [U_SUCIRC] = 0x00FB,
    [U_CUCIRC] = 0x00DB,
    [U_SUTREM] = 0x00FC,
    [U_CUTREM] = 0x00DC,

    // °
    [U_DEG] = 0x00B0,

    // €
    [U_EUR] = 0x20AC
};

const uint16_t PROGMEM number_layer_combo[] = {LT(_EXT, KC_NO), OSM(MOD_LSFT), COMBO_END};
const uint16_t PROGMEM accent_layer_combo[] = {LT(_SYM, KC_NO), KC_SPC, COMBO_END};
combo_t key_combos[] = {
    COMBO(number_layer_combo, LT(_NUM, KC_NO)),
    COMBO(accent_layer_combo, LT(_ACC, KC_NO)),
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
          KC_Q, KC_W, KC_F, KC_P, KC_B,    KC_J, KC_L, KC_U,    KC_Y,   KC_SCLN,
          KC_A, KC_R, KC_S, KC_T, KC_G,    KC_M, KC_N, KC_E,    KC_I,   KC_O,
          KC_Z, KC_X, KC_C, KC_D, KC_V,    KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH,
        LT(_EXT, KC_NO), OSM(MOD_LSFT),    KC_SPC, LT(_SYM, KC_NO)
    ),

    [_GAM] = LAYOUT(
        KC_Q,    KC_R, KC_X, KC_E, KC_T,    KC_Y, KC_U, KC_I, KC_O, KC_P,
        KC_LSFT, KC_A, KC_W, KC_D, KC_G,    KC_H, KC_K, KC_L, KC_P, KC_Q,
        KC_LCTL, KC_Z, KC_S, KC_F, KC_C,    KC_N, KC_H, KC_M, KC_V, KC_DOT,
                KC_SPC, LT(_EXT, KC_NO),    KC_SPC, LT(_SYM, KC_NO)
    ),

    [_EXT] = LAYOUT(
        KC_ESC,  KC_TAB,  KC_ENT,  KC_BSPC,  KC_MPRV,    QK_BOOT,  KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
        BKC_ESC, BKC_TAB, BKC_ENT, BKC_BSP,  KC_MPLY,    TO(_GAM), BKC_SFT, BKC_CTL, BKC_ALT, BKC_GUI,
        KC_NO,   KC_NO,   KC_NO,   KC_DEL,   KC_MNXT,    KC_NO,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
                                    TO(_BASE), KC_NO,    KC_NO, KC_NO
    ),

    [_SYM] = LAYOUT(
        KC_CIRC, KC_EXLM, KC_QUES, KC_AT,   KC_PERC,    KC_AMPR, KC_TILD, KC_PIPE, KC_GRV,  X(U_DEG),
        KC_LABK, KC_SCLN, KC_LCBR, KC_LPRN, KC_LBRC,    KC_HASH, KC_MINS, KC_SLSH, KC_QUOT, KC_DLR,
        KC_RABK, KC_COLN, KC_RCBR, KC_RPRN, KC_RBRC,    KC_ASTR, KC_UNDS, KC_BSLS, KC_DQUO, X(U_EUR),
                                      KC_EQL, KC_NO,    KC_NO, TO(_BASE)
    ),

    [_NUM] = LAYOUT(
        KC_SLSH, KC_7, KC_8, KC_9, KC_PLUS,    KC_NO, KC_F12, KC_F11, KC_F10, KC_F9,
        KC_0,    KC_1, KC_2, KC_3, KC_MINS,    KC_NO, KC_F4,  KC_F3,  KC_F2,  KC_F1,
        KC_ASTR, KC_4, KC_5, KC_6, KC_EQL,     KC_NO, KC_F8,  KC_F7,  KC_F6,  KC_F5,
                      TO(_BASE), TO(_BASE),    KC_NO, KC_NO
    ),

    [_ACC] = LAYOUT(
        XP(U_SOCIRC, U_COCIRC), XP(U_SUCIRC, U_CUCIRC), XP(U_SACIRC, U_CACIRC), XP(U_SECIRC, U_CECIRC), XP(U_SITREM, U_CITREM),    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        XP(U_SOTREM, U_COTREM), XP(U_SUTREM, U_CUTREM), XP(U_SATREM, U_CATREM), XP(U_SEAIGU, U_CEAIGU), XP(U_SETREM, U_CETREM),    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        XP(U_SCCEDI, U_CCCEDI), XP(U_SESZET, U_CESZET), XP(U_SAGRAV, U_CAGRAV), XP(U_SEGRAV, U_CEGRAV), XP(U_SODANS, U_CODANS),    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,    TO(_BASE), TO(_BASE)
    ),
};
