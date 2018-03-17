#pragma once

#define FRONT_KEY 1500
#define FRONT_TYPE 2000

// CChordInfoDlg ダイアログ

class CChordInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CChordInfoDlg)

public:
	CChordInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChordInfoDlg();

	BOOL Create();

// ダイアログ データ
	enum { IDD = IDD_CHORDINFODLG };

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
