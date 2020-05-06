# ScriptEngine

C++11 と DX ライブラリをを使用した  スクリプトエンジンの基本的なプログラムです。  
最終的には、本プログラムを理解し独自のスクリプトエンジンを作成出来る事を目指します。

# Description

使用するスクリプトデータは __UTF-8 BOM 無し__ の Json ファイルを採用しています。  
Json の中身は、単に __文字列の配列__ です。  
(文字列の記述に沿って、本プログラムが動作する独自スクリプト)  

本プログラムを変更する場合には、直接 Json ファイルを書き換えても可能ですが  
__excel\amg_scripts.xlsm__ のエクセルが、本プログラム用の Json を出力するマクロを記述してあります。  
(非エンジニアの使用を想定)

Json パーサーは __picjson__ ライブラリを採用しています。 

スクリプトの構文の詳細は __script_engine.cpp__ に記載されています。

スクリプトプログラムの基本動作は __インタプリタ方式__ で実装されています。

# Requirement

* Visual Studio 2019
* DX ライブラリ

Visual Studio のプロジェクト設定は DX ライブラリ(を使用する為)の指定に準拠しています。

# Note

__ScriptEngine\dxlib__ ディレクトリを作成して  
そこに DX ライブラリの __プロジェクトに追加すべきファイル_VC用__ の中身をコピーして下さい。  
(Visual Studio のプロジェクト設定がされています)

他のディレクトリパスで追加する場合は Visual Studio の追加のインクルードパスを設定して下さい。  
(上記は強制ではありません)

本プロジェクトは DX ライブラリの指定により __マルチバイト文字コード 設定__ となっていますが  
ソースコード自体は __BOM 付き UTF-8 の文字コード__ を採用しています。  
(ファイルの文字コードとプログラムが扱う文字コードは別という事です)

これは昨今の文字コード事情やエディター事情を考慮して Shift_JIS (マルチバイト文字)を使用しない方針のためです。  
ソースコードへの コピー＆ペースト や DxLib.h(Shift_JIS) のインクルードなどマルチバイト文字を扱う際はご注意下さい。
