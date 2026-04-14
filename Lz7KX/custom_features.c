// Tap-hold tuning and key overrides live in this file so Oryx layout exports
// (which overwrite keymap.c) can merge into main without touching this logic.

#include QMK_KEYBOARD_H

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Per-key tapping term tweaks for home-row mods.
    // Kept intentionally minimal: global TAPPING_TERM is tuned for general typing,
    // but some keys (like home-row Shift) benefit from a slightly larger window.
    switch (keycode) {
        case MT(MOD_LSFT, KC_A):
            return TAPPING_TERM + 5;
        default:
            return TAPPING_TERM;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    (void)record;

    // Enable "Hold On Other Key Press" only for the home-row Shift key.
    // This improves fast capitalisation (e.g. typing "Igor" quickly) by ensuring
    // that when another key is pressed during the tap/hold window, the Shift hold
    // action is chosen early enough to affect that keypress.
    //
    // We keep this scoped to MT(MOD_LSFT, KC_A) so thumb layer-taps (e.g.
    // LT(2, KC_SPACE)) do not become overly eager to activate layers during fast
    // rolling/overlap typing.
    return keycode == MT(MOD_LSFT, KC_A);
}

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t *key_overrides[] = {
    &delete_key_override,
    NULL,
};
