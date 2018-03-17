// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
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

// ����
public:
	//�t���[�Y�o�^�_�C�A���O��A�X�R�A�̂��߂Ƀt���[�Y�v���C����݂��o��
	CMPhrase_player &getPlayer(void){	return player;	}
	CMScore_player &getSPlayer(void){	return sPlayer;	}
	CPhraseTree &getPhraseTree(){	return tree;	}
	//CPhrase *getPhrase(){	return nowPhrase;	}

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CPhrasesDiaBar diaBar;
    CMDITabs m_wndMDITabs;

protected: //�_�C�A���O
	CEntryDialog entryDialog;

private: //�f�[�^�����o
	
	CPhraseTree tree;
	CImageList images;
	HTREEITEM hItem;//���ݑI������Ă���A�C�e��
	CTNode *nowTNode;//���ݑI������Ă���A�C�e��
	//CPhrase *nowPhrase;//���ݑI������Ă���t���[�Y

public:
	static CToneTable tones;
	static CDrumTable drums;

    //CSynthControlerDlg synthCtrl;
	CMPhrase_player player;
	CMScore_player sPlayer;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
	//�t���[�Y�̃c���[���t���[�Y�̔z����쐬
	bool makePhrases(void);
	//�q�b�g�e�X�g�𗘗p���ăc���[�̃A�C�e����I��
	void selByHitTest(void);
	//// �V�����t���[�Y��o�^
	//void entryNewPhrase(void);
	////�t���[�Y���폜
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


