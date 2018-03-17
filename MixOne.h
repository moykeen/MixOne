// MixOne.h : MixOne アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"       // メイン シンボル


// CMixOneApp:
// このクラスの実装については、MixOne.cpp を参照してください。
//

class CMixOneApp : public CWinApp
{
public:
	CMixOneApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMixOneApp theApp;
