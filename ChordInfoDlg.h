#pragma once

#define FRONT_KEY 1500
#define FRONT_TYPE 2000

// CChordInfoDlg �_�C�A���O

class CChordInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CChordInfoDlg)

public:
	CChordInfoDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CChordInfoDlg();

	BOOL Create();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CHORDINFODLG };

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
