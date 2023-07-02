#pragma once

enum custom_layers {
  _BASE,
  _GAM,
  _EXT,
  _SYM,
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
    BKC_GUI,
    // Custom ALTREPS
    ALTREP_C,
    ALTREP_S,
    // Accent dead keys
    AC_GRAV,
    AC_AIGU,
    AC_TREM,
    AC_CIRC,
    AC_SPEC
};

