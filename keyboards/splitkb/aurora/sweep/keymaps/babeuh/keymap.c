/* Copyright 2022 splitkb.com <support@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "os_detection.h"

enum layers {
    _DEFAULT = 0,
    LR_EXT,
    LR_SYM,
    LR_NUM,
    LR_ACC
};

enum my_keycodes {
    GB_ESC = SAFE_RANGE,
    GB_TAB,
    GB_ENT,
    GB_BSP,
    GB_GUI,
    GBS_SFT,
    GBS_CTL,
    GBS_ALT,
    GBS_GUI
};

enum unicode_names {
    UNI_EURO,
    UNI_DEGR,
    ACS_OCIRC,
    ACC_OCIRC,
    ACS_OTREM,
    ACC_OTREM,
    ACS_UCIRC,
    ACC_UCIRC,
    ACS_UTREM,
    ACC_UTREM,
    ACS_ESZ,
    ACC_ESZ,
    ACS_ACIRC,
    ACC_ACIRC,
    ACS_ATREM,
    ACC_ATREM,
    ACS_AGRAV,
    ACC_AGRAV,
    ACS_ECIRC,
    ACC_ECIRC,
    ACS_EAIGU,
    ACC_EAIGU,
    ACS_EGRAV,
    ACC_EGRAV,
    ACS_ICIRC,
    ACC_ICIRC,
    ACS_ETREM,
    ACC_ETREM,
    ACS_ITREM,
    ACC_ITREM
};
const uint32_t unicode_map[] PROGMEM = {
    [UNI_EURO] = 0x20AC,  // €
    [UNI_DEGR] = 0x00B0,  // °
    [ACS_OCIRC] = 0x00F4,  // ô
    [ACC_OCIRC] = 0x00D4,  // Ô
    [ACS_OTREM] = 0x00F6,  // ö
    [ACC_OTREM] = 0x00D6,  // Ö
    [ACS_UCIRC] = 0x00FB,  // û
    [ACC_UCIRC] = 0x00DB,  // Û
    [ACS_UTREM] = 0x00FC,  // ü
    [ACC_UTREM] = 0x00DC,  // Ü
    [ACS_ESZ]   = 0x00DF,  // ß
    [ACC_ESZ]   = 0x1E9E,  // ẞ
    [ACS_ACIRC] = 0x00E2,  // â
    [ACC_ACIRC] = 0x00C2,  // Â
    [ACS_ATREM] = 0x00E4,  // ä
    [ACC_ATREM] = 0x00C4,  // Ä
    [ACS_AGRAV] = 0x00E0,  // à
    [ACC_AGRAV] = 0x00C0,  // À
    [ACS_ECIRC] = 0x00EA,  // ê
    [ACC_ECIRC] = 0x00CA,  // Ê
    [ACS_EAIGU] = 0x00E9,  // é
    [ACC_EAIGU] = 0x00C9,  // É
    [ACS_EGRAV] = 0x00E8,  // è
    [ACC_EGRAV] = 0x00C8,  // È
    [ACS_ICIRC] = 0x00EE,  // î
    [ACC_ICIRC] = 0x00CE,  // Î
    [ACS_ETREM] = 0x00EB,  // ë
    [ACC_ETREM] = 0x00CB,  // Ë
    [ACS_ITREM] = 0x00EF,  // ï
    [ACC_ITREM] = 0x00CF,  // Ï
};

const uint16_t PROGMEM go_lr_num[] = {LT(LR_EXT, KC_NO), OSM(MOD_LSFT), COMBO_END};
const uint16_t PROGMEM go_lr_acc[] = {LT(LR_SYM, KC_NO), KC_SPC, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(go_lr_num, LT(LR_NUM, KC_NO)),
    COMBO(go_lr_acc, LT(LR_ACC, KC_NO)),
};

// Layer DOC
// DF(layer): switch default
// TG(Layer): activate
//   - Use when moving up
// TO(layer): activate and deactivate other except default
//   - Use when moving down
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEFAULT] = LAYOUT(
        KC_Q, KC_W, KC_F, KC_P,     KC_B,    KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
        KC_A, KC_R, KC_S, KC_T,     KC_G,    KC_M, KC_N, KC_E, KC_I, KC_O,
        KC_Z, KC_X, KC_C, KC_D,     KC_V,    KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH,
	LT(LR_EXT, KC_NO), OSM(MOD_LSFT),    KC_SPC, LT(LR_SYM, KC_NO)
    ),

    // Navigation and Media
    [LR_EXT] = LAYOUT(
        KC_ESC, KC_TAB, KC_ENT, KC_BSPC, KC_MNXT,     KC_NO, KC_LSFT, KC_LCTL, KC_LALT, GB_GUI,
        GB_ESC, GB_TAB, GB_ENT, GB_BSP,  KC_MPLY,     KC_NO, GBS_SFT, GBS_CTL, GBS_ALT, GBS_GUI,
	KC_NO,  KC_NO,  KC_NO,  KC_DEL,  KC_MNXT,     KC_NO, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
	                          KC_TRNS, KC_NO,     KC_NO, TO(_DEFAULT)
    ),
    // Symbols
    [LR_SYM] = LAYOUT(
        KC_CIRC, KC_EXLM, KC_QUES, KC_AT,   KC_PERC,    KC_AMPR, KC_TILD, KC_PIPE, KC_GRV,  X(UNI_DEGR),
        KC_LABK, KC_SCLN, KC_LCBR, KC_LPRN, KC_LBRC,    KC_HASH, KC_MINS, KC_SLSH, KC_DQUO, KC_DLR,
	KC_RABK, KC_COLN, KC_RCBR, KC_RPRN, KC_RBRC,    KC_ASTR, KC_UNDS, KC_BSLS, KC_QUOT, X(UNI_EURO),
	                        TO(_DEFAULT), KC_NO,    KC_NO, KC_TRNS
    ),
    // Numbers (and F-keys)
    [LR_NUM] = LAYOUT(
        KC_SLSH, KC_7, KC_8, KC_9, KC_PLUS,    OSM(MOD_LCTL), KC_F9, KC_F8, KC_F7, KC_NO,
        KC_0,    KC_1, KC_2, KC_3, KC_MINS,    OSM(MOD_LALT), KC_F3, KC_F2, KC_F1, KC_NO,
	KC_ASTR, KC_4, KC_5, KC_6, KC_EQL,     OSM(MOD_LGUI), KC_F6, KC_F5, KC_F4, KC_NO,
	                  KC_TRNS, KC_NO,    KC_NO, TO(_DEFAULT)
    ),
    // Accents
    [LR_ACC] = LAYOUT(
        XP(ACS_OCIRC, ACC_OCIRC), XP(ACS_UCIRC, ACC_UCIRC), XP(ACS_ACIRC, ACC_ACIRC), XP(ACS_ECIRC, ACC_ECIRC), XP(ACS_ITREM, ACC_ITREM),    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        XP(ACS_OTREM, ACC_OTREM), XP(ACS_UTREM, ACC_UTREM), XP(ACS_ATREM, ACC_ATREM), XP(ACS_EAIGU, ACC_EAIGU), XP(ACS_ETREM, ACC_ETREM),    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	KC_NO,                    XP(ACS_ESZ, ACC_ESZ),     XP(ACS_AGRAV, ACC_AGRAV), XP(ACS_EGRAV, ACC_EGRAV), XP(ACS_ITREM, ACC_ITREM),     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
	               TO(_DEFAULT), KC_NO,    KC_TRNS, KC_TRNS
    ),
};

uint8_t current_layer = _DEFAULT;
layer_state_t layer_state_set_user(layer_state_t state) {
  current_layer = get_highest_layer(state);
  switch (current_layer) {
    case _DEFAULT:
      rgb_matrix_sethsv_noeeprom(HSV_AZURE);
      break;
    case LR_EXT:
      rgb_matrix_sethsv_noeeprom(HSV_GOLD);
      break;
    case LR_SYM:
      rgb_matrix_sethsv_noeeprom(HSV_TEAL);
      break;
    case LR_NUM:
      rgb_matrix_sethsv_noeeprom(HSV_CHARTREUSE);
      break;
    case LR_ACC:
      rgb_matrix_sethsv_noeeprom(HSV_CORAL);
      break;
  }
  return state;
}

void gb_handler(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    register_code(keycode);
  } else {
    unregister_code(keycode);
    layer_off(current_layer);
  }
}

void gbs_handler(uint16_t keycode) {
  set_oneshot_mods(MOD_BIT(keycode));
  layer_off(current_layer);
}

bool lt_handler(uint8_t layer, keyrecord_t *record) {
  if (record->tap.count && record->event.pressed) {
    layer_on(layer);
    return false;
  } else if (current_layer == layer && record->event.pressed) {
    return false;
  }
  return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(LR_EXT, KC_NO):
      return lt_handler(LR_EXT, record);
    case LT(LR_SYM, KC_NO):
      return lt_handler(LR_SYM, record);
    case LT(LR_NUM, KC_NO):
      return lt_handler(LR_NUM, record);
    case LT(LR_ACC, KC_NO):
      return lt_handler(LR_ACC, record);
    case GB_ESC:
      gb_handler(KC_ESC, record);
      return false;
    case GB_TAB:
      gb_handler(KC_TAB, record);
      return false;
    case GB_ENT:
      gb_handler(KC_ENT, record);
      return false;
    case GB_BSP:
      gb_handler(KC_BSPC, record);
      return false;
    case GB_GUI:
      gb_handler(KC_LGUI, record);
      return false;
    case GBS_SFT:
      gbs_handler(KC_LSFT);
      return false;
    case GBS_CTL:
      gbs_handler(KC_LCTL);
      return false;
    case GBS_ALT:
      gbs_handler(KC_LALT);
      return false;
    case GBS_GUI:
      gbs_handler(KC_LGUI);
      return false;
  }
  return true;
}

os_variant_t host_os;
uint32_t startup_exec(uint32_t trigger_time, void *cb_arg) {
  if (is_keyboard_master()) {
    host_os = detected_host_os();
    if (host_os) {
      if (host_os == OS_LINUX) {
	set_unicode_input_mode(UNICODE_MODE_LINUX);
      } else if (host_os == OS_WINDOWS) {
	set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
      } else if (host_os == OS_MACOS || host_os == OS_IOS) {
        set_unicode_input_mode(UNICODE_MODE_MACOS);
      };
    };
  }
  return host_os ? 0 : 500;
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable_noeeprom();
  rgb_matrix_sethsv_noeeprom(HSV_AZURE);
  defer_exec(100, startup_exec, NULL);
}

void keyboard_pre_init_user(void) {
  // Set our LED pin as output
  setPinOutput(24);
  // Turn the LED off
  // (Due to techincal reasons, high is off and low is on)
  writePinHigh(24);
};
