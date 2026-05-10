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

## Addendum: Tapping term (global vs home-row Shift on `A`)

Tap-hold timing is controlled in two places. `#define TAPPING_TERM_PER_KEY` in `Lz7KX/config.h` enables per-key overrides; the actual per-key values are returned from `get_tapping_term()` in `Lz7KX/keymap.c`.

### Where to edit

| What | File | Notes |
|------|------|--------|
| **Global** tapping term (milliseconds) | `Lz7KX/config.h` | `#define TAPPING_TERM …` — also set in Oryx; a successful **Fetch and build layout** workflow overwrites this from the latest export unless you resolve merges manually. |
| **Shift-on-`A` only** (`MT(MOD_LSFT, KC_A)`) | `Lz7KX/keymap.c` | Inside `get_tapping_term()`, the `case MT(MOD_LSFT, KC_A):` branch returns `TAPPING_TERM + <offset>`. Adjust **only** that offset (or the expression) to tune this key without changing thumbs and other tap-holds. |
| **Hold on other key press** (optional) | `Lz7KX/keymap.c` | `get_hold_on_other_key_press()` — when this returns `true` for a mod-tap, another key pressed during the undecided window can favour the **hold** path earlier (helpful for fast capitals, often worse for quick `a` + roll). |

### Which direction to change things

- **Global `TAPPING_TERM` (increase)** — You must hold **longer** before any tap-hold counts as a hold. **Reduces** accidental holds on **all** mod-taps and layer-taps; taps need slightly snappier releases. **Decrease** does the opposite (holds register sooner).

- **`TAPPING_TERM + offset` for `MT(MOD_LSFT, KC_A)` (increase the offset, e.g. `+15` → `+35`)** — Only the **Shift-on-`A`** key waits longer before committing to Shift. Use this when **`a` is dropped or turns into Shift** on fast rolls, without slowing layer thumbs or other home-row mods. **Decrease the offset** if that key feels sluggish when you **intend** Shift.

- **`get_hold_on_other_key_press`** — Returning **`false`** (as in this repo) avoids biasing toward Shift when the next key arrives during the window; return **`true`** for specific keycodes only if you want that “capitalise on roll” behaviour and accept the trade-off for lowercase `a`.

Keep offsets **positive** for the `A` Shift mod-tap unless you know what you are doing: subtracting from `TAPPING_TERM` for that key shortens its tap window and makes false Shift **more** likely.



