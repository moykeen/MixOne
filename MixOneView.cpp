// MixOneView.cpp : CScoreView �N���X�̎���
//

#include "stdafx.h"
#include "MixOne.h"

#include "MixOneDoc.h"
#include "MixOneView.h"
#include ".\mixoneview.h"
#include "MainFrm.h"
#include "Phrase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CCell

const int CCell::space= 8;
const CSize CCell::cellSize(122, 27);
const COLORREF CCell::color[] = {RGB(150, 173, 232),RGB(170, 170, 170)};

void CCell::setCellRect(int i, int j)
{
	top = j * (cellSize.cy + space);
	bottom = top + cellSize.cy;
	left = + cellSize.cx * i;
	right = left + cellSize.cx;

}

//CBoxCell
void CBoxCell::paint(CDC *pDC)
{
	CCell::paint(pDC);
	if(pBox != 0){
		//MessageBox(0, "pBox != 0", 0,0);
		//pDC->TextOut(this->left, top, pBox->getPhrase().getName());
		pDC->DrawText(pBox->getPhrase().getName(), this, DT_CENTER);
	}

}

//CChCell
void CChCell::paint(CDC *pDC)
{
	CCell::paint(pDC);
	char buf[10];
	sprintf(buf, "Ch %d", j);
	pDC->TextOut(left + 5, top + 5, buf);
}

// CCellView



IMPLEMENT_DYNCREATE(CCellView, CScrollView)

CCellView::CCellView() : nowCell(0)
{
}

CCellView::~CCellView()
{
}


BEGIN_MESSAGE_MAP(CCellView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCellView �`��

void CCellView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: ���̃r���[�̃T�C�Y�̍��v���v�Z���܂��B
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

}

BOOL CCellView::OnEraseBkgnd(CDC* pDC)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CBrush* lpOld;
	CBrush	cBack;
	CRect	rect;

	cBack.CreateSolidBrush(CCell::color[1]);		//�D�F
	

	lpOld = pDC->SelectObject(&cBack);
	//lpOld = pDC->SelectObject(&back);
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
	pDC->SelectObject(lpOld);

	return TRUE;

	//return CScrollView::OnEraseBkgnd(pDC);
}


void CCellView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
	pDC->TextOut(20, 20, "CELLVIEW");
}


// CCellView �f�f

#ifdef _DEBUG
void CCellView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCellView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CCellView ���b�Z�[�W �n���h��

void CCellView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CScrollView::OnPrepareDC(pDC, pInfo);
	org = pDC->GetViewportOrg();
}

CPoint CCellView::getCellIndex(CPoint pt)
{
	CPoint index;
	CPoint point = absCoordinate(pt);//-= org;//���Ń}�C�i�X���̓q�~�c

	index.y = point.y / (CCell::cellSize.cy + CCell::space);
	index.x = point.x / CCell::cellSize.cx;
	
	if((((point.x < 0) || (point.y < 0))) || ((point.y % (CCell::cellSize.cy + CCell::space)) > CCell::cellSize.cy))
		throw int();

	return index;

}


// CScoreView

IMPLEMENT_DYNCREATE(CScoreView, CCellView)

BEGIN_MESSAGE_MAP(CScoreView, CCellView)
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	//ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
//	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_COPY, OnPhraseCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,OnUpdatePhraseCopy)
	ON_COMMAND(ID_EDIT_CUT, OnPhraseCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT,OnUpdatePhraseCut)
	ON_COMMAND(ID_EDIT_PASTE, OnPhrasePaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,OnUpdatePhrasePaste)
	ON_COMMAND(ID_CHORDINFO, OnChordinfo)
END_MESSAGE_MAP()

// CScoreView �R���X�g���N�V����/�f�X�g���N�V����

CPhrase *CScoreView::nowPhrase = 0;
bool CScoreView::loadedImage = false;
CDib CScoreView::dibs[NUM_IMAGE];

CScoreView::CScoreView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B
	

}

CScoreView::~CScoreView()
{
	int i;
	vector<CBoxCell *>::const_iterator itr;
	for(i = 0; i < table.size(); i++){
		for(itr = table.at(i).begin(); itr != table.at(i).end(); itr++){
			delete (*itr);
		}
	}
}

BOOL CScoreView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CScrollView::PreCreateWindow(cs);
}

// CScoreView �`��

void CScoreView::OnDraw(CDC* pDC)
{
	CScore* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B

	RECT rect = {0, 0, (pDoc->getNumOfBar() + 1) * CCell::cellSize.cx, CCell::cellSize.cy};

	int i, j;

	for(i = 0; i < 16; i++){

		pDC->Rectangle(&rect);
		rect.top += CCell::cellSize.cy + CCell::space;
		rect.bottom += CCell::cellSize.cy + CCell::space;
		//pDC->SelectObject(newBrush[i % 2]);
	}

	CPoint pt;
	CPoint dest;
	for(i = 1; i < pDoc->getNumOfBar() + 1; i++){
		pt.x = CCell::cellSize.cx * i;
		pt.y = 0;
		dest.x = pt.x;
		dest.y = pt.y +( CCell::cellSize.cy + CCell::space) * pDoc->getNumOfCh() - CCell::space;
		pDC->MoveTo(pt);
			pDC->LineTo(dest);
		
	}

	

	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(220, 220, 240));

	CFont font;
	font.CreatePointFont(120, "");
	CGdiObject *pOld = pDC->SelectObject(&font);

	
	vector<CBoxCell *>::const_iterator itr;
	for(i = 0; i < table.size(); i++){
		for(itr = table.at(i).begin(); itr != table.at(i).end(); itr++){
			(*itr)->paint(pDC);
		}
	}

	if(nowCell != 0){
		pDC->DrawFocusRect(nowCell);
	}

	pDC->SelectObject(pOld);
}


// CScoreView ���

BOOL CScoreView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �f�t�H���g�̈������
	return DoPreparePrinting(pInfo);
}

void CScoreView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CScoreView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}


// CScoreView �f�f

#ifdef _DEBUG
void CScoreView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScoreView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

//CMixOneDoc* CScoreView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMixOneDoc)));
//	return (CMixOneDoc*)m_pDocument;
//}
#endif //_DEBUG


// CScoreView ���b�Z�[�W �n���h��

void CScoreView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//this->SetScaleToFitSize(getScoreSize());
	SetScrollSizes(MM_TEXT, getScoreSize());

	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
		
	CScore *pDc = GetDocument();
	int i, j;
	CBoxCell *pBCell;

	//MessageBox("");

	for(i = 0; i < pDc->getNumOfBar() + 1; i++){
		table.push_back(vector<CBoxCell *>());
		for(j = 0; j < 17; j++){
			pBCell = new CBoxCell(i, j);
			if(pDc->existBox(i + 1, j+1)){
				pBCell->setBox(&(pDc->operator ()(i+1 , j+1)));
				//�C���[�W�̓t���[�Y��ID�Ō��߂�
				pBCell->setImage(&dibs[pBCell->getBox()->getPhrase().getID() % NUM_IMAGE]);
			}

			table.at(i).push_back(pBCell);
		}
	}
	
	if(!loadedImage){
		for(int i = 0; i < NUM_IMAGE ; i++){
			dibs[i].AttachMemory((void*) ::LoadResource(0
				, ::FindResource(0, MAKEINTRESOURCE(FRONT_IMAGE + i), RT_BITMAP)));
		}
		loadedImage = true;
	}

	POSITION pos = pDc->GetFirstViewPosition();
	CView *pV;

	while(pos != 0){
		pV = pDc->GetNextView(pos);
		if(pV->IsKindOf(RUNTIME_CLASS(CBarView))){
			pBView = (CBarView*)pV;
			break;
		}
	}

	if(chordInfoDlg.GetSafeHwnd() == 0){
		chordInfoDlg.Create();	
	}
	
}



// �N���b�N���ꂽ�ꏊ�ɂ���Cell��Ԃ��B
CBoxCell *CScoreView::getClickedBox(CPoint pt)
{
	CBoxCell *pCl = 0;
	try{
	CPoint index = getCellIndex(pt);
	pCl = table.at(index.x).at(index.y);
	}catch(int){
	}catch(out_of_range){
	}
	return pCl;
	
}



void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(nowCell != 0){
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
	}
	nowCell = getClickedBox(point);
	
	if(nowCell != 0){
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
		displaychordInfo();
		
	}
	//CScrollView::OnLButtonDown(nFlags, point);
}


void CScoreView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	/*if(nowCell != 0){
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
	}
	nowCell = getClickedBox(point);
	if(nowCell != 0){
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
	}*/
	OnLButtonDown(nFlags, point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_SCORE);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON
			, GetCurrentMessage()->pt.x, GetCurrentMessage()->pt.y, this);

	CCellView::OnMButtonDown(nFlags, point);
}

#include "stdafx.h"
#include "MixOne.h"
#include "C:\Documents and Settings\�I���@��\My Documents\Visual Studio Projects\project_swan\MixOne\MixOneView.h"
//#include "MixOneView.h"

// CChView

IMPLEMENT_DYNCREATE(CChView, CCellView)

CChView::CChView()
{
	//CMainFrame *pMainFrame =( CMainFrame*)AfxGetApp()->m_pMainWnd;
}

CChView::~CChView()
{
	//delete pSynth;
}


BEGIN_MESSAGE_MAP(CChView, CCellView)
//	ON_COMMAND(ID_SYNTH, OnSynth)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CChView �`��

void CChView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, getChSize());
	for(int i = 0; i < 16; i++){
		chHeader.push_back(CChCell(i));
		//chHeader[i].setImage(&CScoreView::dibs[i]);
	}

	chHeader.resize(16);

}

void CChView::OnDraw(CDC* pDC)
{
	// TODO: �`��R�[�h�������ɒǉ����Ă�������	
	//CBrush newBrush(CCell::color[0]);
	
	//CGdiObject *pOld = pDC->SelectObject(&newBrush);

	pDC->SetBkMode(TRANSPARENT);

	TRIVERTEX        vert[3] ;	GRADIENT_TRIANGLE    gTri;
	vert [0] .x      = 1;	vert [0] .y      = 1;	vert [0] .Red    = 0xffff;
	vert [0] .Green  = 0xffff;	vert [0] .Blue   = 0xffff;	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = CCell::cellSize.cx-1;	vert [1] .y      = 0; 
	vert [1] .Red    = 0x0000;	vert [1] .Green  = 0x0000;	vert [1] .Blue   = 0x0000;
	vert [1] .Alpha  = 0x0000;

	vert [2] .x      = CCell::cellSize.cx-1;	vert [2] .y      = CCell::cellSize.cy-5;
	vert [2] .Red    = 0xffff;	vert [2] .Green  = 0xffff;	vert [2] .Blue   = 0xffff;
	vert [2] .Alpha  = 0x0000;

	gTri.Vertex1 = 0;	gTri.Vertex2 = 1;	gTri.Vertex3 = 2;

	int j;
	for(int i = 0; i < chHeader.size(); i++){
		pDC->Rectangle(&(chHeader[i]));
		pDC->GradientFill(vert, 3, &gTri, 1, GRADIENT_FILL_TRIANGLE);
		for(j = 0; j < 3; j++)
			vert[j].y += CCell::space + CCell::cellSize.cy;
		//vert[1].Blue += 0x22;

		chHeader[i].paint(pDC);
	}

	//pDC->SelectObject(pOld);
}


// CChView �f�f

#ifdef _DEBUG
void CChView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CChView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CChView ���b�Z�[�W �n���h��
// MixOneView.cpp : �����t�@�C��
//

CChCell *CChView::getClickedCh(CPoint pt)
{
	CChCell *pCl = 0;
	try{
	CPoint index = getCellIndex(pt);
	pCl = &chHeader.at(index.y);
	}catch(int){
	}catch(out_of_range){
	}
	return pCl;
	
}


// CBarView

IMPLEMENT_DYNCREATE(CBarView, CCellView)

CBarView::CBarView()
{
}

CBarView::~CBarView()
{
}


BEGIN_MESSAGE_MAP(CBarView, CCellView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CBarlView �`��

void CBarView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CScore *pDoc = GetDocument();

	SetScrollSizes(MM_TEXT, getBarSize());
	for(int i = 0; i < pDoc->getNumOfBar() + 1; i++)
		barHeader.push_back(CBarCell(i, 0));

	//startImage.CreateCompatibleBitmap(this->GetDC(), CCell::cellSize.cx, CCell::cellSize.cy);
	startImage.LoadBitmap(IDB_BITMAP_START);
	
}

CBarCell *CBarView::getClickedBar(CPoint pt)
{
	CBarCell *pCl = 0;
	try{
		CPoint index = getCellIndex(pt);
		pCl = &(barHeader.at(index.x));
	}catch(int){
	}catch(out_of_range){
	}
	
	return pCl;
	
}

void CBarView::OnDraw(CDC* pDC)
{
	CScore* pDoc = GetDocument();

	pDC->SetBkMode(TRANSPARENT);

	CRect re(barHeader[0].TopLeft(), (barHeader.end()-1)->BottomRight());
	pDC->Rectangle(&re);

	CPoint pt;
	CPoint dest;

	char buf[8];
	double dangerous;
	COLORREF oldCl = pDC->GetTextColor();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CGdiObject *pOld = memDC.SelectObject(&startImage);

	
	TRIVERTEX        vert[3] ;
	GRADIENT_TRIANGLE    gTri;
	vert [0] .x      = 1;	vert [0] .y      = 1;	vert [0] .Red    = 0xffff;
	vert [0] .Green  = 0xffff;	vert [0] .Blue   = 0xffff;	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = CCell::cellSize.cx-1;	vert [1] .y      = 0; 
	vert [1] .Red    = 0x0000;	vert [1] .Green  = 0x0000;	vert [1] .Blue   = 0x0000;
	vert [1] .Alpha  = 0x0000;

	vert [2] .x      = CCell::cellSize.cx-1;	vert [2] .y      = CCell::cellSize.cy-5;
	vert [2] .Red    = 0xffff;	vert [2] .Green  = 0xffff;	vert [2] .Blue   = 0xffff;
	vert [2] .Alpha  = 0x0000;

	gTri.Vertex1 = 0;	gTri.Vertex2 = 1;	gTri.Vertex3 = 2;

	int j;
	for(int i = 0; i < barHeader.size(); i++){
		pt.x = CCell::cellSize.cx * i;
		pt.y = 0;
		dest.x = pt.x;
		dest.y = pt.y + CCell::cellSize.cy ;
		pDC->MoveTo(pt);
		pDC->LineTo(dest);
		pDC->GradientFill(vert, 3, &gTri, 1, GRADIENT_FILL_TRIANGLE);
		for(j = 0; j < 3; j++)
			vert[j].x += CCell::cellSize.cx;
		if( i < pDoc->getNumOfBar()){
			dangerous = pDoc->getNumOfEvents(i +1);
			dangerous /= (SCORE_PLAYER_BUFSIZE / 12);
			dangerous *= 100;
			
			if(dangerous > 80)
				pDC->SetTextColor(RGB(255, 0, 0));
			
		}else
			dangerous = 0;
		sprintf(buf, "%.01f%%", dangerous);
		pDC->DrawText(buf, &(barHeader[i]), DT_LEFT);
		pDC->SetTextColor(oldCl);
		if(&(barHeader[i]) == nowCell){
			pDC->TransparentBlt(nowCell->left, nowCell->top, 
				CCell::cellSize.cx, CCell::cellSize.cy, &memDC,0, 0,
				CCell::cellSize.cx, CCell::cellSize.cy, RGB(0xff, 0xff, 0xff));

		}
	}

	memDC.SelectObject(pOld);
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// CBarlView �f�f

//#ifdef _DEBUG
/*void CBarView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBarView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
*/
void CBarView::makeNewBar(int i)
{
	barHeader.push_back(CBarCell(i, 0));
	InvalidateRect(&(barHeader.at(i)));
}

void CBarView::invalidateCell(int i)
{
	CRect re = barHeader[i];
	re += org;

	InvalidateRect(&re);	
}
//#endif //_DEBUG

//void CChView::OnSynth()
//{/*
//	if(pSynth->GetSafeHwnd() == 0)
//		pSynth->Create();
//	pSynth->ShowWindow(SW_SHOW);*/
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

void CChView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	
	//pSynth->ShowWindow(SW_SHOW);
	CCellView::OnLButtonDblClk(nFlags, point);
}

void CChView::OnSetFocus(CWnd* pOldWnd)
{
	CCellView::OnSetFocus(pOldWnd);

	/*if(pSynth->GetSafeHwnd() != 0){
		pSynth->ShowWindow(SW_HIDE);
	}*/


	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

// MixOneView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MixOne.h"
#include "MixOneView.h"


// CTitleView

IMPLEMENT_DYNCREATE(CTitleView, CCellView)

CTitleView::CTitleView()
{
}

CTitleView::~CTitleView()
{
}


BEGIN_MESSAGE_MAP(CTitleView, CCellView)
//	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CTitleView �`��

void CTitleView::OnInitialUpdate()
{
	CCellView::OnInitialUpdate();

	CSize sizeTotal;
	//TODO: ���̃r���[�̃T�C�Y�̍��v���v�Z���܂��B
	sizeTotal.cx = sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CTitleView::OnDraw(CDC* pDC)
{
	CFont font;
	font.CreatePointFont(120, "Times");
	CGdiObject *pOld = pDC->SelectObject(&font);
	CScore* pDoc = GetDocument();
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(1, 1, pDoc->getName()); 
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
	pDC->SelectObject(pOld);
}


// CTitleView �f�f

#ifdef _DEBUG
void CTitleView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTitleView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CTitleView ���b�Z�[�W �n���h��

//void CScoreView::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
//
//	if(nowCell != 0){
//		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
//		InvalidateRect(&marginRect);
//	}
//	nowCell = getClickedBox(point);
//	if(nowCell != 0){
//		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
//		InvalidateRect(&marginRect);
//	}
//	CMenu menu;
//	menu.LoadMenu(IDR_MENU_SCORE);
//	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON
//			, GetCurrentMessage()->pt.x, GetCurrentMessage()->pt.y, this);
//}

void CScoreView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	nowCell = getClickedBox(point);
	if(nowCell != 0)
		OnPhrasePaste();

	//CCellView::OnLButtonDblClk(nFlags, point);
}



void CScoreView::OnPhraseCopy()
{
	
	nowPhrase = &(((CBoxCell*)nowCell)->getBox()->getPhrase());
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CScoreView::OnUpdatePhraseCopy(CCmdUI *pCmdUI)
{
	
	pCmdUI->Enable(havePhrase());
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CScoreView::OnPhraseCut()
{
	OnPhraseCopy();
	((CBoxCell*)nowCell)->deleteBox();
	CScore *pDoc = GetDocument();
	pDoc->deletePhrase(nowCell->getI() + 1, nowCell->getJ() + 1);
	CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));

	pBView->invalidateCell(nowCell->getI());

	InvalidateRect(&marginRect);


	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CScoreView::OnUpdatePhraseCut(CCmdUI *pCmdUI)
{

	OnUpdatePhraseCopy(pCmdUI);
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CScoreView::OnPhrasePaste()
{
	CScore *pDoc = GetDocument();
	int nowBar = pDoc->getNumOfBar();
	if(nowPhrase != 0){
		pDoc->pastePhrase(nowCell->getI() + 1, nowCell->getJ() + 1, nowPhrase);
		CBoxCell *pB = (CBoxCell*)nowCell;
		pB->setBox(&(pDoc->operator ()(nowCell->getI() + 1, nowCell->getJ() + 1)));
		//�C���[�W�̓t���[�Y��ID�Ō��߂�
		nowCell->setImage(&dibs[nowPhrase->getID() % NUM_IMAGE]);
		pBView->invalidateCell(nowCell->getI());
		displaychordInfo();
	}
	if(nowBar != pDoc->getNumOfBar()){//�V�����o�[���ł����ꍇ
		table.push_back(vector<CBoxCell *>());
		for(int j = 0; j < 17; j++){
			table.at(nowCell->getI() + 1).push_back(new CBoxCell(nowCell->getI() + 1, j));
		}
		pBView->makeNewBar(nowCell->getI() + 1 );
		SetScrollSizes(MM_TEXT, getScoreSize());
	
		CRect re(table[nowCell->getI() + 1][0]->TopLeft(), table[nowCell->getI() + 1][15]->BottomRight());
		re.left -= 1;//�����]�T����������
		InvalidateRect(&re);
		
	}

	CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));

	InvalidateRect(&marginRect);

	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CScoreView::OnUpdatePhrasePaste(CCmdUI *pCmdUI)
{
	
	if((nowCell == 0) || (nowPhrase == 0))
		pCmdUI->Enable(FALSE);
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}



void CBarView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if(nowCell != 0){
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
	}

	CCell *temp = getClickedBar(point);
	if(temp == nowCell)
		nowCell = 0;
	else
		nowCell = temp;
	CMainFrame *pMain =( CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	if(nowCell != 0){
		pMain->sPlayer.setStartBar((GetDocument()->getNumOfBar() < nowCell->getI() + 1) ?
		0 : nowCell->getI() + 1);
		CRect marginRect(relCoordinate(nowCell->TopLeft()), relCoordinate(nowCell->BottomRight()));
		InvalidateRect(&marginRect);
	}else
		pMain->sPlayer.setStartBar(0);

	//CCellView::OnLButtonDown(nFlags, point);
}

void CScoreView::OnChordinfo()
{


	chordInfoDlg.ShowWindow(SW_SHOW);
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}



// nowCell���t���[�Y�������Ă��邩
bool CScoreView::havePhrase(void)
{
	if(nowCell == 0)
		return false;
	if(((CBoxCell*)nowCell)->getBox() == 0)
		return false;
	if((&(((CBoxCell*)nowCell)->getBox()->getPhrase()) == 0))
		return false;

	return true;
}

void CScoreView::displaychordInfo(void)
{
	if(havePhrase()){
		CPhrase &p =  ((CBoxCell*)nowCell)->getBox()->getPhrase();
		chordInfoDlg.SetWindowText(p.getName());
		CEdit *pKeyEdit, *pTypeEdit;
		for(int i = 0; i < 8; i++){
			pKeyEdit = (CEdit*)chordInfoDlg.GetDlgItem(FRONT_KEY + i);
			pTypeEdit = (CEdit*)chordInfoDlg.GetDlgItem(FRONT_TYPE +i);
			struct Chord c =p.getChord(i);
			pKeyEdit->SetWindowText(ChordName::keys[c.key]);
			pTypeEdit->SetWindowText(ChordName::types[c.type]);
		}
	}

}
