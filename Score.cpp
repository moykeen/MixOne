// Score.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MixOne.h"
#include "Score.h"
#include "MainFrm.h"


// CScore

IMPLEMENT_DYNCREATE(CScore, CDocument)

//CToneTable CScore::tones("sc8850.ton");
//CDrumTable CScore::drums("sc8850drum");


BOOL CScore::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	
	createChannels();
	setTempo(120);
	setName("a new score");

	//MessageBox(0, "", 0,0);

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



BEGIN_MESSAGE_MAP(CScore, CDocument)
END_MESSAGE_MAP()


// CScore �f�f

#ifdef _DEBUG
void CScore::AssertValid() const
{
	CDocument::AssertValid();
}

void CScore::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScore �V���A����

void CScore::Serialize(CArchive& ar)
{
	int i, j;
	Bar::iterator itr;
	CPhrase *p;

	if (ar.IsStoring())
	{
		ar << musicName << tempo;
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
		for(i = 0; i < num_channels; i++)
			channels[i]->Serialize(ar);

		//�X�R�A�̓��e��ۑ�
		ar << matrix.size();
		
		for(i = 0; i < matrix.size(); i++){
			ar << matrix[i + 1]->size();
			for(itr = matrix[i + 1]->begin(); itr != matrix[i + 1]->end(); itr++){
				p = &((itr->second)->getPhrase());

				ar << itr->first << p->getGenre() << p->getCategory() << p->getID();
			}
		}
	}
	else
	{
		createChannels();
		ar >> musicName >> tempo;
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
		for(i = 0; i < num_channels; i++)
			channels[i]->Serialize(ar);
		//�`���l���̃p�����^�𑗂�
		CMainFrame *pMainFrame =( CMainFrame*)AfxGetApp()->m_pMainWnd;
		pMainFrame->sPlayer.initChannel(this);

		int matrixSize;
		int barSize;
		int ch, id;
		CString gen;
		CString cate;
		CPhraseTree &refTree = pMainFrame->getPhraseTree();
		CPhrase *pP;

		ar >> matrixSize;
		for(i = 0; i < matrixSize; i++){
			ar >> barSize;
			for(j = 0 ; j < barSize ; j++){
				ar >> ch >> gen >> cate >> id;
				if((pP = refTree.getPhrase(gen, cate, id)) == 0)
					pP = pMainFrame->sPlayer.tk;
				pastePhrase(i + 1, ch, pP);
			}
		}

	}
	
}


// CScore �R�}���h

const int CScore::num_channels = 16;


CScore::CScore() 
{
}


void CScore::createChannels()
{
	channels.reserve(16);
	for(int i = 0; i < 9; i++)
		channels.push_back(new CNormalChannel(i, &CMainFrame::tones));
	channels.push_back(new CDrumChannel(9, &CMainFrame::drums));
	for(int i = 10; i < 16; i++)
		channels.push_back(new CNormalChannel(i, &CMainFrame::tones));
}

CScore::~CScore()
{
	Matrix::const_iterator itr1;
	Bar::const_iterator itr2;

	for(itr1 = matrix.begin(); itr1 != matrix.end(); itr1++){
		for(itr2 = itr1->second->begin(); itr2 != itr1->second->end(); itr2++){
			delete itr2->second;
		}
		delete itr1->second;
	}

	//�`���l���̔j��
	for(int i = 0; i < 16; i++){
		delete channels[i];
	}


}


// CScore �����o�֐�
int CScore::haveBox(int bar, int ch)
{
	if(matrix.count(bar) == 0)
		return NOBAR;
	if(matrix[bar]->count(ch) == 0)
		return NOCH;

	return EXIST;
}
	

bool CScore::createBox(int bar, int ch)
{
	int having = haveBox(bar, ch);

	if(having == EXIST)
		return false;

	if(having == NOBAR)
		matrix.insert(Matrix::value_type(bar, new Bar()));

	matrix[bar]->insert(Bar::value_type(ch, new CBox()));

	return true;
}

bool CScore::destroyBox(int bar, int ch)
{
    if(haveBox(bar, ch) != EXIST)
		return false;

	Bar *p =  (matrix[bar]);
	
	delete (p->operator [](ch));
	p->erase(ch);
	return true;
}

int CScore::getNumOfEvents(int n_bar)
{
	//char b[120];

	int sum = 0;
	Bar *pBar = matrix[n_bar];
	Bar::const_iterator itr;
	for(itr = pBar->begin(); itr != pBar->end(); itr++){
		//if((itr->second) == 0)
		//	MessageBox(0, "", 0,0);
		//else if(&((*itr->second).getPhrase()) == 0)
		//	9;//MessageBox(0, "", 0,0);
		/*else*/
			sum += (*(itr->second)).getPhrase().getEvents().size();
		
	}

	return sum;

}

bool CScore::newScore()
{
	return true;
}

bool CScore::midiFileOut(CString fileName)
{
	return true;
}

