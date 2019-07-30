#include QMK_KEYBOARD_H
#define _BASE 0
#define _FN 1
#define _RGB 2

enum {
    TD_FN_RGB
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3
};

//Function associated with all tap dances
int cur_dance (qk_tap_dance_state_t *state);

//Functions associated with individual tap dances
void td_finished (qk_tap_dance_state_t *state, void *user_data);
void td_reset (qk_tap_dance_state_t *state, void *user_data);

//Declare variable to track which layer is active
int active_layer;

//Update active_layer
uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case 1:
      active_layer = 1;
      break;
    case 2:
      active_layer = 2;
      break;
    case 3:
      active_layer = 3;
      break;
    default:
      active_layer = 0;
      break;
  }
  return state;
}

//Determine the current tap dance state
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) {return SINGLE_TAP;}
    else return SINGLE_HOLD;
  } else if (state->count == 2) {return DOUBLE_TAP;}
  else return 8;
}

//Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
  .is_press_action = true,
  .state = 0
};

void td_finished (qk_tap_dance_state_t *state, void *user_data) {
  ql_tap_state.state = cur_dance(state);
  switch (ql_tap_state.state) {
    case SINGLE_HOLD: layer_on(_FN); break;
    case DOUBLE_TAP:
      if (active_layer==_RGB) {layer_off(_RGB);}
      else {
        layer_on(_RGB);
      }
  }
  }

void td_reset (qk_tap_dance_state_t *state, void *user_data) {
  if (ql_tap_state.state==SINGLE_HOLD) {layer_off(_FN);}
  ql_tap_state.state = 0;
}


qk_tap_dance_action_t tap_dance_actions[] = {
//   [TD_FN_RGB]  = ACTION_TAP_DANCE_DOUBLE(MO(_FN), TG(_RGB))
 [TD_FN_RGB]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_finished, td_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
		[_BASE] = LAYOUT_65_ansi( /* Base */
        /*
        * ,---------------------------------------------------------------------------------------------------------------------------.
        * |  Esc/~  |   1    |   2     |    3   |    4    |    5   |    6    |    7   |    8    |    9   |    0   |    -     |   =    |  Bksp  |  Home   |
        * |---------+------+------+------+------+------+------+------+------+------+------+------+------+--------+----------|
        * |   Tab    |   Q   |   W   |   E    |   R    |   T    |   Y    |   U   |    I    |    O   |    P    |    [    |    ]    |      \     |   Pgup   |
        * |---------+------+------+------+------+------+------+------+------+------+------+------+------+--------+----------|
        * |Caps/L1|   A   |   S    |   D    |   F    |   G    |   H    |   J   |    K    |    L   |    ;    |    '    |        Enter       |    Pgdn  |
        * |---------+------+------+------+------+------+------+------+------+------+------+------+----------------+----------|
        * | Shft/(  |   Z    |   X   |   C    |   V    |   B    |   N    |   M   |    ,    |    .     |    /    |      Shft/)     |    Up    |   End    |
        * |---------+------+------+------+------+------+------+------+------+------+------+--------------+---------+----------|
        * |   Ctrl   |Super |  Alt  |                       Space                     |   Alt    |   L1   |   Ctrl    |  Left |  Down  |   Right  |
        * ,----------------------------------------------------------------------------------------------------------------------------.
         */
			KC_GESC,       KC_1,     KC_2,     KC_3,  KC_4,  KC_5,  KC_6,    KC_7,  KC_8,    KC_9,     KC_0,     KC_MINS,  KC_EQL,  KC_BSPC,   KC_HOME,\
		    KC_TAB,        KC_Q,     KC_W,     KC_E,  KC_R,  KC_T,  KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,     KC_LBRC,  KC_RBRC, KC_BSLASH, KC_PGUP,\
		    LT(_FN,KC_CAPS),KC_A,     KC_S,     KC_D,  KC_F,  KC_G,  KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN,  KC_QUOT,           KC_ENT,    KC_PGDN,\
		    KC_LSPO,       KC_Z,     KC_X,     KC_C,  KC_V,  KC_B,  KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH,  KC_RSPC,           KC_UP,     KC_END,\
		    KC_LCTL,       KC_LGUI,  KC_LALT,                KC_SPC,                KC_RALT, TD(TD_FN_RGB),    KC_RCTL,    KC_LEFT,           KC_DOWN,   KC_RIGHT),

			[_FN] = LAYOUT_65_ansi( /* FN */
                    /*
        * ,---------------------------------------------------------------------------------------------------------------------------.
        * |             |   F1   |   F2  |   F3  |   F4   |   F5  |   F6  |   F7  |   F8  |   F9  |   F10 |  F11 |   F12  |  Del    |              |
        * |---------+-------+------+------+------+------+------+------+------+------+------+------+-------+--------+----------|
        * |             | Home|  Up  |  End |          |         |          |         |  Ins   |         |  PScr |Slock| Pause| DFU  |               |
        * |---------+-------+------+------+------+------+------+------+------+------+------+------+-------+--------+----------|
        * |             |  Left | Down | Right |       |         |          |         |         |          |         |          |          | EEP_RST |        |
        * |---------+-------+-------+-------+------+------+------+------+------+------+------+------+----------------+--------|
        * |             |         |          |         |          |         |          |         |         |          |         |          |         |  Vol+    |   Mute   |
        * |---------+------+------+------+------+------+------+------+------+------+------+------+------+---------+----------|
        * |            |          |           |                                                  |            |            |           |  Prev   |  Vol-    |    Next   |
        * ,----------------------------------------------------------------------------------------------------------------------------.
         */
			_______,     KC_F1,    KC_F2,   KC_F3,  KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_DEL,   KC_HOME,\
		    _______,     KC_HOME, KC_UP, KC_END,KC_PGUP, _______, _______, _______, KC_INS, _______, KC_PSCR, KC_SLCK,  KC_PAUS, RESET,    _______,\
			_______,      KC_LEFT,  KC_DOWN, KC_RIGHT,KC_PGDN, _______, _______, _______, _______, _______, _______,  _______,          EEP_RST,  _______,\
		    _______,       _______,  _______, _______,_______, _______, _______, _______, _______, _______, _______,  _______,          KC_VOLU,  KC_MUTE,\
		    _______,       _______,  _______,                  _______,                   _______, _______, _______,  KC_MPRV,          KC_VOLD,  KC_MNXT),

			[_RGB]=LAYOUT_65_ansi(/* RGB */
		/* ,---------------------------------------------------------------------------------------------------------------------------.
        * |             |           |         |         |          |         |          |         |          |         |          |         |           |            |              |
        * |---------+-------+------+------+------+------+------+------+------+------+------+------+-------+--------+----------|
        * |             |Toggle|Mode|Hue+|Hue-  |Sat+ |Sat-    | Brt+ | Brt-  |         |         |          |           |            |               |
        * |---------+-------+------+------+------+------+------+------+------+------+------+------+-------+--------+----------|
        * |             | Spd+ | Spd- |         |          |         |          |         |         |          |         |          |          |            |              |
        * |---------+-------+------+------+------+------+------+------+------+------+------+------+-------+--------+----------|
        * |             |         |          |         |          |         |          |         |         |          |         |          |          |             |               |
        * |---------+------+------+------+------+------+------+------+------+------+------+------+------+---------+----------|
        * |            |          |           |                                                  |            |            |           |            |             |               |
        * ,----------------------------------------------------------------------------------------------------------------------------.
        */
       		_______,     _______,    _______,   _______,  _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,   _______,\
		    _______,       RGB_TOG,  RGB_MOD, RGB_HUI,RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______,  _______, _______,    _______,\
			_______,      RGB_SPI,  RGB_SPD, _______,_______, _______, _______, _______, _______, _______, _______,  _______,          _______,  _______,\
		    _______,       _______,  _______, _______,_______, _______, _______, _______, _______, _______, _______,  _______,          _______,  _______,\
		    _______,       _______,  _______,                  _______,                   _______, _______, _______,  _______,          _______,  _______),
};

void rgb_matrix_indicators_user(void)
{
	    if (IS_LED_ON(host_keyboard_leds(), USB_LED_CAPS_LOCK))
	    {
	      rgb_matrix_set_color(8, 0xFF, 0x00, 0x00);
	    }

        if (active_layer == _RGB) {
            rgb_matrix_set_color(65,0xFF,0x00,0x00);
        }
}

void matrix_init_user(void)
{
  //user initialization
}

void matrix_scan_user(void)
{
  //user matrix
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	  return true;
}
