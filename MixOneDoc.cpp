// MixOneDoc.cpp : CMixOneDoc �N���X�̎���
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


// CMixOneDoc �R���X�g���N�V����/�f�X�g���N�V����

CMixOneDoc::CMixOneDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CMixOneDoc::~CMixOneDoc()
{
}

BOOL CMixOneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}




// CMixOneDoc �V���A����

void CMixOneDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}


// CMixOneDoc �f�f

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


// CMixOneDoc �R�}���h
