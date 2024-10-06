/*
//Macro / layout notes

//Eclipse:

CTRL SPACE (Auto complete, arrow keys to select)

Ableton:

Space (play/pause)
Enter (record / stop record at en of measure)
r (stop / return)
m (metronome)
Encoders-
left right / up down

*/
#include QMK_KEYBOARD_H
#include "print.h"
#include "keymap.h"

//Array based stuff:

const uint8_t mode_traversal_map[] = {
    [0] = APP,     // General -> App
    [1] = GENERAL,      // App -> General
};


const uint8_t sub_mode_next_map[] = {
    [GENERAL] = MACROS,     // General -> Macros
    [MACROS] = GENERAL,// Macros -> General
    [APP] = APP   // App -> App
};

//Since there's only on sub mode this is unnecessary right now
/*
const uint8_t sub_mode_previous_map[] = {
    [GENERAL] = GENERAL,     // General -> General (No General sub-modes yet)
    [WORK] = WORK_MACROS,      // Work -> Work Macros
    [WORK_MACROS] = WORK,// Work Macros -> Work
    [APP] = APP   // App -> App
};
*/

const uint8_t rgb_maps[][11] = {
    [GENERAL] = {
        GENERAL_HUE, GENERAL_HUE, GENERAL_HUE,
        GENERAL_HUE, GENERAL_HUE, GENERAL_HUE,
        GENERAL_HUE, GENERAL_HUE, GENERAL_HUE,
        GENERAL_HUE, GENERAL_HUE
    },
    [MACROS] = {
        GENERAL_HUE, WORK_MACROS_HUE, WORK_MACROS_HUE,
        GENERAL_HUE, WORK_MACROS_HUE, WORK_MACROS_HUE,
        GENERAL_HUE, WORK_MACROS_HUE, WORK_MACROS_HUE,
        GENERAL_HUE, WORK_MACROS_HUE
    },
    [APP] = {
        APP_HUE, APP_HUE, APP_HUE,
        APP_HUE, APP_HUE, APP_HUE,
        APP_HUE, APP_HUE, APP_HUE,
        APP_HUE, APP_HUE
    },
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [GENERAL] = LAYOUT(
        TD(SELLINE_PAR), TD(BUILD_REBUILD), TD(GENERAL_MENU),
        TD(REFRESH), TD(TAB_CONTROL), TD(SAVE_CLOSE),
        TD(COPY_PASTE), TD(UNDO_REDO), TD(SEARCH_KEY)
    ),
    [MACROS] = LAYOUT(
        SELECT_WORD, SELECT_LINE, TD(GENERAL_MENU),
        DIV, M_CTRL_Y, GIT_STATUS,
        M_CTRL_SHIFT_Y, NPM_RUN_BUILD, M_CTRL_F
    ),
    [APP] = LAYOUT(
        KC_NO, KC_NO, TD(APP_MENU),
        KC_Z, KC_UP, KC_X,
        KC_LEFT, KC_DOWN, KC_RIGHT
    ),
    [EMPTY] = LAYOUT(
        KC_NO, KC_NO, TD(EMPTY_MENU),
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
    [UTIL] = LAYOUT(
        TD(HISTORY), TD(ZOOM), _______,
        TD(PGUP_HOME), TD(BROWSER_KEY), TD(COMS_KEY),
        TD(PGDN_END), M_WIN_SHIFT_LEFT, TD(MSWITCH)
    ),
    [APP_UTIL] = LAYOUT(
        TD(HISTORY), TD(ZOOM), _______,
        TD(B_1), TD(N_2), TD(C_3),
        TD(V_4), TD(D_5), TD(P_6)
    ),
    [RGB_UTIL] = LAYOUT(
        RGB_TOG, KC_NO, _______,
        RGB_MOD, RGB_HUI, RGB_SPI,
        RGB_RMOD, RGB_HUD, RGB_SPD
    ),
    [MENU] = LAYOUT(
        TO(EMPTY), KC_NO, TG(MENU),
        TO(GENERAL), TO(APP), KC_NO,
        KC_NO, TO(MACROS), KC_NO
    ),
};


void keyboard_post_init_user(void) {
    //init RGB matrix
    rgb_matrix_enable_noeeprom();
    rgb_matrix_set_color_all(0,0,255);

    layer_clear();
    layer_on(GENERAL);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DOUBLE_CLICK:
            if (record->event.pressed) {
                SEND_STRING(M_DOUBLE_CLICK);
            }
            return false;
        case SELECT_WORD:
            if (record->event.pressed) {
                SEND_STRING(M_SELECT_WORD);
            }
            return false;
        case SELECT_LINE:
            if (record->event.pressed) {
                SEND_STRING(M_SELECT_LINE);
            }
            return false;
        case NPM_RUN_BUILD:
            if (record->event.pressed) {
                SEND_STRING(M_NPM_RUN_BUILD);
            }
            return false;
        case GIT_STATUS:
            if (record->event.pressed) {
                SEND_STRING(M_GIT_STATUS);
            }
            return false;
        case DIV:
            if (record->event.pressed) {
                SEND_STRING(M_DIV);
            }
            return false;
        default:
            if(is_selecting_line || is_selecting_par) {
                is_selecting_line = false;
                is_selecting_par = false;
            }
            return true;
    }
}

uint8_t get_current_mode_index(void) {
    if(active_base_layer <= GENERAL_CYCLE_END) {
        return 0;
    } else if(active_base_layer <= APP_CYCLE_END) {
        return 1;
    } else {
        return 2;
    }
}

void mode_layer_cycle(bool cw) {
    uint8_t next_mode_layer = 0;
    uint8_t current_mode_index = get_current_mode_index();
    if(cw) {
        next_mode_layer = mode_traversal_map[current_mode_index];
    } else {
        next_mode_layer = mode_traversal_map[(current_mode_index + NUM_MODE_LAYERS - 2) % NUM_MODE_LAYERS];
    }

    layer_off(active_base_layer);
    layer_on(next_mode_layer);
}

void sub_mode_layer_cycle(bool cw) {
    uint8_t base_layer = active_base_layer;
    uint8_t next_layer = sub_mode_next_map[base_layer];

    if(base_layer != next_layer && base_layer != EMPTY) {
        layer_off(base_layer);
        layer_on(next_layer);
    }
}

void selector_encoder(bool clockwise) {
    if(is_selecting_par) {
        if (clockwise) {
            tap_code16(M_SHIFT_DN);
        } else {
            tap_code16(M_SHIFT_UP);
        }
    } else if(is_selecting_line) {
        if (clockwise) {
            tap_code16(M_SHIFT_RIGHT);
        } else {
            tap_code16(M_SHIFT_LEFT);
        }
    } else {
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
}

void alt_tab_encoder(bool clockwise) {
    if(!is_alt_tab) {
        tap_code16(M_ALT_TAB);
        is_alt_tab = true;
    } else {
        if (clockwise) {
            tap_code(KC_RIGHT);
        } else {
            tap_code(KC_LEFT);
        }
    }
}

void browser_tab_encoder(bool clockwise) {
    if (clockwise) {
        tap_code16(M_CTRL_PGDN);
    } else {
        tap_code16(M_CTRL_PGUP);
    }
}

void process_left_encoder(uint8_t layer, bool clockwise) {
    switch(layer) {
        case GENERAL ... MACROS:
            selector_encoder(clockwise);
            break;
        case UTIL ... APP_UTIL:
            alt_tab_encoder(clockwise);
            break;
        case RGB_UTIL:
            if (clockwise) {
                rgb_matrix_increase_val();
            } else {
                rgb_matrix_decrease_val();
            }
            break;
        case MENU:
            mode_layer_cycle(clockwise);
            break;
        default:
            break;
    }
}

void process_right_encoder(uint8_t layer, bool clockwise) {
    switch(layer) {
        case GENERAL ... MACROS:
            if (clockwise) {
                tap_code(KC_RIGHT);
            } else {
                tap_code(KC_LEFT);
            }
            break;
        case UTIL ... APP_UTIL:
            browser_tab_encoder(clockwise);
            break;
        case RGB_UTIL:
            if (clockwise) {
                rgb_matrix_increase_sat();
            } else {
                rgb_matrix_decrease_sat();
            }
            break;
        case MENU:
            sub_mode_layer_cycle(clockwise);
            break;
        default:
            break;
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    // Rotary encoder mapping by layer
    if(index == _LEFT) {
        process_left_encoder(get_highest_layer(layer_state), clockwise);
    } else {
        process_right_encoder(get_highest_layer(layer_state), clockwise);
    }
    return false;
}

uint8_t get_active_base_layer(layer_state_t state) {
    uint8_t layer = BASE_LAYERS_START;
    while(layer < BASE_LAYERS_END + 1) {
        if(IS_LAYER_ON_STATE(state, layer)){
            return layer;
        }
        layer = layer + 1;
    }
    return 255;
}

uint8_t get_active_util_layer(layer_state_t state) {
    uint8_t layer = UTIL_LAYERS_START;
    while(layer < UTIL_LAYERS_END + 1) {
        if(IS_LAYER_ON_STATE(state, layer)){
            return layer;
        }
        layer = layer + 1;
    }
    return 255;
}

void close_alt_tab(void) {
    if(is_alt_tab) {
        tap_code(KC_ENTER);
        is_alt_tab = false;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    active_base_layer = get_active_base_layer(state);
    active_util_layer = get_active_util_layer(state);
    rgb_disabled = active_base_layer == EMPTY || active_base_layer == APP;

    uint8_t last_active_util_layer = get_active_util_layer(layer_state);

    if(last_active_util_layer == UTIL || last_active_util_layer == APP_UTIL) {
        close_alt_tab();
    }
    return state;
}

void render_rgb_led(uint8_t led) {
    if(active_util_layer < 255) {
        if(led == 2 || (active_util_layer == MENU && led % 3 == 2)) {
            HSV hsv = {UTIL_HUE, rgb_matrix_get_sat(), rgb_matrix_get_val()};
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
        } else if (!rgb_disabled) {
            HSV hsv = {rgb_maps[active_base_layer][led], rgb_matrix_get_sat(), rgb_matrix_get_val()};
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
        }
    } else if(!rgb_disabled) {
        HSV hsv = {rgb_maps[active_base_layer][led], rgb_matrix_get_sat(), rgb_matrix_get_val()};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
    }
}

//Change RGB color based on layers
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        render_rgb_led(i);
    }
    return false;
}

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else if (state->count == 3) return TD_TRIPLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t general_menu_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t app_menu_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t empty_menu_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t search_key_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t browser_key_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t coms_key_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t ide_key_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t utility_key_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t copy_paste_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t undo_redo_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t tab_control_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t refresh_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t selline_par_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t build_rebuild_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t save_close_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t pgup_home_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t pgdn_end_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t history_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t zoom_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t mswitch_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t b_1_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t n_2_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t c_3_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t v_4_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t d_5_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t p_6_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void general_menu_finished(tap_dance_state_t *state, void *user_data) {
    general_menu_tap_state.state = cur_dance(state);
    switch (general_menu_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(GM_SINGLE_TAP);
            break;
        case TD_SINGLE_HOLD:
            layer_on(GM_HOLD_LAYER);
            break;
        case TD_DOUBLE_TAP:
            if (layer_state_is(MENU)) {
                layer_off(MENU);
            } else {
                layer_on(MENU);
            }
            break;
        default:
            break;
    }
}

void app_menu_finished(tap_dance_state_t *state, void *user_data) {
    app_menu_tap_state.state = cur_dance(state);
    switch (app_menu_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(AM_SINGLE_TAP);
            break;
        case TD_SINGLE_HOLD:
            layer_on(AM_HOLD_LAYER);
            break;
        case TD_DOUBLE_TAP:
            if (layer_state_is(MENU)) {
                layer_off(MENU);
            } else {
                layer_on(MENU);
            }
            break;
        default:
            break;
    }
}

void empty_menu_finished(tap_dance_state_t *state, void *user_data) {
    empty_menu_tap_state.state = cur_dance(state);
    switch (empty_menu_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(EM_SINGLE_TAP);
            break;
        case TD_SINGLE_HOLD:
            layer_on(EM_HOLD_LAYER);
            break;
        case TD_DOUBLE_TAP:
            if (layer_state_is(MENU)) {
                layer_off(MENU);
            } else {
                layer_on(MENU);
            }
            break;
        default:
            break;
    }
}

void search_key_finished(tap_dance_state_t *state, void *user_data) {
    search_key_tap_state.state = cur_dance(state);
    switch (search_key_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_F);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(M_CTRL_SHIFT_F);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_H);
            break;
        default:
            break;
    }
}

void browser_key_finished(tap_dance_state_t *state, void *user_data) {
    browser_key_tap_state.state = cur_dance(state);
    switch (browser_key_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_WIN_1);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_WIN_2);
            break;
        default:
            break;
    }
}

void coms_key_finished(tap_dance_state_t *state, void *user_data) {
    coms_key_tap_state.state = cur_dance(state);
    switch (coms_key_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_WIN_3);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_WIN_4);
            break;
        default:
            break;
    }
}

void ide_key_finished(tap_dance_state_t *state, void *user_data) {
    ide_key_tap_state.state = cur_dance(state);
    switch (ide_key_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_WIN_5);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_WIN_6);
            break;
        case TD_TRIPLE_TAP:
            tap_code16(M_WIN_7);
            break;
        default:
            break;
    }
}

void utility_key_finished(tap_dance_state_t *state, void *user_data) {
    utility_key_tap_state.state = cur_dance(state);
    switch (utility_key_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_WIN_8);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_WIN_9);
            break;
        case TD_TRIPLE_TAP:
            tap_code16(M_WIN_0);
            break;
        default:
            break;
    }
}

void copy_paste_finished(tap_dance_state_t *state, void *user_data) {
    copy_paste_tap_state.state = cur_dance(state);
    switch (copy_paste_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_C);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(M_CTRL_X);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_V);
            break;
        default:
            break;
    }
}

void undo_redo_finished(tap_dance_state_t *state, void *user_data) {
    undo_redo_tap_state.state = cur_dance(state);
    switch (undo_redo_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_Z);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_Y);
            break;
        default:
            break;
    }
}

void tab_control_finished(tap_dance_state_t *state, void *user_data) {
    tab_control_tap_state.state = cur_dance(state);
    switch (tab_control_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_T);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(M_CTRL_W);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_SHIFT_T);
            break;
        default:
            break;
    }
}

void refresh_finished(tap_dance_state_t *state, void *user_data) {
    refresh_tap_state.state = cur_dance(state);
    switch (refresh_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_F5);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(M_CTRL_SHIFT_DEL);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_F12);
            break;
        default:
            break;
    }
}

void selline_par_finished(tap_dance_state_t *state, void *user_data) {
    selline_par_tap_state.state = cur_dance(state);
    switch (selline_par_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(M_SELECT_WORD);
            is_selecting_line = true;
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(M_SELECT_LINE);
            is_selecting_par = true;
            break;
        default:
            break;
    }
}

void build_rebuild_finished(tap_dance_state_t *state, void *user_data) {
    build_rebuild_tap_state.state = cur_dance(state);
    switch (build_rebuild_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_SHIFT_B);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(M_CTRL_F5);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_SHIFT_R);
            break;
        default:
            break;
    }
}

void save_close_finished(tap_dance_state_t *state, void *user_data) {
    save_close_tap_state.state = cur_dance(state);
    switch (save_close_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_S);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_ALT_F4);
            break;
        default:
            break;
    }
}

void pgup_home_finished(tap_dance_state_t *state, void *user_data) {
    pgup_home_tap_state.state = cur_dance(state);
    switch (pgup_home_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_PGUP);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_HOME);
            break;
        default:
            break;
    }
}

void pgdn_end_finished(tap_dance_state_t *state, void *user_data) {
    pgdn_end_tap_state.state = cur_dance(state);
    switch (pgdn_end_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_PGDN);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_END);
            break;
        default:
            break;
    }
}

void history_finished(tap_dance_state_t *state, void *user_data) {
    history_tap_state.state = cur_dance(state);
    switch (history_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_ALT_LEFT);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_ALT_RIGHT);
            break;
        default:
            break;
    }
}

void zoom_finished(tap_dance_state_t *state, void *user_data) {
    zoom_tap_state.state = cur_dance(state);
    switch (zoom_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_EQUAL);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_MINUS);
            break;
        default:
            break;
    }
}

void mswitch_finished(tap_dance_state_t *state, void *user_data) {
    mswitch_tap_state.state = cur_dance(state);
    switch (mswitch_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(M_CTRL_ALT_H);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(M_CTRL_ALT_W);
            break;
        default:
            break;
    }
}

void b_1_finished(tap_dance_state_t *state, void *user_data) {
    b_1_tap_state.state = cur_dance(state);
    switch (b_1_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_B);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_1);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_7);
            break;
        default:
            break;
    }
}

void n_2_finished(tap_dance_state_t *state, void *user_data) {
    n_2_tap_state.state = cur_dance(state);
    switch (n_2_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_N);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_2);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_8);
            break;
        default:
            break;
    }
}

void c_3_finished(tap_dance_state_t *state, void *user_data) {
    c_3_tap_state.state = cur_dance(state);
    switch (c_3_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_C);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_3);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_9);
            break;
        default:
            break;
    }
}

void v_4_finished(tap_dance_state_t *state, void *user_data) {
    v_4_tap_state.state = cur_dance(state);
    switch (v_4_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_V);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_4);
            break;
        default:
            break;
    }
}

void d_5_finished(tap_dance_state_t *state, void *user_data) {
    d_5_tap_state.state = cur_dance(state);
    switch (d_5_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_D);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_5);
            break;
        default:
            break;
    }
}

void p_6_finished(tap_dance_state_t *state, void *user_data) {
    p_6_tap_state.state = cur_dance(state);
    switch (p_6_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_P);
            break;
        case TD_SINGLE_HOLD:
            tap_code16(KC_6);
            break;
        default:
            break;
    }
}

void general_menu_reset(tap_dance_state_t *state, void *user_data) {
    if (general_menu_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(GM_HOLD_LAYER);
    }
    general_menu_tap_state.state = TD_NONE;
}

void app_menu_reset(tap_dance_state_t *state, void *user_data) {
    if (app_menu_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(AM_HOLD_LAYER);
    }
    app_menu_tap_state.state = TD_NONE;
}

void empty_menu_reset(tap_dance_state_t *state, void *user_data) {
    if (empty_menu_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(EM_HOLD_LAYER);
    }
    empty_menu_tap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [GENERAL_MENU] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, general_menu_finished, general_menu_reset),
    [APP_MENU] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, app_menu_finished, app_menu_reset),
    [EMPTY_MENU] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, empty_menu_finished, empty_menu_reset),
    [SEARCH_KEY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, search_key_finished, NULL),
    [BROWSER_KEY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, browser_key_finished, NULL),
    [COMS_KEY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, coms_key_finished, NULL),
    [IDE_KEY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ide_key_finished, NULL),
    [UTILITY_KEY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, utility_key_finished, NULL),
    [COPY_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, copy_paste_finished, NULL),
    [UNDO_REDO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, undo_redo_finished, NULL),
    [TAB_CONTROL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tab_control_finished, NULL),
    [REFRESH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, refresh_finished, NULL),
    [SELLINE_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, selline_par_finished, NULL),
    [BUILD_REBUILD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, build_rebuild_finished, NULL),
    [SAVE_CLOSE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, save_close_finished, NULL),
    [PGUP_HOME] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pgup_home_finished, NULL),
    [PGDN_END] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pgdn_end_finished, NULL),
    [HISTORY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, history_finished, NULL),
    [ZOOM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, zoom_finished, NULL),
    [MSWITCH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mswitch_finished, NULL),

    [B_1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, b_1_finished, NULL),
    [N_2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, n_2_finished, NULL),
    [C_3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, c_3_finished, NULL),
    [V_4] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, v_4_finished, NULL),
    [D_5] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, d_5_finished, NULL),
    [P_6] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, p_6_finished, NULL),

    [SPACE_PLAY] = ACTION_TAP_DANCE_DOUBLE(KC_SPACE, KC_MPLY),
    [Q_B] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_B),
    [E_H] = ACTION_TAP_DANCE_DOUBLE(KC_E, KC_H),
    /*
    [B_1] = ACTION_TAP_DANCE_DOUBLE(KC_B, KC_1),
    [N_2] = ACTION_TAP_DANCE_DOUBLE(KC_N, KC_2),
    [C_3] = ACTION_TAP_DANCE_DOUBLE(KC_C, KC_3),
    [V_4] = ACTION_TAP_DANCE_DOUBLE(KC_V, KC_4),
    [D_5] = ACTION_TAP_DANCE_DOUBLE(KC_D, KC_5),
    [P_6] = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_6),
    */
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 100;
        default:
            return TAPPING_TERM;
    }
}
