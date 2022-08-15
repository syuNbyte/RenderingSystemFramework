#pragma once

#include<windows.h>

//ShowWindow関数のパラメータとして渡す値
enum WindowModeType {

	Hide = SW_HIDE,								//0	ウィンドウを非表示にする
	Normal = SW_SHOWNORMAL,						//1	ウィンドウをアクティブにして表示する　※ウィンドウが最小化・最大化されている場合は元のウィンドウの状態に戻す
	Minimized = SW_SHOWMINIMIZED,				//2	ウィンドウを最小化
	Maximized = SW_SHOWMAXIMIZED,				//3	ウィンドウを最大化
	No_activate = SW_SHOWNOACTIVATE,			//4	ウィンドウをアクティブにはせずに表示
	Show = SW_SHOW,								//5	ウィンドウをアクティブにして現在の位置とサイズで表示
	MinAndNextZOrderWindow = SW_MINIMIZE,	//6	ウィンドウを最小化して、このウィンドウの次に最前面に位置するウィンドウ(次のZオーダーにあるトップレベルウィンドウ)をアクティブにする
	MinNoActive = SW_SHOWMINNOACTIVE,		//7	ウィンドウは非アクティブにして最小化された状態で表示
	NA = SW_SHOWNA,								//8	ウィンドウを非アクティブにして現在の位置とサイズで表示
	Restore = SW_RESTORE,						//9	ウィンドウをアクティブにして表示する　※ウィンドウが最小化・最大化されている場合は元のウィンドウの状態に戻す
	Default = SW_SHOWDEFAULT,					//10	アプリケーション起動時にCreateProcess関数に設定された値を使用する
	ForceMinimize = SW_FORCEMINIMIZE			//11	他のスレッドからウィンドウを最小化する場合に使用する

};