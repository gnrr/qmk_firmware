# Bootmagic と Bootmagic キーコード

3つの独立した機能があり、どれもファームウェアを書き変えることなくキーボードの動作を変更することができます。 それぞれの機能は似ていますが、キーボードの状況に応じて使い分けます。

Bootmagic は、キーボードの初期化中に設定を変更するシステムです。 Bootmagicコマンドを起動するには、ブートマジックキー（ほとんどのキーボードでは `KC_SPACE`）と1つ以上のコマンドキーを押し続けます。

Bootmagic キーコードを使うと、キーボードの初期化後にBootmagic と同じことができます。Bootmagic キーコードを使うには、他のキーと同じように `MAGIC_`で始まるキーコードを割り当てます。

以前は Magic と呼ばれていた Command という別の方法でも、キーボードの動作を変更できます。Command の詳細については、[Command](feature_command_jp.md) のドキュメントを参照してください。



## Bootmagic を有効化するには

デフォルトでは無効になっています。Bootmagic を有効にするには `rules.mk` で次のようにします。

    BOOTMAGIC_ENABLE = yes



## ホットキーとキーコード

この表では、Bootmagicのデフォルトのホットキーとキーコードについて説明します。 これらは、キーボードレベルまたはキーマップレベルで上書きされます。 一部の機能は、どちらか一方のレベルでしか使用できないものもあります。

Bootmagic を使用するには、 `BOOTMAGIC_KEY_SALT`（デフォルトでは` KC_SPACE`）とホットキーを同時押ししながらキーボードを接続します。キーコードを使用するには、レイヤにそのキーコードを割り当てます。 たとえば、ほとんどのキーボードでは `Space` + `B` を同時押ししながら接続すると、ブートローダーモードに入ります。

| Hotkey      | Keycode                            | Description                                             |
| ----------- | ---------------------------------- | ------------------------------------------------------- |
| `ESC`       |                                    | Bootmagic で EEPROM に保存した設定を無視する            |
| `B`         | `RESET`                            | ブートローダに入る                                      |
| `D`         | `DEBUG`                            | debugging を有効化（シリアルへのメッセージ出力）        |
| `X`         |                                    | matrix debugging を有効化                               |
| `K`         |                                    | keyboard debugging を有効化                             |
| `M`         |                                    | mouse debugging を有効化                                |
| `BACKSPACE` |                                    | EEPROM に保存した設定を消去                             |
| `CAPSLOCK`  | `MAGIC_CAPSLOCK_TO_CONTROL`        | `Caps` を `Ctrl`として扱う                              |
|             | `MAGIC_UNCAPSLOCK_TO_CONTROL`      | `Caps` を `Ctrl`として扱うのを中止                      |
| `LCTRL`     | `MAGIC_SWAP_CONTROL_CAPSLOCK`      | `Ctrl` と `Caps` をスワップ                             |
|             | `MAGIC_UNSWAP_CONTROL_CAPSLOCK`    | `Ctrl` と `Caps` のスワップを中止                       |
|             | `MAGIC_SWAP_ALT_GUI`               | `Alt` と `GUI` を左右両方ともスワップ                   |
|             | `MAGIC_UNSWAP_ALT_GUI`             | `Alt` と `GUI` のスワップを中止                         |
| `LALT`      | `MAGIC_SWAP_LALT_LGUI`             | 左側の `Alt` と `GUI`をスワップ（Mac の Opt と Cmd 用） |
|             | `MAGIC_UNSWAP_LALT_LGUI`           | 左側の `Alt` と `GUI`のスワップを中止                   |
| `RALT`      | `MAGIC_SWAP_RALT_RGUI`             | 右側の `Alt` と `GUI`をスワップ                         |
|             | `MAGIC_UNSWAP_RALT_RGUI`           | 右側の `Alt` と `GUI`のスワップを中止                   |
| `LGUI`      | `MAGIC_NO_GUI`                     | `GUI`を無効化 （ゲーム中に Windows キー無効化など）     |
|             | `MAGIC_UNNO_GUI`                   | `GUI` を有効化                                          |
| `GRAVE`     | `MAGIC_SWAP_GRAVE_ESC`             | ``~` と `ESC` をスワップ                                |
|             | `MAGIC_UNSWAP_GRAVE_ESC`           | ``~` と `ESC` のスワップを中止                          |
| `BACKSLASH` | `MAGIC_SWAP_BACKSLASH_BACKSPACE`   | `\` と `Backspace` をスワップ                           |
|             | `MAGIC_UNSWAP_BACKSLASH_BACKSPACE` | `\` と `Backspace` のスワップを中止                     |
| `N`         | `MAGIC_HOST_NKRO`                  | NKRO を強制 ON                                          |
|             | `MAGIC_UNHOST_NKRO`                | NKRO を強制 OFF                                         |
|             | `MAGIC_TOGGLE_NKRO`                | NKRO ON/OFF トグル                                      |
| `0`         | `DF(0)`                            | ブート時にデフォルトレイヤをレイヤ0に変更               |
| `1`         | `DF(1)`                            | ブート時にデフォルトレイヤをレイヤ1に変更               |
| `2`         | `DF(2)`                            | ブート時にデフォルトレイヤをレイヤ2に変更               |
| `3`         | `DF(3)`                            | ブート時にデフォルトレイヤをレイヤ3に変更               |
| `4`         | `DF(4)`                            | ブート時にデフォルトレイヤをレイヤ4に変更               |
| `5`         | `DF(5)`                            | ブート時にデフォルトレイヤをレイヤ5に変更               |
| `6`         | `DF(6)`                            | ブート時にデフォルトレイヤをレイヤ6に変更               |
| `7`         | `DF(7)`                            | ブート時にデフォルトレイヤをレイヤ7に変更               |



## 設定

Bootmagic には、キーボードやキーマップを定義するときに使う `#define`が多数あります。 これらを使うにはキーボードレベル/キーマップレベルを問わず`config.h`に定義します。

|Define |Default|Description |
|-------|-------|------------|
|`BOOTMAGIC_KEY_SALT`|`KC_SPACE`|初期化中に押し続けるBootmagic起動するキー|
|`BOOTMAGIC_KEY_SKIP`|`KC_ESC`|Bootmagic で EEPROMに保存した設定を無視するホットキー|
|`BOOTMAGIC_KEY_EEPROM_CLEAR`|`KC_BSPACE`|EEPROMに保存した設定を消去するホットキー|
|`BOOTMAGIC_KEY_BOOTLOADER`|`KC_B`|ブートローダに入るホットキー|
|`BOOTMAGIC_KEY_DEBUG_ENABLE`|`KC_D`|debug mode を有効化するホットキー|
|`BOOTMAGIC_KEY_DEBUG_MATRIX`|`KC_X`|matrix debugging mode を有効化するホットキー|
|`BOOTMAGIC_KEY_DEBUG_KEYBOARD`|`KC_K`|keyboard debugging mode を有効化するホットキー|
|`BOOTMAGIC_KEY_DEBUG_MOUSE`|`KC_M`|mouse debugging mode を有効化するホットキー|
|`BOOTMAGIC_KEY_SWAP_CONTROL_CAPSLOCK`|`KC_LCTRL`||
|`BOOTMAGIC_KEY_CAPSLOCK_TO_CONTROL`|`KC_CAPSLOCK`||
|`BOOTMAGIC_KEY_SWAP_LALT_LGUI`|`KC_LALT`||
|`BOOTMAGIC_KEY_SWAP_RALT_RGUI`|`KC_RALT`||
|`BOOTMAGIC_KEY_NO_GUI`|`KC_LGUI`||
|`BOOTMAGIC_KEY_SWAP_GRAVE_ESC`|`KC_GRAVE`||
|`BOOTMAGIC_KEY_SWAP_BACKSLASH_BACKSPACE`|`KC_BSLASH`||
|`BOOTMAGIC_HOST_NKRO`|`KC_N`||
|`BOOTMAGIC_KEY_DEFAULT_LAYER_0`|`KC_0`|デフォルトレイヤをレイヤ 0 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_1`|`KC_1`|デフォルトレイヤをレイヤ 1 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_2`|`KC_2`|デフォルトレイヤをレイヤ 2 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_3`|`KC_3`|デフォルトレイヤをレイヤ 3 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_4`|`KC_4`|デフォルトレイヤをレイヤ 4 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_5`|`KC_5`|デフォルトレイヤをレイヤ 5 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_6`|`KC_6`|デフォルトレイヤをレイヤ 6 にするホットキー|
|`BOOTMAGIC_KEY_DEFAULT_LAYER_7`|`KC_7`|デフォルトレイヤをレイヤ 7 にするホットキー|
