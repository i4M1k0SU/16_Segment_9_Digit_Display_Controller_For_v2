# 16_Segment_9_Digit_Display_Controller_Ver2.0

## このレポジトリについて
このレポジトリはhtlab.netの「16 Segment 9 Digit Display Controller」に関連するファームウェア・ソフトウェアを管理するレポジトリです。
<pre>
./
├── Firmware
│   ├── Bootloader_For_v2.X コントローラーに直接書き込むブートローダーのファームウェア
│   └── 16_Segment_9_Digit_Display_Controller_For_v2.X コントローラーにUSB経由で書き込まれるコントローラープログラム本体
└── Software
    ├── ArduinoSample コントローラーをシリアル通信で制御するArduinoコードのサンプル
    ├── 16SegTest1 コントローラーをUSB経由で制御するwindowsのソフトウェアのサンプル
    ├── 16SegTest2 コントローラーをUSB経由で制御するwindowsのソフトウェアのサンプル
    └── bootloader コントローラーにプログラムをUSB経由で書き込むソフトウェア
</pre>

## DIP OPTIONS
**DIPスイッチが上に上がっている状態で「無効」、下げている状態で「有効」です。**
複数のスイッチを有効にした場合の動作は保証されていません。必ず、「スイッチを一つも下げない」 もしくは 「一つだけ下げた状態」で使うようにしてください。

| スイッチ番号 | 内容 |
| :--: | :-- |
| 1 | ブートローダー起動 |
| 2 | デモメッセージを表示 |
| 3 | 全桁点灯 | 
| 4 | 未実装 |
| 5 | 未実装 |
| 6 | 未実装 |
| 7 | 時計モード |
| 8 | 時刻設定モード |

## ファームの書き込み方
1. このレポジトリにある/Software/bootloader/bin/win/HIDBootloader.exeをダウンロードします。
2. このレポジトリのreleaseページから最新のバージョンのhexファイルをダウンロードします。
3. LEDボードを取り外し、コントローラーのDIPスイッチの1番を有効にして、コンピューターにUSB接続します。
4. HIDBootloader.exeを起動します。(Device Attached. と表示されているはずです。Device Detached.の場合はDIPスイッチが正しく設定されているかを確認してください。)
5. ファイルオープンアイコン(一番左)をクリックし、2.でダウンロードしたhexファイルを指定します。
6. Erase/Program/Verify Deviceアイコン(真ん中)をクリックし、書き込みます。
7. Erase/Program/Verify sequence completed successfully.と表示されたら、USBを抜き、DIPスイッチの1番を無効にします。
8. LEDと電源を接続して、正しく動作していれば完了です。

## シリアル通信仕様 
文字を設定する桁や、ドットの有無を指定する「コマンドバイト」と、  
実際の文字の内容を指定する「文字バイト」を交互に送ることで自由に文字を表示することができます。  
<pre>
「コマンドバイト」は次のような構成になっています。  
111 x xxxx  
 |  |   |  
 |  |   +-- 設定する桁を二進数で指定します。  
 |  +------ ドットの有無を設定します。0の時にドットなし、1の時にドット有りです。  
 +--------- 先頭は設定上不使用ですが、すべて1でないとコマンドバイトとして認識されません。  
 </pre>
 例えば、0から数えて3桁目にドットなしで設定したければ、コマンドバイトは"11100011"となります。  
 また、0から数えて5桁目にドット有りで設定したければ、コマンドバイトは"11110101"となります。  
 
 「文字バイト」については別ファイルの一覧画像をご覧ください。  
