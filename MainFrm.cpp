// MainFrm.cpp : CMainFrame クラスの実装
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
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame コンストラクション/デストラクション

CToneTable CMainFrame::tones;
CDrumTable CMainFrame::drums;

CMainFrame::CMainFrame() : player(PHRASE_PLAYER_BUFSIZE), sPlayer(SCORE_PLAYER_BUFSIZE)
, tree("..\\Phrases\\")
//, tree("C:\\Documents and Settings\\棚橋　誠\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\Phrases\\")
//, tree(".\\Phrases\\")
, hItem(NULL)/*, nowPhrase(0)*//*, synthCtrl(sPlayer, tones, drums)*/
{
	// TODO: メンバ初期化コードをここに追加してください。
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
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}
	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/
	if (!diaBar.Create(this, IDD_DIABAR,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_DIABAR))
	{
		TRACE0("Failed to create DlgBar\n");
		return -1;      // 作成に失敗
	}
	m_wndMDITabs.Create(this);

	//イメージリスト生成
	images.Create(16, 16, 0, 2, 8);
	images.Add(AfxGetApp()->LoadIcon(IDI_ICON_PHRASE));
	images.Add(AfxGetApp()->LoadIcon(IDI_ICON_CATE));

	//トーンマップのクリエイト
	tones.create("sc8850.ton");
	drums.create("sc8850drum.ton");
	//tones.create("C:\\Documents and Settings\\棚橋　誠\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\sc8850.ton");
	//drums.create("C:\\Documents and Settings\\棚橋　誠\\My Documents\\Visual Studio Projects\\project_swan\\MixOne\\sc8850drum.ton");

	tree.create();
	//ツリーの作成
	makePhrases();
	
	//プレイヤのオープン
	sPlayer.open();
	player.open(1);

	//フレーズプレイヤへメトロノームフレーズ、スコアプレイヤにタイムキーパフレーズ
	player.rythm = tree.getPhrase("Utility", "Utility", 1);
	sPlayer.tk = tree.getPhrase("Utility", "Utility", 2);

	





	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}


// CMainFrame 診断

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
		if((*tItr)->getName() == "Utility")//ユーティリティフォルダは表示しない
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

// CMainFrame メッセージ ハンドラ

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

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

	//sprintf(infoBuf, "フレーズ名:%s\r\n食い込み:%s", pNode->getName(), "なし");
	//selByHitTest();
	pInfoEdit->SetWindowText(pNode->getInfo());
	
		
	*pResult = 0;
}

void CMainFrame::selByHitTest(void)
{

	CPoint pt;
	pt.x= GetCurrentMessage()->pt.x;
	pt.y= GetCurrentMessage()->pt.y;
	//ツリーコントロールのクライアント座標に変換
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
	// TODO : ここにコマンド ハンドラ コードを追加します。

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
	if(MessageBox("一度削除すると元に戻すことはできません\n本当に削除しますか？"
		, 0, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
		return;
	
	if(nowTNode->remove(nowTNode)){
		CTreeCtrl* pTree = (CTreeCtrl*)diaBar.GetDlgItem(IDC_PHRASESTREE);

		pTree->DeleteItem(hItem);
		hItem = NULL;

	}
	// TODO : ここにコマンド ハンドラ コードを追加します。
}

void CMainFrame::OnPrelisten()
{

	player.stop();
	player.setData((CPhrase*)nowTNode);
	player.play();
	// TODO : ここにコマンド ハンドラ コードを追加します。
}

void CMainFrame::OnUpdatePrelisten(CCmdUI *pCmdUI)
{

	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		pCmdUI->Enable(FALSE);

	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
}

void CMainFrame::OnUpdateEntry(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CCategory)))
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateErase(CCmdUI *pCmdUI)
{
	if((hItem == 0) || !nowTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		pCmdUI->Enable(FALSE);
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
}

//void CMainFrame::OnSynth()
//{
//	// TODO : ここにコマンド ハンドラ コードを追加します。
//	
//}


