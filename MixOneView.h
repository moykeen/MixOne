// MixOneView.h : CScoreView クラスのインターフェイス
//

#include "Score.h"
#include "..\..\DynamicLinkLibrarys\DIBclass\cdib.h"
#include "SynthControlerDlg.h"
#include "ChordInfoDlg.h"

#pragma once

#define FRONT_IMAGE 3000
#define NUM_IMAGE 16

class CCell : public CRect{
public:
	//CCell(){}
	CCell( CDib *p = 0) 
		: pDib(p){	 }
	virtual ~CCell(){}
	virtual void clicked(){	MessageBox(0, "CCell", 0, 0);};
	virtual void paint(CDC *pDC)
	{	
		if(pDib != 0)
			pDib->Draw(pDC, TopLeft(), cellSize);
	}
	void setImage(CDib *image){	pDib = image;	}
	void deleteImage(){	pDib = 0;	}
	virtual int getI() = 0;
	virtual int getJ() = 0;

protected:
	void setCellRect(int i, int j);

protected:
	CDib *pDib;

public:
	const static int space;
	const static CSize cellSize;
	const static COLORREF color[];
};

class CChCell : public CCell{
public:
	CChCell(){}
	CChCell( int jj , CChannel *pc = 0, CDib *pd = 0) 
		: j(jj), pCh(pc), CCell( pd){setCellRect(0 , j);}
	virtual ~CChCell(){}
	virtual void clicked(){	MessageBox(0, "CChCell", 0, 0); }
	virtual void paint(CDC *pDC);
	virtual int getI(){	return 0;	}
	virtual int getJ(){	return j;	}

private:
	CChannel *pCh;
public:
	int j;
};

class CBarCell : public CCell{
public:
	CBarCell(){}
	CBarCell(int ii, CDib *pd = 0) 
		: i(ii), CCell(pd){setCellRect(i, 0);}
	virtual ~CBarCell(){}
	virtual void clicked(){	MessageBox(0, "CBarCell", 0, 0); }
	virtual void paint(CDC *pDC){}
	virtual int getI(){	return i;	}
	virtual int getJ(){	return 0;	}

public:
	int i;
};

class CBoxCell : public CCell{
public:
	CBoxCell(){}
	CBoxCell(int ii, int jj) : i(ii), j(jj),pBox(0){setCellRect(i, j);}
	virtual ~CBoxCell(){}
	void setBox(CBox *p){	pBox = p;	}
	CBox *getBox(){	return pBox;	}
	void deleteBox(){	pBox = 0; pDib = 0;	}
	virtual void clicked(){	MessageBox(0, "CBoxCell", 0, 0);}
	virtual void paint(CDC *pDC);
	virtual int getI(){	return i;	}
	virtual int getJ(){	return j;	}

	
private:
	CBox *pBox;
public:
	int i;
	int j;
};

// CCellView ビュー
#pragma once

class CCellView : public CScrollView
{
	DECLARE_DYNCREATE(CCellView)

protected:
	CCellView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CCellView();

public:
	CScore* GetDocument() const
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScore)));
		return (CScore*) m_pDocument;	
	}
protected:
	//相対座標に変換
	CPoint relCoordinate(const CPoint &absPt)
	{
		return CPoint(absPt + org);

	}
	//絶対座標にへんかん
	CPoint absCoordinate(const CPoint &relPt)
	{
		return CPoint(relPt - org);

	}

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//セルのインデックス取得(int型の例外出す)
	CPoint getCellIndex(CPoint pt);
	

protected:
	CPoint org;//現在のビューポートの原点
	CCell *nowCell;//現在選択されているセル

private:
	//static CDib backImage;
	/*static bool isLoaded;
	static CBrush back;*/

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	virtual void OnInitialUpdate();     // 構築後 1 回目

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};


class CBarView;

class CScoreView : public CCellView{
protected: // シリアル化からのみ作成します。
	CScoreView();
	DECLARE_DYNCREATE(CScoreView)

// 属性
public:
	/*CScore* GetDocument() const
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScore)));
		return (CScore*) m_pDocument;	
	}*/
	//CMixOneDoc* GetDocument() const;

// 操作
public:

// オーバーライド
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CScoreView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSize getScoreSize(void)
	{	CScore *pDoc = GetDocument();
		return CSize((pDoc->getNumOfBar() + 1) * CCell::cellSize.cx
			, 16 * (CCell::cellSize.cy + CCell::space));
		//バーは、現在存在しているバーしか表示できないと困るので、＋1してある
	}

	// クリックされた場所にあるセルを返す。なければNULL
	CBoxCell *getClickedBox(CPoint pt);

	
	
// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

private:
	vector<vector<CBoxCell * > > table;

	CBarView *pBView;
	

	CChordInfoDlg chordInfoDlg;

	
	static bool loadedImage;
	static CPhrase *nowPhrase;//現在貼り付けられるべくして選択されているフレーズ
public:
	static CDib dibs[NUM_IMAGE];
public:
	static setNowPhrase(CPhrase *p){	nowPhrase = p;	}
	virtual void OnInitialUpdate();
	
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
protected:
//	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPhraseCopy();
	afx_msg void OnUpdatePhraseCopy(CCmdUI *pCmdUI);
	afx_msg void OnPhraseCut();
	afx_msg void OnUpdatePhraseCut(CCmdUI *pCmdUI);
	afx_msg void OnPhrasePaste();
	afx_msg void OnUpdatePhrasePaste(CCmdUI *pCmdUI);
	afx_msg void OnChordinfo();
protected:
	// nowCellがフレーズをもっているか
	bool havePhrase(void);
	void displaychordInfo(void);
};

#ifndef _DEBUG  // MixOneView.cpp のデバッグ バージョン
//inline CMixOneDoc* CScoreView::GetDocument() const
 //  { return reinterpret_cast<CMixOneDoc*>(m_pDocument); }
#endif

#pragma once



// CChView ビュー

class CChView : public CCellView
{
	DECLARE_DYNCREATE(CChView)

protected:
	CChView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CChView();

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	CSize getChSize(void)
	{
		return CSize(CCell::space + CCell::cellSize.cx, (CCell::cellSize.cy + CCell::space) * 16 );
	}
	CChCell *getClickedCh(CPoint pt);
protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	virtual void OnInitialUpdate();     // 構築後 1 回目

	DECLARE_MESSAGE_MAP()

private:
	vector<CChCell> chHeader;
	CDib chImage;

	/*CSynthControlerDlg *pSynth;*/
public:
//	afx_msg void OnSynth();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


#pragma once


// CChView ビュー

class CBarView : public CCellView
{
	DECLARE_DYNCREATE(CBarView)

protected:
	CBarView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CBarView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void makeNewBar(int i);
	void invalidateCell(int i);
protected:
	CSize getBarSize()
	{
		CScore *pDoc = GetDocument();
		return CSize((pDoc->getNumOfBar() + 1 )* CCell::cellSize.cx, CCell::space + CCell::cellSize.cy);
	}
	CBarCell *getClickedBar(CPoint pt);

protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	virtual void OnInitialUpdate();     // 構築後 1 回目

	DECLARE_MESSAGE_MAP()

private:
	vector<CBarCell> barHeader;
	CBitmap startImage;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};







#pragma once



// CTitleView ビュー

class CTitleView : public CCellView
{
	DECLARE_DYNCREATE(CTitleView)

protected:
	CTitleView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CTitleView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	virtual void OnInitialUpdate();     // 構築後 1 回目

	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


