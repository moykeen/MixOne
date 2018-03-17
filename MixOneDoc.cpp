// MixOneDoc.cpp : CMixOneDoc クラスの実装
//

#include "stdafx.h"
#include "MixOne.h"

#include "MixOneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMixOneDoc

IMPLEMENT_DYNCREATE(CMixOneDoc, CDocument)

BEGIN_MESSAGE_MAP(CMixOneDoc, CDocument)
END_MESSAGE_MAP()


// CMixOneDoc コンストラクション/デストラクション

CMixOneDoc::CMixOneDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CMixOneDoc::~CMixOneDoc()
{
}

BOOL CMixOneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}




// CMixOneDoc シリアル化

void CMixOneDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}


// CMixOneDoc 診断

#ifdef _DEBUG
void CMixOneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMixOneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMixOneDoc コマンド
