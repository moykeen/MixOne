// ChildFrm.h : CChildFrame クラスのインターフェイス
//


#pragma once

#include "..\..\DynamicLinkLibrarys\Flat-Splitter\FlatSplitterWnd.h"
#include "Player.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 属性
public:

// 操作
public:

// オーバーライド
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:

	CFlatSplitterWnd spWnd;
	bool created;

	CSynthControlerDlg *pSynth;

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMDIActivate(BOOL, CWnd*, CWnd*);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//afx_msg void OnSynth();
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSynth();
};
