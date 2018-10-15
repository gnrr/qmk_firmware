;; http://boiled-mag.hatenablog.jp/entry/2018/05/23/205017

;; flycheck
((nil . ((flycheck-gcc-language-standard . "c")     ; このプロジェクトはCに対応
         (flycheck-gcc-include-path . ("."          ; インクルードパスを設定する
                                         "src"
                                         "include"))
         (compile-command . "cd ~/git-clone/qmk_firmware; make dichotemy:default")               ; make時のデフォルトコマンド
         )))

