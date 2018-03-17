// ChordInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MixOne.h"
#include "ChordInfoDlg.h"


// CChordInfoDlg ダイアログ

IMPLEMENT_DYNAMIC(CChordInfoDlg, CDialog)
CChordInfoDlg::CChordInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChordInfoDlg::IDD, pParent)
{
}

CChordInfoDlg::~CChordInfoDlg()
{
}

//void CChordInfoDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}


BEGIN_MESSAGE_MAP(CChordInfoDlg, CDialog)
END_MESSAGE_MAP()


// CChordInfoDlg メッセージ ハンドラ

BOOL CChordInfoDlg::Create()
{
	return CDialog::Create(CChordInfoDlg::IDD);
}
