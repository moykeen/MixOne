#pragma once
#include "afxwin.h"
#include "Phrase.h"
#include "Player.h"

//DoModalの返り値
#define DIDENTRY 3
#define DIDNOTENTRY 4

#define FRONT_COMBO_KEY 1600
#define FRONT_COMBO_TYPE 1700

// CEntryDialog ダイアログ

class CEntryDialog : public CDialog
{
	DECLARE_DYNAMIC(CEntryDialog)

public:
	CEntryDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEntryDialog();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ENTRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 新しく登録するフレーズの名前
	CString name;
	
	BOOL isJakki;
	
	CButton listenButton;
	CButton entryButton;
	CEdit midiFile;
	CString genre;
	
	CFileDialog fileDialog;
	PhraseInfo info;
	CString managePath;

public:
	afx_msg void OnBnClickedButtonSelfile();

protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedButtonPrelisten();
	afx_msg void OnBnClickedButtonEntry();

	
	afx_msg void OnBnClickedButtonDelete();
};
