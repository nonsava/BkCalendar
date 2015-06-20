# BkCalendar
[Becky!](http://www.rimarts.co.jp/becky-j.htm)で[iCalendar](https://tools.ietf.org/html/rfc5545)（.icsファイル）が添付されたメールを表示したときに、内容を簡易的にポップアップウィンドウに表示するプラグインです。

##概要
* iCalendarが添付されたメールを表示している間、ポップアップウィンドウを表示します。邪魔にならないよう、タイトルバーなし、細枠としています。

* ポップアップウィンドウは移動やサイズ変更できます。位置やサイズはBecky!終了時に保持され、再起動後に復元されます。
* iCalendarの次の内容を表示します。
 * **目的:** 招待/キャンセル/返信のいずれかを表示します。返信のときはさらに、承諾/辞退/参加未定を表示します。
 * **日時:** 予定の開始日時/終了日時を表示します。
 * **場所:** 指定場所を表示します。

## バイナリファイル
バイナリファイルは[binariesリポジトリ](https://github.com/nonsava/binaries/tree/master/BkCalendar)から入手してください。

## インストール方法
Becky!のプラグインフォルダにBkCalendar.dllを入れてください。
プラグインフォルダはBecky!のインストールディレクトリの下にあります。
```
例) C:\Program Files\RimArts\B2\PlugIns\
```

## アンインストール方法
Becky!のプラグインフォルダからBkCalendar.dllを削除してください。
ユーザーごとのプラグインフォルダに保存されている設定ファイルBkCalendar.iniを削除してください。
```
例) C:\Program Files\RimArts\B2\Users\<ユーザー名>\PlugIns\BkCalendar.ini
```

## ビルド方法
* [boost](http://www.boost.org/)を利用しています。ビルド前にboostを準備してください。

  コンパイラがboostを使えるように、`/I`オプションを設定してください。Visual Studioを使用している場合は、
  BkCalendarプロジェクトのプロパティページを開き、[構成プロパティ] > [C/C++] > [全般] > [追加のインクルードディレクトリ]で
  boostを展開したフォルダを指定すればよいです。

* 親フォルダにBecky! SDKファイル（BeckyAPI.h, BeckyAPI.cpp, BkCommon.h, BkCommon.cpp）が
  存在することを前提にしています。

  Becky! SDKはhttp://www.rimarts.co.jp/becky-j.htm の「Becky! Plug-In SDK」リンクからダウンロードしてください。

## ライセンス
[ライセンスファイル](https://github.com/nonsava/BkCalendar/blob/master/LICENSE)をご覧ください。

## バージョン履歴:
* Version 1.0.0 （2015/06/20）
  - 初回リリース
