#pragma once

enum custom_layers {
  _BASE,
  _GAM,
  _EXT,
  _SYM,
  _NUM,
  _ACC,
};

enum custom_keys {
    // Back Keycodes (go to previous layer)
    BKC_ESC = QK_KB_0,
    BKC_TAB,
    BKC_ENT,
    BKC_BSP,
    BKC_SFT,
    BKC_CTL,
    BKC_ALT,
    BKC_GUI
};

enum unicode_names {
    // A
    U_SAGRAV,
    U_CAGRAV,
    U_SACIRC,
    U_CACIRC,
    U_SATREM,
    U_CATREM,

    // Ç
    U_SCCEDI,
    U_CCCEDI,

    // E
    U_SEAIGU,
    U_CEAIGU,
    U_SEGRAV,
    U_CEGRAV,
    U_SECIRC,
    U_CECIRC,
    U_SETREM,
    U_CETREM,

    // I
    U_SITREM,
    U_CITREM,

    // O
    U_SOCIRC,
    U_COCIRC,
    U_SOTREM,
    U_COTREM,

    // ẞ
    U_SESZET,
    U_CESZET,

    // U
    U_SUCIRC,
    U_CUCIRC,
    U_SUTREM,
    U_CUTREM,

    // °
    U_DEG,

    // €
    U_EUR
};
