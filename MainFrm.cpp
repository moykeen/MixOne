// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "MixOne.h"
#include <fstream>
using namespace std;

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "MixOneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_PHRASESTREE, OnTvnSelChanged)
	ON_NOTIFY(NM_RCLICK , IDC_PHRASESTREE, OnTvnRClicked)
	ON_NOTIFY(NM_CLICK , IDC_PHRASESTREE, OnTvnLClicked)
	ON_NOTIFY(NM_DBLCLK , IDC_PHRASESTREE, OnTvnDClicked)
	ON_COMMAND(ID_ENTRY, OnEntry)
	ON_UPDATE_COMMAND_UI(ID_ENTRY, OnUpdateEntry)
	ON_COMMAND(ID_ERASE, OnErase)
	ON_COMMAND(ID_PRELISTEN, OnPrelisten)
	ON_UPDATE_COMMAND_UI(ID_PRELISTEN, OnUpdatePrelisten)
	ON_UPDATE_COMMAND_UI(ID_ERASE, OnUpdateErase)
	//ON_WM_MDIACTIVATE()
//	ON_COMMAND(ID_SYNTH, OnSynth)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CToneTable CMainFrame::tones;
CDrumTable CMainFrame::drums;

CMainFrame::CMainFrame() : player(PHRASE_PLAYER_BUFSIZE), sPlayer(SCORE_PLAYER_BUFSIZE)
, tree("..\\Phrases\\")
//, tree("C:\\Documents and Settings\\�I���@��\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\Phrases\\")
//, tree(".\\Phrases\\")
, hItem(NULL)/*, nowPhrase(0)*//*, synthCtrl(sPlayer, tones, drums)*/
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
}

CMainFrame::~CMainFrame()
{
	player.close();
	sPlayer.close();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}
	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A������ 3 �s���폜���Ă��������B
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/
	if (!diaBar.Create(this, IDD_DIABAR,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_DIABAR))
	{
		TRACE0("Failed to create DlgBar\n");
		return -1;      // �쐬�Ɏ��s
	}
	m_wndMDITabs.Create(this);

	//�C���[�W���X�g����
	images.Create(16, 16, 0, 2, 8);
	images.Add(AfxGetApp()->LoadIcon(IDI_ICON_PHRASE));
	images.Add(AfxGetApp()->LoadIcon(IDI_ICON_CATE));

	//�g�[���}�b�v�̃N���G�C�g
	tones.create("sc8850.ton");
	drums.create("sc8850drum.ton");
	//tones.create("C:\\Documents and Settings\\�I���@��\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\sc8850.ton");
	//drums.create("C:\\Documents and Settings\\�I���@��\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\sc8850drum.ton");

	tree.create();
	//�c���[�̍쐬
	makePhrases();
	
	//�v���C���̃I�[�v��
	sPlayer.open();
	player.open(1);

	//�t���[�Y�v���C���փ��g���m�[���t���[�Y�A�X�R�A�v���C���Ƀ^�C���L�[�p�t���[�Y
	player.rythm = tree.getPhrase("Utility", "Utility", 1);
	sPlayer.tk = tree.getPhrase("Utility", "Utility", 2);

	





	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return TRUE;
}


// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

bool CMainFrame::makePhrases()
{
	CTreeCtrl* pTree = (CTreeCtrl*)diaBar.GetDlgItem(IDC_PHRASESTREE);
	TV_INSERTSTRUCT TreeCtrlItem;
	char nameBuf[64];

	pTree->SetImageList(&images, TVSIL_NORMAL);
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.iImage = 1;
	HTREEITEM gNode;

	CPhraseTree::const_iterator tItr;
	CGenre::const_iterator gItr;
	CCategory::const_iterator cItr;

	for(tItr = tree.begin(); tItr != tree.end(); tItr++){
		if((*tItr)->getName() == "Utility")//���[�e�B���e�B�t�H���_�͕\�����Ȃ�
			continue;
		sprintf(nameBuf, "%s",(*tItr)->getName());
		TreeCtrlItem.item.pszText = nameBuf;
		TreeCtrlItem.item.lParam = (LPARAM)(*tItr);
		TreeCtrlItem.hParent = gNode = pTree->InsertItem(&TreeCtrlItem);

		for(gItr = (*tItr)->begin(); gItr != (*tItr)->end(); gItr++){
			sprintf(nameBuf, "%s", (*gItr)->getName());
			TreeCtrlItem.item.pszText = nameBuf;
			TreeCtrlItem.item.lParam = (LPARAM)(*gItr);
			TreeCtrlItem.hParent = pTree->InsertItem(&TreeCtrlItem);

			for(cItr = (*gItr)->begin(); cItr != (*gItr)->end(); cItr++){
				sprintf(nameBuf, "%s", (*cItr)->getName());
				TreeCtrlItem.item.pszText = nameBuf;
				TreeCtrlItem.item.lParam = (LPARAM)(*cItr);
				pTree->InsertItem(&TreeCtrlItem);
			}
			TreeCtrlItem.hParent = gNode;
		}
		TreeCtrlItem.hParent = TVI_ROOT;
	}
	return true;
}

// CMainFrame ���b�Z�[�W �n���h��

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	m_wndMDITabs.Update(); 
}

void CMainFrame::OnTvnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CTNode *pNode = (CTNode*)(pNMTreeView->itemNew.lParam);
	CEdit *pInfoEdit = (CEdit*)diaBar.GetDlgItem(IDC_EDIT_INFO);
	//char infoBuf[256];
	//selByHitTest();

	//sprintf(infoBuf, "�t���[�Y��:%s\r\n�H������:%s", pNode->getName(), "�Ȃ�");
	//selByHitTest();
	pInfoEdit->SetWindowText(pNode->getInfo());
	
		
	*pResult = 0;
}

void CMainFrame::selByHitTest(void)
{

	CPoint pt;
	pt.x= GetCurrentMessage()->pt.x;
	pt.y= GetCurrentMessage()->pt.y;
	//�c���[�R���g���[���̃N���C�A���g���W�ɕϊ�
	CTreeCtrl* pTree = (CTreeCtrl*)diaBar.GetDlgItem(IDC_PHRASESTREE);
	pTree->ScreenToClient(&pt);

	UINT uFlags;
	hItem = pTree->HitTest(pt, &uFlags);

	if ((hItem != NULL)/* && (TVHT_ONITEM & uFlags)*/){
		pTree->Select(hItem, TVGN_CARET);
		nowTNode = (CTNode*)pTree->GetItemData(hItem);

	}

}


void CMainFrame::OnTvnRClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	selByHitTest();
	CMenu menu;
	menu.LoadMenu(IDR_MENU_MANAGE);

	if(hItem != NULL)
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON
			, GetCurrentMessage()->pt.x, GetCurrentMessage()->pt.y, this);
	
	*pResult = 0;
}

void CMainFrame::OnTvnLClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	selByHitTest();

	if(hItem != NULL){
		if(nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase))){
			//OnPrelisten();
			CScoreView::setNowPhrase((CPhrase*)nowTNode);
		
		}
	}

	*pResult = 0;
}

void CMainFrame::OnTvnDClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	selByHitTest();

	if(hItem != NULL){
		if(nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase))){
			OnPrelisten();
			//CScoreView::setNowPhrase((CPhrase*)nowTNode);
		
		}
	}

	//MessageBox("");

	*pResult = 0;
}



void CMainFrame::OnEntry()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B

	CCategory *pCate = (CCategory*)nowTNode;
	entryDialog.genre = pCate->getGenAndCate();
	entryDialog.managePath = pCate->getPathName();

	if(entryDialog.DoModal() == DIDENTRY){
		CPhrase *pNew;
		if(entryDialog.info.isJakki)
			pNew = new CJakkiPhrase(entryDialog.info, pCate);
		else
			pNew = new CPhrase(entryDialog.info, pCate);
			
		pCate->entry(pNew);

		CTreeCtrl* pTree = (CTreeCtrl*)diaBar.GetDlgItem(IDC_PHRASESTREE);
		
		TV_INSERTSTRUCT TreeCtrlItem;
		char buf[32];
		sprintf(buf, "%s", pNew->getName());

		TreeCtrlItem.hParent = hItem;
		TreeCtrlItem.hInsertAfter = TVI_LAST;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
		TreeCtrlItem.item.pszText = buf;
		TreeCtrlItem.item.lParam = (LPARAM)pNew;
		pTree->InsertItem(&TreeCtrlItem);
	}

}



void CMainFrame::OnErase()
{
	if(MessageBox("��x�폜����ƌ��ɖ߂����Ƃ͂ł��܂���\n�{���ɍ폜���܂����H"
		, 0, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
		return;
	
	if(nowTNode->remove(nowTNode)){
		CTreeCtrl* pTree = (CTreeCtrl*)diaBar.GetDlgItem(IDC_PHRASESTREE);

		pTree->DeleteItem(hItem);
		hItem = NULL;

	}
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CMainFrame::OnPrelisten()
{

	player.stop();
	player.setData((CPhrase*)nowTNode);
	player.play();
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CMainFrame::OnUpdatePrelisten(CCmdUI *pCmdUI)
{

	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		pCmdUI->Enable(FALSE);

	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMainFrame::OnUpdateEntry(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CCategory)))
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateErase(CCmdUI *pCmdUI)
{
	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		pCmdUI->Enable(FALSE);
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

//void CMainFrame::OnSynth()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//	
//}


