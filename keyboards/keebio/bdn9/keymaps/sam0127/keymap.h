
//COLORS
#define GENERAL_RGB 63, 255, 127
#define GENERAL_EDITOR_RGB 200, 20, 200
#define WORK_RGB 255, 50, 0
#define WORK_EDITOR_RGB 255, 64, 64
#define WORK_MACROS_RGB 0, 255, 64
#define UTIL_RGB 255, 0, 0

#define GENERAL_HUE 99
#define GENERAL_EDITOR_HUE 212
#define WORK_HUE 10
#define WORK_EDITOR_HUE 240
#define WORK_MACROS_HUE 135
#define APP_HUE 205
#define UTIL_HUE 0

//MACROS:
#define M_ALT_F4 LALT(KC_F4)
#define M_ALT_LEFT LALT(KC_LEFT)
#define M_ALT_RIGHT LALT(KC_RIGHT)
#define M_ALT_TAB LCTL(LALT(KC_TAB))
#define M_CTRL_ALT_H LCTL(LALT(KC_H))
#define M_CTRL_ALT_W LCTL(LALT(KC_W))
#define M_CTRL_B LCTL(KC_B)
#define M_CTRL_C LCTL(KC_C)
#define M_CTRL_EQUAL LCTL(KC_EQUAL)
#define M_CTRL_F LCTL(KC_F)
#define M_CTRL_F5 LCTL(KC_F5)
#define M_CTRL_H LCTL(KC_H)
#define M_CTRL_MINUS LCTL(KC_MINUS)
#define M_CTRL_N LCTL(KC_N)
#define M_CTRL_PGDN LCTL(KC_PGDN)
#define M_CTRL_PGUP LCTL(KC_PGUP)
#define M_CTRL_S LCTL(KC_S)
#define M_CTRL_SHIFT_B LCTL(LSFT(KC_B))
#define M_CTRL_SHIFT_DEL LCTL(LSFT(KC_DEL))
#define M_CTRL_SHIFT_F LCTL(LSFT(KC_F))
#define M_CTRL_SHIFT_N LCTL(LSFT(KC_N))
#define M_CTRL_SHIFT_P LCTL(LSFT(KC_P))
#define M_CTRL_SHIFT_R LCTL(LSFT(KC_R))
#define M_CTRL_SHIFT_T LCTL(LSFT(KC_T))
#define M_CTRL_SHIFT_Y LCTL(LSFT(KC_Y))
#define M_CTRL_SPACE LCTL(KC_SPACE)
#define M_CTRL_T LCTL(KC_T)
#define M_CTRL_V LCTL(KC_V)
#define M_CTRL_W LCTL(KC_W)
#define M_CTRL_X LCTL(KC_X)
#define M_CTRL_Y LCTL(KC_Y)
#define M_CTRL_Z LCTL(KC_Z)
#define M_DIV "<div>" SS_TAP(X_ENTER) SS_TAP(X_ENTER) "</div>"
#define M_DOUBLE_CLICK SS_TAP(X_BTN1) SS_DELAY(20) SS_TAP(X_BTN1)
#define M_GIT_STATUS "git status"
#define M_NPM_RUN_BUILD "npm run build"
#define M_SELECT_WORD SS_LCTL(SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_RGHT)))
#define M_SELECT_LINE SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END))
#define M_SHIFT_UP LSFT(KC_UP)
#define M_SHIFT_DN LSFT(KC_DOWN)
#define M_SHIFT_LEFT LSFT(KC_LEFT)
#define M_SHIFT_RIGHT LSFT(KC_RIGHT)
#define M_WIN_1 LWIN(KC_1)
#define M_WIN_2 LWIN(KC_2)
#define M_WIN_3 LWIN(KC_3)
#define M_WIN_4 LWIN(KC_4)
#define M_WIN_5 LWIN(KC_5)
#define M_WIN_6 LWIN(KC_6)
#define M_WIN_7 LWIN(KC_7)
#define M_WIN_8 LWIN(KC_8)
#define M_WIN_9 LWIN(KC_9)
#define M_WIN_0 LWIN(KC_0)
#define M_WIN_SHIFT_LEFT LWIN(LSFT(KC_LEFT))
#define M_WIN_TAB LWIN(KC_TAB)

//LAYER ITERATORS
#define GENERAL_CYCLE_START 0
#define GENERAL_CYCLE_END 1

#define APP_CYCLE_START 2
#define APP_CYCLE_END 2

#define BASE_LAYERS_START 0
#define BASE_LAYERS_END 3
#define UTIL_LAYERS_START 4
#define UTIL_LAYERS_END 7

#define NUM_BASE_LAYERS 4
#define NUM_UTIL_LAYERS 4

#define NUM_MODE_LAYERS 2

//TAPDANCE DEFINITIONS

//GENERAL_MENU:
#define GM_SINGLE_TAP KC_ESC
#define GM_HOLD_LAYER UTIL

//EMPTY_MENU
#define EM_SINGLE_TAP KC_NO
#define EM_HOLD_LAYER RGB_UTIL

//APP_MENU
#define AM_SINGLE_TAP KC_ESC
#define AM_HOLD_LAYER APP_UTIL

/////
enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum layer_names {
    GENERAL,
    MACROS,
    APP,
    EMPTY,
    UTIL,
    APP_UTIL,
    RGB_UTIL,
    MENU,
};

//Global variables :3
uint8_t active_base_layer = 0;
uint8_t active_util_layer = 255;
bool rgb_disabled = false;
bool is_alt_tab = false;
bool is_selecting_line = false;
bool is_selecting_par = false;

uint8_t mode_layers[NUM_MODE_LAYERS] = {GENERAL, APP};

enum custom_keycodes {
    DOUBLE_CLICK = SAFE_RANGE,
    SELECT_WORD,
    SELECT_LINE,
    NPM_RUN_BUILD,
    GIT_STATUS,
    DIV,
};

//// TAPDANCE
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_TRIPLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    GENERAL_MENU,
    APP_MENU,
    EMPTY_MENU,
    SEARCH_KEY,
    BROWSER_KEY,
    COMS_KEY,
    IDE_KEY,
    UTILITY_KEY,
    COPY_PASTE,
    UNDO_REDO,
    TAB_CONTROL,
    REFRESH,
    SELLINE_PAR,
    BUILD_REBUILD,
    SAVE_CLOSE,
    PGUP_HOME,
    PGDN_END,
    HISTORY,
    ZOOM,
    MSWITCH,

    SPACE_PLAY,
    Q_B,
    E_H,
    B_1,
    N_2,
    C_3,
    V_4,
    D_5,
    P_6
};

td_state_t cur_dance(tap_dance_state_t *state);

void general_menu_finished(tap_dance_state_t *state, void *user_data);
void app_menu_finished(tap_dance_state_t *state, void *user_data);
void empty_menu_finished(tap_dance_state_t *state, void *user_data);
void search_key_finished(tap_dance_state_t *state, void *user_data);
void browser_key_finished(tap_dance_state_t *state, void *user_data);
void coms_key_finished(tap_dance_state_t *state, void *user_data);
void ide_key_finished(tap_dance_state_t *state, void *user_data);
void utility_key_finished(tap_dance_state_t *state, void *user_data);
void copy_paste_finished(tap_dance_state_t *state, void *user_data);
void undo_redo_finished(tap_dance_state_t *state, void *user_data);
void tab_control_finished(tap_dance_state_t *state, void *user_data);
void refresh_finished(tap_dance_state_t *state, void *user_data);
void selline_par_finished(tap_dance_state_t *state, void *user_data);
void build_rebuild_finished(tap_dance_state_t *state, void *user_data);
void save_close_finished(tap_dance_state_t *state, void *user_data);
void pgup_home_finished(tap_dance_state_t *state, void *user_data);
void pgdn_end_finished(tap_dance_state_t *state, void *user_data);
void history_finished(tap_dance_state_t *state, void *user_data);
void zoom_finished(tap_dance_state_t *state, void *user_data);
void mswitch_finished(tap_dance_state_t *state, void *user_data);

void b_1_finished(tap_dance_state_t *state, void *user_data);
void n_2_finished(tap_dance_state_t *state, void *user_data);
void c_3_finished(tap_dance_state_t *state, void *user_data);
void v_4_finished(tap_dance_state_t *state, void *user_data);
void d_5_finished(tap_dance_state_t *state, void *user_data);
void p_6_finished(tap_dance_state_t *state, void *user_data);

void general_menu_reset(tap_dance_state_t *state, void *user_data);
void app_menu_reset(tap_dance_state_t *state, void *user_data);
void empty_menu_reset(tap_dance_state_t *state, void *user_data);
void selline_par_reset(tap_dance_state_t *state, void *user_data);
