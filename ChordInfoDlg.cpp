// ChordInfoDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MixOne.h"
#include "ChordInfoDlg.h"


// CChordInfoDlg �_�C�A���O

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


// CChordInfoDlg ���b�Z�[�W �n���h��

BOOL CChordInfoDlg::Create()
{
	return CDialog::Create(CChordInfoDlg::IDD);
}
