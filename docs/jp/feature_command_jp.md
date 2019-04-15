# Command (旧名 Magic)

Command は [Bootmagic](feature_bootmagic.md) とは異なり、フラッシュメモリに書き込んだりケーブルを抜き挿しすることなくキーボードの動作を変更する方法です。機能的には [Bootmagic Keycodes](feature_bootmagic.md) と重複するものが多くありますので、可能であれば Command の代わりに Bootmagic を使うことをお勧めします。



## Command を有効にするには

デフォルトでは無効になっています。Command を有効にするには `rules.mk` で次のようにします。

    COMMAND_ENABLE = yes



## 使い方

Command を使うには、まず`IS_COMMAND` で定義されているキーコンビネーションを押下します。デフォルトでは両側のShiftキーとなっています。このキーコンビネーションを押したまま、実行したいコマンドに対応したキーを押すとその機能が発動します。

たとえば、使用中のQMK Firmware バージョンを QMK Toolbox コンソールに表示するには `Left Shift`+`Right Shift`+`V` を押すことになります。



## 設定

 `config.h` に以下の値を定義して Command の動作を制御することができます。

|Define |Default | Description |
|-------|--------|-------------|
|`IS_COMMAND()`                      |`(keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)))`|Command を有効にするキーコンビネーション|
|`MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS` |`true`                                                                |ファンクションキー列が有効なレイヤに切り替える|
|`MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS` |`true`                                                                |数字キーが有効なレイヤに切り替える|
|`MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM`|`false`                                                               |カスタムキー(`MAGIC_KEY_LAYER0..9`) が有効なレイヤに切り替える|
|`MAGIC_KEY_HELP1`                   |`H`                                                                   |Show help.|
|`MAGIC_KEY_HELP2`                   |`SLASH`                                                               |Show help.|
|`MAGIC_KEY_DEBUG`                   |`D`                                                                   |debug mode を有効化|
|`MAGIC_KEY_DEBUG_MATRIX`            |`X`                                                                   |matrix debugging を有効化|
|`MAGIC_KEY_DEBUG_KBD`               |`K`                                                                   |keyboard debugging を有効化|
|`MAGIC_KEY_DEBUG_MOUSE`             |`M`                                                                   |mouse debugging を有効化|
|`MAGIC_KEY_VERSION`                 |`V`                                                                   |QMK version をコンソールに表示|
|`MAGIC_KEY_STATUS`                  |`S`                                                                   |現在のキーボード状態を表示|
|`MAGIC_KEY_CONSOLE`                 |`C`                                                                   |Command コンソールを有効化|
|`MAGIC_KEY_LAYER0_ALT1`             |`ESC`                                                                 |レイヤ 0 に切り替え|
|`MAGIC_KEY_LAYER0_ALT2`             |`GRAVE`                                                               |レイヤ 0 に切り替え|
|`MAGIC_KEY_LAYER0`                  |`0`                                                                   |デフォルトレイヤをレイヤ0に変更|
|`MAGIC_KEY_LAYER1`                  |`1`                                                                   |デフォルトレイヤをレイヤ1に変更|
|`MAGIC_KEY_LAYER2`                  |`2`                                                                   |デフォルトレイヤをレイヤ2に変更|
|`MAGIC_KEY_LAYER3`                  |`3`                                                                   |デフォルトレイヤをレイヤ3に変更|
|`MAGIC_KEY_LAYER4`                  |`4`                                                                   |デフォルトレイヤをレイヤ4に変更|
|`MAGIC_KEY_LAYER5`                  |`5`                                                                   |デフォルトレイヤをレイヤ5に変更|
|`MAGIC_KEY_LAYER6`                  |`6`                                                                   |デフォルトレイヤをレイヤ6に変更|
|`MAGIC_KEY_LAYER7`                  |`7`                                                                   |デフォルトレイヤをレイヤ7に変更|
|`MAGIC_KEY_LAYER8`                  |`8`                                                                   |デフォルトレイヤをレイヤ8に変更|
|`MAGIC_KEY_LAYER9`                  |`9`                                                                   |デフォルトレイヤをレイヤ9に変更|
|`MAGIC_KEY_BOOTLOADER`              |`PAUSE`                                                               |キーボード処理を中断しブートローダに入る|
|`MAGIC_KEY_LOCK`                    |`CAPS`                                                                |キーボードをロック|
|`MAGIC_KEY_EEPROM`                  |`E`                                                                   |EEPROMの設定を消去|
|`MAGIC_KEY_NKRO`                    |`N`                                                                   |NKRO ON/OFF トグル|
|`MAGIC_KEY_SLEEP_LED`               |`Z`                                                                   |スリープLED機能を ON/OFFする|
