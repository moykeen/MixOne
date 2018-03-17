// ChildFrm.cpp : CChildFrame �N���X�̎���
//
#include "stdafx.h"
#include "MixOne.h"
#include "MixOneView.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include ".\childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
//ON_COMMAND(ID_SYNTH, OnSynth)
//ON_WM_ACTIVATE()
ON_COMMAND(ID_START, OnStart)
ON_COMMAND(ID_STOP, OnStop)
ON_COMMAND(ID_SYNTH, OnSynth)
END_MESSAGE_MAP()


// CChildFrame �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame::CChildFrame() : created(false)
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
	CMainFrame *pMainFrame =( CMainFrame*)AfxGetApp()->m_pMainWnd;
	pSynth = new CSynthControlerDlg(pMainFrame->sPlayer, pMainFrame->tones, pMainFrame->drums, this);

}

CChildFrame::~CChildFrame()
{
		delete pSynth;
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs ��ύX���āAWindow �N���X�܂��̓X�^�C����ύX���܂��B
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}


// CChildFrame �f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame ���b�Z�[�W �n���h��

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	spWnd.CreateStatic(this,2,2, WS_CHILD |WS_VISIBLE | WS_VSCROLL | WS_HSCROLL);
	created = true;
	//�����Ċ��蓖��
	
	spWnd.CreateView(0,0,RUNTIME_CLASS(CTitleView),CSize(20,10),pContext);
	spWnd.CreateView(1,0,RUNTIME_CLASS(CChView),CSize(20,10),pContext);
	spWnd.CreateView(0,1,RUNTIME_CLASS(CBarView),CSize(20,10),pContext);
	spWnd.CreateView(1,1,RUNTIME_CLASS(CScoreView),CSize(20,10),pContext);
	//spWnd.RecalcLayout();
	
	
	


	return TRUE;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

   if( created)  // m_bSplitterCreated set in OnCreateClient
   {
	   spWnd.SetColumnInfo(0, CCell::cellSize.cx, 10);
	   spWnd.SetRowInfo(0, CCell::cellSize.cy, 10);
      spWnd.RecalcLayout();
   }


	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CChildFrame::OnMDIActivate( BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMainFrame *pMain = (CMainFrame*)GetMDIFrame();
	CScore *pDoc = (CScore*)GetActiveDocument();
	if(pActivateWnd != 0){
		if(pDoc != 0){
			//MessageBox("ONMDI");
			pMain->sPlayer.stop();
			pMain->sPlayer.initChannel(pDoc);
			//pSynth->ShowWindow(SW_HIDE);
		}
	}

	
}

void CChildFrame::OnStart()
{
	CMainFrame *pMain = (CMainFrame*)GetMDIFrame();
	CScore *pDoc = (CScore*)GetActiveDocument();
	pMain->sPlayer.stop();
	pMain->sPlayer.setData(pDoc);
	pMain->sPlayer.play();

	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B

}

void CChildFrame::OnStop()
{
	CMainFrame *pMain = (CMainFrame*)GetMDIFrame();
	pMain->sPlayer.stop();
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CChildFrame::OnSynth()
{
	
	if(pSynth->GetSafeHwnd() == 0){
		CScore *pDoc = (CScore*)GetActiveDocument();
		pSynth->setChannels((pDoc->getCh()));
		pSynth->setScore(pDoc);
		pSynth->Create();		
	}

	/*if(pSynth->GetSafeHwnd() == 0)
		pSynth->Create();*/
	pSynth->ShowWindow(SW_SHOW);

	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}
