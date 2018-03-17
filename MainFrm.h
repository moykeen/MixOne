// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "afxext.h"

#include "..\..\DynamicLinkLibrarys\mditabs_src\MDITabs.h"
#include "PhrasesDiaBar.h"
#include "Phrase.h"
#include "Player.h"
#include "EntryDialog.h"
#include "SynthControlerDlg.h"

#define PHRASE_PLAYER_BUFSIZE 5400
#define SCORE_PLAYER_BUFSIZE 12000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
	//フレーズ登録ダイアログや、スコアのためにフレーズプレイヤを貸し出す
	CMPhrase_player &getPlayer(void){	return player;	}
	CMScore_player &getSPlayer(void){	return sPlayer;	}
	CPhraseTree &getPhraseTree(){	return tree;	}
	//CPhrase *getPhrase(){	return nowPhrase;	}

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CPhrasesDiaBar diaBar;
    CMDITabs m_wndMDITabs;

protected: //ダイアログ
	CEntryDialog entryDialog;

private: //データメンバ
	
	CPhraseTree tree;
	CImageList images;
	HTREEITEM hItem;//現在選択されているアイテム
	CTNode *nowTNode;//現在選択されているアイテム
	//CPhrase *nowPhrase;//現在選択されているフレーズ

public:
	static CToneTable tones;
	static CDrumTable drums;

    //CSynthControlerDlg synthCtrl;
	CMPhrase_player player;
	CMScore_player sPlayer;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelChanged(NMHDR *, LRESULT*);
	afx_msg void OnTvnLClicked(NMHDR *, LRESULT *);
	afx_msg void OnTvnRClicked(NMHDR *, LRESULT *);
	afx_msg void OnTvnDClicked(NMHDR *, LRESULT *);

	DECLARE_MESSAGE_MAP()


protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

protected:
	//フレーズのツリー＆フレーズの配列を作成
	bool makePhrases(void);
	//ヒットテストを利用してツリーのアイテムを選択
	void selByHitTest(void);
	//// 新しいフレーズを登録
	//void entryNewPhrase(void);
	////フレーズを削除
	//void removePhrase(void);

public:
	afx_msg void OnEntry();
	afx_msg void OnUpdateEntry(CCmdUI *pCmdUI);

	afx_msg void OnErase();
	afx_msg void OnPrelisten();
	afx_msg void OnUpdatePrelisten(CCmdUI *pCmdUI);
	afx_msg void OnUpdateErase(CCmdUI *pCmdUI);
	
//	afx_msg void OnSynth();
};


