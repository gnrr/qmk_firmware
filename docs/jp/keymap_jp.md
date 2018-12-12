# キーマップの概要

QMK では、キーマップは C ソースファイル で定義されます。

キーマップのデータ構造は配列の配列で構成されており、外側の配列はレイヤー配列のリスト、内側の配列はキーのリストとなっています。この **配列の配列** という構成を組み立てるため、ほとんどのキーボードでは`KEYMAP()`マクロが使われています。

## キーマップとレイヤ
**`const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]`** は複数のキーマップ情報の **レイヤ** を持っており、それぞれのレイヤには **16ビット** の **アクションコード** が入っています。レイヤは **32個** まで定義できます。

通常のキー定義では、**アクションコード** の上位8ビットが全て0、下位8ビットが **キーコード** から生成されるUSB HID の USAGE ID となっています。

それぞれのレイヤーは同時に有効化されます。レイヤーのインデックスと優先度は 0(最低) 〜31(最高) となっています。



    Keymap: 32 Layers                   Layer: action code matrix
    -----------------                   ---------------------
    stack of layers                     array_of_action_code[row][column]
           ____________ precedence               _______________________
          /           / | high                  / ESC / F1  / F2  / F3   ....
      31 /___________// |                      /-----/-----/-----/-----
      30 /___________// |                     / TAB /  Q  /  W  /  E   ....
      29 /___________/  |                    /-----/-----/-----/-----
       :   _:_:_:_:_:__ |               :   /LCtrl/  A  /  S  /  D   ....
       :  / : : : : : / |               :  /  :     :     :     :
       2 /___________// |               2 `--------------------------
       1 /___________// |               1 `--------------------------
       0 /___________/  V low           0 `--------------------------

キーマップのアクションコードは、TMKの慣例上 キーコードとして扱われることもあります。



### レイヤのステータス
レイヤのステートは2つの32ビットパラメータで決定されます。

- **`default_layer_state`** : 常時有効なBaseレイヤ0〜31を表す
- **`layer_state`**  : それぞれのレイヤのON/OFFをビット列で格納



ふつうはプート後の初期化によってレイヤ 0 が `default_layer` に設定され、他のレイヤは無効化されます。この設定は  `config.h` で変更することができます。たとえば Qwerty の代わりに Colemak に変更するときなど、完全に別のキーレイアウトに切り替えたい時に `default_layer` を変えるといいでしょう。



    Initial state of Keymap          Change base layout
    -----------------------          ------------------
    
      31                               31
      30                               30
      29                               29
       :                                :
       :                                :   ____________
       2   ____________                 2  /           /
       1  /           /              ,->1 /___________/
    ,->0 /___________/               |  0
    |                                |
    `--- default_layer = 0           `--- default_layer = 1
         layer_state   = 0x00000001       layer_state   = 0x00000002



一方、 `layer_state` はBaseレイヤを他のレイヤとオーバーレイするためのもので、たとえば、ナビゲーションキー、ファンクションキー（F1〜F12）、メディアキー、または特別なアクションなどの機能のために使います。

    Overlay feature layer
    ---------------------      bit|status
           ____________        ---+------
      31  /           /        31 |   0
      30 /___________// -----> 30 |   1
      29 /___________/  -----> 29 |   1
       :                        : |   :
       :   ____________         : |   :
       2  /           /         2 |   0
    ,->1 /___________/  ----->  1 |   1
    |  0                        0 |   0
    |                                 +
    `--- default_layer = 1            |
         layer_state   = 0x60000002 <-'



### レイヤの優先度と透過性
***上にあるレイヤほど優先度が高い*** ことに注意してください。ファームウェアは一番上のレイヤから最下層に向かってキーコードを調べていきます。その過程で特定のレイヤに `KC_TRNS`（透過）以外のキーコードがあると、そのキーの検索はそこで停止しそのレイヤより下層のレイヤは参照されません。

レイアウトの一部だけを変更したい場合、変更したいキー以外の場所に `KC_TRNS`を置くことで、その下のレイヤにあるキーコードを有効にすることができます。
`KC_TRNS`（ `_______`はエイリアス）のキーは独自のキーコードを持っておらず、代わりにより低いレイヤのキーコードを参照します。



## `keymap.c ` の一例

この例では、[Clueboard 66％ の古いバージョンのデフォルトキーマップ](https://github.com/qmk/qmk_firmware/blob/ca01d94005f67ec4fa9528353481faa622d949ae/keyboards/clueboard/keymaps/default/keymap.c)を見ていきます。 このファイルを別のブラウザウィンドウで開き、状況に応じて参照するといいでしょう。

`keymap.c`ファイルには3つの主要なセクションがあります。

- [#define定義](#define定義)
- [レイヤとキーマップ](#レイヤとキーマップ)
- [カスタム関数](#カスタム関数)



### #define定義

`keymap.c` は次のように始まっています。

    #include "clueboard.h"
    
    // Helpful defines
    #define GRAVE_MODS  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT)|MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI)|MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
    #define _______ KC_TRNS
    
    // Each layer gets a name for readability.
    // The underscores don't mean anything - you can
    // have a layer called STUFF or any other name.
    // Layer names don't all need to be of the same
    // length, and you can also skip them entirely
    // and just use numbers.
    #define _BL 0
    #define _FL 1
    #define _CL 2

これらは、キーマップとカスタム関数を構築するとき用の `#define` です。

 `GRAVE_MODS`は後で出てくるカスタム関数で使用されます。
 `_______` は、そのレイヤがどのキーをオーバーライドしているのかを見やすいようにするものです。 `_BL`、` _FL`、 `_CL` はレイヤ番号の別名で、それぞれ Baseレイヤ、Functionレイヤ、カスタムレイヤを分かりやすくするためのものです。

### レイヤとキーマップ

このファイルの主な部分は `keymaps[]`の定義です。 ここでは、レイヤとそのレイヤの内容を列挙しています。 この部分は次の定義から始まります。

    const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

この後に、`KEYMAP()` マクロのリストがあります。
 `KEYMAP()` は一枚のレイヤをより簡単に定義するためのものです。 通常、1つまたは複数の **Baseレイヤ** （たとえば QWERTY、Dvorak、Colemakなど）があり、その上に1つ以上の **Functionレイヤ** を重ねます。 レイヤ処理の構造上、上位レイヤの上に下位レイヤを重ねることはできません。

`keymaps[][MATRIX_ROWS][MATRIX_COLS] `には16ビットのアクションコード（quantumキーコード とも呼ばれます）が保持されます。 典型的なキーを表すキーコードの場合、その上位バイトは0であり、その下位バイトは USB HID クラスの USAGE IDです。

> QMKのフォーク元であるTMKは、代わりに `const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]`を使用しており、8ビットのキーコードを保持しています。 いくつかのキーコードは、 `fn_actions[]`配列を介して特定のアクションコードの実行を誘導するために予約されています。



#### Base レイヤ

Clueboard の Baseレイヤは次のようになっています。

      /* Keymap _BL: Base Layer (Default Layer)
       */
    [_BL] = KEYMAP(
      F(0),    KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_GRV,  KC_BSPC,          KC_PGUP, \
      KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,                   KC_PGDN, \
      KC_CAPS, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                             \
      KC_LSFT, KC_NUBS, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,   KC_SLSH,  KC_RO,    KC_RSFT,          KC_UP,            \
      KC_LCTL, KC_LGUI, KC_LALT, KC_MHEN,          KC_SPC,KC_SPC,                        KC_HENK,  KC_RALT,  KC_RCTL,  MO(_FL), KC_LEFT, KC_DOWN, KC_RGHT),

注目すべきところを挙げます。

- Cソースの視点から見ると配列は1つだけですが、各キーがどこにあるか物理的にわかりやすいように、空白が埋め込まれています。

- 素のスキャンコードは `KC_` で始まりますが、特別なキーにはそれがありません。
- 左上のキーには `F(0)`（カスタム関数0）が割りつけられています。
- Fnキーは `MO(_FL)` で定義されており、このキーが押されている間は `_FL` （Function レイヤ）に遷移します。

#### 

#### Function レイヤ

コードの観点では、Functionレイヤは Baseレイヤとなんら変わりません。 ただし、概念的にはそのレイヤーをオーバーレイとして構築します。 この違いは多くの人にとっては重要ではありませんが、より複雑なレイヤ設定を構築する場合は重要になってきます。

    [_FL] = KEYMAP(
      KC_GRV,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______, KC_DEL,           BL_STEP, \
      _______, _______, _______,_______,_______,_______,_______,_______,KC_PSCR,KC_SLCK, KC_PAUS,  _______,  _______,  _______,                   _______, \
      _______, _______, MO(_CL),_______,_______,_______,_______,_______,_______,_______, _______,  _______,  _______,  _______,                           \
      _______, _______, _______,_______,_______,_______,_______,_______,_______,_______, _______,  _______,  _______,  _______,          KC_PGUP,         \
      _______, _______, _______, _______,        _______,_______,                        _______,  _______,  _______,  MO(_FL), KC_HOME, KC_PGDN, KC_END),

注目すべきところを挙げます。

-  `KC_TRNS` の代わりに `_______` を使うことで、このレイヤで変更しているキーを簡単に見分けられます。
- このレイヤで  `_______` のキーを押すと、この下のレイヤの同じ位置のキーが効くよう振る舞います。



### カスタム関数

ファイルの最後にカスタム関数を一つ定義してあります。
この関数は、モディファイヤキーなしなら `KC_ESC` を送信し、モディファイヤキーが押されていれば` KC_GRAVE`を送信するキーを定義しています。 これがうまくいくために必要なものが2つあります。

#### `fn_actions[]`

`fn_actions[]`配列をカスタム関数を指すように定義します。

 キーマップ内の `F(N)`はその配列の要素Nを呼び出します。 Clueboardの場合は次のようになっています。

    const uint16_t PROGMEM fn_actions[] = {
      [0] = ACTION_FUNCTION(0),  // Calls action_function()
    };

この例では、次のセクションで定義する `ACTION_FUNCTION` コールバックを呼ぶようにQMKを構成しています。

> この `fn_actions[]` は主に下位互換性のためにあります。 QMKでは  `fn_actions[]`を使う必要はなく、  `ACTION_FUNCTION(N)` やマクロで生成される他のアクションコードを `keymaps[][MATRIX_ROWS][MATRIX_COLS]`  で直接使うことができます。  `F(N)` のNには0から31までしか指定できませんが、 `keymaps` 内でアクションコードを直接使用すると、この制限がありません。 

すべてのアクション関数を見るには [action_code.h](https://github.com/qmk/qmk_firmware/blob/master/tmk_core/common/action_code.h) を参照します。



#### `action_function()`

keypressイベントを処理するには、 `action_function()` を定義します。 この関数はキーが押されると呼び出され、キーが離されると再び呼び出されます。 ここではこの両方の状況をうまく処理することで、 `KC_ESC` または ` KC_GRAVE` を送信するかどうかを判断させています。



    void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
      static uint8_t mods_pressed;
    
      switch (id) {
        case 0:
          /* Handle the combined Grave/Esc key
           */
          mods_pressed = get_mods()&GRAVE_MODS; // Check to see what mods are pressed
    
          if (record->event.pressed) {
            /* The key is being pressed.
             */
            if (mods_pressed) {
              add_key(KC_GRV);
              send_keyboard_report();
            } else {
              add_key(KC_ESC);
              send_keyboard_report();
            }
          } else {
            /* The key is being released.
             */
            if (mods_pressed) {
              del_key(KC_GRV);
              send_keyboard_report();
            } else {
              del_key(KC_ESC);
              send_keyboard_report();
            }
          }
          break;
      }
    }



# より実践的な詳細について

以上により、独自のキーマップを作成するための基本的な準備は整いました。 詳細については、以下のリソースを参照してください。

* [Keycodes](keycodes.md)
* [Keymap FAQ](faq_keymap.md)



我々はドキュメントの改善に努力しています。もし提案などありましたら [GitHub Issue](https://github.com/qmk/qmk_firmware/issues/new) に提出をお願いします。
