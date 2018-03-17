#pragma once
#include "afxwin.h"
#include "Phrase.h"
#include "Player.h"

//DoModal�̕Ԃ�l
#define DIDENTRY 3
#define DIDNOTENTRY 4

#define FRONT_COMBO_KEY 1600
#define FRONT_COMBO_TYPE 1700

// CEntryDialog �_�C�A���O

class CEntryDialog : public CDialog
{
	DECLARE_DYNAMIC(CEntryDialog)

public:
	CEntryDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CEntryDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_ENTRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �V�����o�^����t���[�Y�̖��O
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
