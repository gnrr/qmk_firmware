;; http://boiled-mag.hatenablog.jp/entry/2018/05/23/205017

;; flycheck
((nil . ((flycheck-gcc-language-standard . "c")     ; このプロジェクトはCに対応
         (flycheck-gcc-include-path . ("."          ; インクルードパスを設定する
                                         "~/git-clone/qmk_firmware/keyboards/handwired/asceension"
                                         "~/git-clone/qmk_firmware/quantum"
                                         "/usr/local/Cellar/avr-gcc/7.3.0/avr/include"
                                         "/usr/include"))
         (compile-command . "cd ~/git-clone/qmk_firmware; make handwired/asceension:default")               ; make時のデフォルトコマンド
         )))

