# Oryx with custom QMK

This repository allows combining the convenience of [Oryx’s](https://www.zsa.io/oryx) graphical layout editing with the power of [QMK](https://qmk.fm), allowing you to customize your Oryx layout with advanced QMK features like Achordion and Repeat Key, while automating firmware builds through GitHub Actions.

For a detailed guide, check out the full [blog post here](https://blog.zsa.io/oryx-custom-qmk-features).

## How it works

Each time you run the GitHub Action, the workflow will:
1. Fetch the latest changes made in Oryx.
2. Merge them with any QMK features you've added in the source code.
3. Build the firmware, incorporating modifications from both Oryx and your custom source code.

## How to use

1. Fork this repository (be sure to **uncheck the "Copy the main branch only" option**).
2. To initialize the repository with your layout:
   - Go to the **Actions** tab.
   - Select **Fetch and build layout**.
   - Click **Run workflow**.
   - Input your layout ID and keyboard type (your layout must be public in Oryx), then run the workflow.
   - (To avoid having to input values each time, you can modify the default values at the top of the `.github/workflows/fetch-and-build-layout.yml` file).
3. A folder containing your layout will be generated at the root of the repository.
4. You can now add your custom QMK features to this folder:
   - Edit `config.h`, `keymap.c` and `rules.mk` according to the [QMK documentation](https://github.com/qmk/qmk_firmware/tree/master/docs/features).
   - Commit and push to the **main** branch.
5. You can continue editing your layout through Oryx:
   - Make your changes in Oryx. 
   - Optionally, add a description of your changes in the **Some notes about what you changed** field; if provided, this will be used as commit message.
   - Confirm changes by clicking the **Compile this layout** button.
6. To build the firmware (including both Oryx and code modifications), rerun the GitHub Action. The firmware will be available for download in the action’s artifacts.
7. Flash your downloaded firmware using [Keymapp](https://www.zsa.io/flash#flash-keymap).
8. Enjoy!

## Oryx Chrome extension

To make building even easier, [@nivekmai](https://github.com/nivekmai) created an [Oryx Chrome extension](https://chromewebstore.google.com/detail/oryx-extension/bocjciklgnhkejkdfilcikhjfbmbcjal) to be able to trigger the GitHub Actions from inside Oryx itself.

## Custom QMK Configurations

This section documents all custom QMK features and modifications applied to this keymap beyond what's configured in Oryx.

### Key Overrides

- **Shift + Backspace → Delete**: Holding Shift and pressing Backspace sends Delete instead of regular backspace.
  - Configured in `keymap.c` using `ko_make_basic()`.
  - **Fix applied**: Changed `key_overrides` declaration from `const key_override_t **key_overrides` to `const key_override_t *key_overrides[]` to fix compilation errors with `ARRAY_SIZE()` macro.

### Combos

**Disabled.** Combos are off in `rules.mk` (`COMBO_ENABLE = no`) to match the layout in Oryx (no chorded keys). Brackets live on layer 1 via normal keys. If you enable combos again in Oryx or in `rules.mk`, you must add **`#define COMBO_COUNT`** in `config.h` and **`key_combos[]`** (plus combo arrays) in **`keymap.c`**, or QMK’s `keymap_introspection` will fail to build—those symbols must stay in `keymap.c`, not a separate file.

### Enabled Features

The following QMK features are enabled in `rules.mk`:
- `COMBO_ENABLE = no` — Combos disabled (aligns with Oryx)
- `KEY_OVERRIDE_ENABLE = yes` - Enables key override functionality
- `ORYX_ENABLE = yes` - Enables Oryx integration
- `RGB_MATRIX_CUSTOM_KB = yes` - Enables custom RGB matrix handling

## Addendum: Tapping term (global and per-key)

`#define TAPPING_TERM_PER_KEY` in `Lz7KX/config.h` enables `get_tapping_term()` in `Lz7KX/keymap.c`. The **home row `A` key is plain `KC_A`** (no Shift mod-tap); Oryx’s export still includes a `get_tapping_term` switch so merges stay aligned—tune other mod-taps in that function as needed.

### Where to edit

| What | File | Notes |
|------|------|--------|
| **Global** tapping term (milliseconds) | `Lz7KX/config.h` | `#define TAPPING_TERM …` — also set in Oryx; the **Fetch and build layout** workflow overwrites this from the latest export when merging. |
| **Per-key** (mod-taps, layer-taps) | `Lz7KX/keymap.c` | `get_tapping_term()` — add `case` branches for the **full** keycodes (e.g. `MT(MOD_LSFT, KC_A)`, `LT(1, KC_F)`). If you put Shift back on `A` in Oryx, switch the `case` to match that keycode or Git will conflict on the next merge. |

### If you re-enable Shift on `A` in Oryx

1. Use `MT(MOD_LSFT, KC_A)` in the keymap and change `get_tapping_term` to use `case MT(MOD_LSFT, KC_A):` (not `KC_A` alone—tap-hold decisions use the mod-tap keycode).
2. Optionally re-add `#define HOLD_ON_OTHER_KEY_PRESS` and `#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY` in `config.h` and implement `get_hold_on_other_key_press()` in `keymap.c` if you want per-key “hold on other key press” behaviour.

### Direction (global)

- **Increase `TAPPING_TERM`** — Holds need a longer press; fewer accidental holds on mod-taps / layer-taps.
- **Decrease** — Holds register sooner; snappier tap-hold typing but more accidental holds.



