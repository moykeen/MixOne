// ChildFrm.h : CChildFrame �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\..\DynamicLinkLibrarys\Flat-Splitter\FlatSplitterWnd.h"
#include "Player.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:

// ����
public:

// �I�[�o�[���C�h
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
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

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
