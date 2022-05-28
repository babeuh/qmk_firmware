#include QMK_KEYBOARD_H

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
  bool is_press_action;
  int state;
} td_tap_t;

// Tapdance enums
enum {
    TD_ESC_GRV,
    TD_CAPS_LL,
};


// Layer enums
enum custom_layers {
    LR_DFLT,
    LR_MISC,
    LR_FUNC,
    LR_LANG,
};

td_state_t cur_dance (qk_tap_dance_state_t *state);

void caps_finished (qk_tap_dance_state_t *state, void *user_data);
void caps_reset (qk_tap_dance_state_t *state, void *user_data);

// Layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Default layer
     [LR_DFLT] = LAYOUT_60_ansi(
        TD(TD_ESC_GRV),        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        OSL(LR_MISC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,                 KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
        KC_LCTL,        KC_LGUI, KC_LALT,                            KC_SPC,                            KC_RALT, KC_NO,   KC_NO,   KC_RCTL
    ),
    // Misc layer iguess
    [LR_MISC] = LAYOUT_60_ansi(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, OSL(LR_FUNC), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, OSL(LR_LANG), KC_NO, TO(LR_FUNC), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO,
        KC_NO,          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO,
        KC_NO, KC_NO, KC_NO,                            KC_NO,                            KC_NO, KC_NO, KC_NO, KC_NO
    ),
    // Func layer
    [LR_FUNC] = LAYOUT_60_ansi(
        RGB_TOG, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        RESET, KC_MPRV, KC_UP,   KC_MNXT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO,
        KC_LSFT,          KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO,          KC_NO,
        TO(LR_DFLT), KC_NO, KC_NO,                            KC_MPLY,                            KC_NO, KC_TRNS, KC_NO, KC_NO
    ),
    // Language layer
    [LR_LANG] = LAYOUT_60_ansi(
        KC_NO, KC_NO, KC_NO, UC(0x00EA), UC(0x00E8), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, UC(0x00E2), KC_NO, UC(0x00E9), KC_NO, KC_NO, KC_NO, UC(0x00F9), UC(0x00EE), UC(0x00F4), KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, UC(0x00E0), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO,
        KC_NO,          KC_NO, KC_NO, UC(0x00E7), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO,
        KC_NO, KC_NO, KC_NO,                            KC_NO,                            KC_NO, KC_NO, KC_NO, KC_NO
    )
};

// Cur dance
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Caps Tap Dance functions
static td_tap_t capstap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void caps_finished (qk_tap_dance_state_t *state, void *user_data) {
    capstap_state.state = cur_dance(state);
    switch (capstap_state.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_CAPS);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
        case TD_DOUBLE_SINGLE_TAP:
            set_oneshot_layer(LR_LANG, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
    }
}

void caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    capstap_state.state = TD_NONE;
}

// Tap Dance Functions
qk_tap_dance_action_t tap_dance_actions[] = {
    // 1 tap: escape, 2 taps: grave
    [TD_ESC_GRV] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRV),
    // 1 tap: caps, 2 taps: osl(LR_LANG)
    [TD_CAPS_LL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
};

// process_record_keymap
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {
        case KC_TRNS:
        case KC_NO:
            /* Cancel one-shot layer when a non-key gets pressed */
            if (record->event.pressed && is_oneshot_layer_active())
            clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            return true;
        case RESET:
            /* Don't allow reset from oneshot layer state */
            if (record->event.pressed && is_oneshot_layer_active()){
                clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                return false;
            }
            return true;
        default:
            return true;
    }
    return true;
}

// Keyboard init
void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(180, 0, 255); //white
};
