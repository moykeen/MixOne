// MixOneView.h : CScoreView �N���X�̃C���^�[�t�F�C�X
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

// CCellView �r���[
#pragma once

class CCellView : public CScrollView
{
	DECLARE_DYNCREATE(CCellView)

protected:
	CCellView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~CCellView();

public:
	CScore* GetDocument() const
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScore)));
		return (CScore*) m_pDocument;	
	}
protected:
	//���΍��W�ɕϊ�
	CPoint relCoordinate(const CPoint &absPt)
	{
		return CPoint(absPt + org);

	}
	//��΍��W�ɂւ񂩂�
	CPoint absCoordinate(const CPoint &relPt)
	{
		return CPoint(relPt - org);

	}

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//�Z���̃C���f�b�N�X�擾(int�^�̗�O�o��)
	CPoint getCellIndex(CPoint pt);
	

protected:
	CPoint org;//���݂̃r���[�|�[�g�̌��_
	CCell *nowCell;//���ݑI������Ă���Z��

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
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();     // �\�z�� 1 ���

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};


class CBarView;

class CScoreView : public CCellView{
protected: // �V���A��������̂ݍ쐬���܂��B
	CScoreView();
	DECLARE_DYNCREATE(CScoreView)

// ����
public:
	/*CScore* GetDocument() const
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScore)));
		return (CScore*) m_pDocument;	
	}*/
	//CMixOneDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
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
		//�o�[�́A���ݑ��݂��Ă���o�[�����\���ł��Ȃ��ƍ���̂ŁA�{1���Ă���
	}

	// �N���b�N���ꂽ�ꏊ�ɂ���Z����Ԃ��B�Ȃ����NULL
	CBoxCell *getClickedBox(CPoint pt);

	
	
// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()

private:
	vector<vector<CBoxCell * > > table;

	CBarView *pBView;
	

	CChordInfoDlg chordInfoDlg;

	
	static bool loadedImage;
	static CPhrase *nowPhrase;//���ݓ\��t������ׂ����đI������Ă���t���[�Y
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
	// nowCell���t���[�Y�������Ă��邩
	bool havePhrase(void);
	void displaychordInfo(void);
};

#ifndef _DEBUG  // MixOneView.cpp �̃f�o�b�O �o�[�W����
//inline CMixOneDoc* CScoreView::GetDocument() const
 //  { return reinterpret_cast<CMixOneDoc*>(m_pDocument); }
#endif

#pragma once



// CChView �r���[

class CChView : public CCellView
{
	DECLARE_DYNCREATE(CChView)

protected:
	CChView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
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
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();     // �\�z�� 1 ���

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


// CChView �r���[

class CBarView : public CCellView
{
	DECLARE_DYNCREATE(CBarView)

protected:
	CBarView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
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
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();     // �\�z�� 1 ���

	DECLARE_MESSAGE_MAP()

private:
	vector<CBarCell> barHeader;
	CBitmap startImage;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};







#pragma once



// CTitleView �r���[

class CTitleView : public CCellView
{
	DECLARE_DYNCREATE(CTitleView)

protected:
	CTitleView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~CTitleView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();     // �\�z�� 1 ���

	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


