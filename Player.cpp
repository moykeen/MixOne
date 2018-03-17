#include "StdAfx.h"
#include ".\player.h"
#include <algorithm>

//BMaker
int BMaker::sub_merge(const MelodyMap &meloMap, byte * b, int previousSize, bool isJakki)
{
	MelodyMap::const_iterator itr;
	int note, velocity;
	ShortMIDIEVENT *sPtr = (ShortMIDIEVENT *)b
			, *ePtr = ((ShortMIDIEVENT *)b) + previousSize;
	Melody::const_iterator pv;


	for(itr = meloMap.begin(); itr != meloMap.end(); itr++){
		for(pv = itr->second->begin(); pv != itr->second->end(); pv++){
			note = (*pv)->getNote(); velocity = (*pv)->getVel();
			*ePtr = ShortMIDIEVENT(
				isJakki ? (CPhrase::getMaxTicks() + (*pv)->getTime()) : ((*pv)->getTime())
				,(velocity << 16) | (note << 8) | ((*pv)->getType() >>16) | itr->first);
					
			ePtr++;
		}
		inplace_merge(sPtr, sPtr + previousSize, ePtr);//<=ePtr - 1 か？
		previousSize += itr->second->size();
	}

	return previousSize;


}

void BMaker::sub_toDT(int size, byte *b)
{
	ShortMIDIEVENT *sPtr = (ShortMIDIEVENT *)b;

	for(int i = size - 1; i > 0; i--)
		sPtr[i].dwDeltaTime -= sPtr[i-1].dwDeltaTime;
	if(sPtr[0].dwDeltaTime < 0)//フレーズプレイヤで弱起再生のため
		sPtr[0].dwDeltaTime = 0;

}

int BMaker::mergeMelody(const MelodyMap &meloMap, byte *b)
{
	int size = sub_merge(meloMap, b);
	sub_toDT(size, b);
	return size * 12;
}

int BMaker::mergeMelody(const MelodyMap &meloMap, const MelodyMap &jakkiMap, byte *b)
{
	int size = sub_merge(meloMap, b);
	size = sub_merge(jakkiMap, b, size, true);
	sub_toDT(size, b);
	return size * 12;

}

// CMPhrase_player

//CPhrase *CMPhrase_player::rythm = 0;

CMPhrase_player::CMPhrase_player(int size) 
: CMfin_player(size), pPhrase(0), withRythm(false)
{
}

CMPhrase_player::~CMPhrase_player()
{
}

void CMPhrase_player::setData(void *p)
{
	CPhrase *ptr = (CPhrase *)p;
	if(pPhrase != ptr){
		pPhrase = ptr;
		fillBuf();
	}

	CMfin_player::ready();

}

void CMPhrase_player::fillBuf(void)
{
	
	MelodyMap m;
	int ch;
	Inst inst = pPhrase->getDefInst();
	if(pPhrase->melodyIsDrum()){
		//MessageBox(0, "", 0, 0);
		ch = 9;

	}else{
		ch = 1;
		
		if(withRythm)
			m.insert(MelodyMap::value_type(9, &(rythm->getEvents())));
	}
	m.insert(MelodyMap::value_type(ch, &(pPhrase->getEvents())));
	shortMsg(MMT_CONTROL, ch,MCT_BANK_MSB, inst.msb);
	shortMsg(MMT_CONTROL, ch,MCT_BANK_LSB, inst.lsb);
	shortMsg(MMT_PROGRAM, ch, inst.program);

	if(getBufSize() < 12 * pPhrase->getNumOfEvents()){
		MessageBox(0, "プレイヤのバッファの大きさが足りないので再生できない", 0, 0);
		return;
	}
	getMh().dwBytesRecorded = BMaker::mergeMelody(m, getBuf());

}

// CMScore_player


//CPhrase *CMScore_player::tk = 0;

CMScore_player::CMScore_player(int size) 
	: CMinf_player(size), nowBar(0/*1*/), startBar(0), pScore(0)
{
}

CMScore_player::~CMScore_player()
{
}

void CMScore_player::setData(void *p)
{
	if((pScore != 0) && (pScore != (CScore *)p))//前回再生したスコアと異なるスコアを再生する場合
		setStartBar(0);
	pScore = (CScore*)p;

	nowBar = startBar;//0;

	getMh().dwBytesRecorded = 0;
	turnSide();
	
	CMinf_player::ready();

}

void CMScore_player::fillBuf(void)
{
	m.clear();
	j.clear();

	Bar *pB;
	Bar::const_iterator itr;

	CJakkiPhrase *jpPhrase;

	//まず、タイムキーパを入れる
	m.insert(MelodyMap::value_type(15, &(tk->getEvents())));
	//m.insert(MelodyMap::value_type(9, &(ry.getEvents())));

	if(nowBar != 0){//一番初めだけ最初のバーの弱起を入れる

		pB = pScore->matrix[nowBar];

		for(itr = pB->begin(); itr != pB->end(); itr++){
			if(itr->second->pPhrase->IsKindOf(RUNTIME_CLASS(CJakkiPhrase))){
				jpPhrase = (CJakkiPhrase *)itr->second->pPhrase;
				m.insert(MelodyMap::value_type(itr->first - 1
						, &(jpPhrase->getMainEvents())));
			}else{
				m.insert(MelodyMap::value_type(itr->first - 1
						, &(itr->second->pPhrase->getEvents())));
			}
		}
	}

	if(pScore->matrix.count(nowBar + 1) == 0){
		setAllEnqueued(true);
	}else{
		pB = pScore->matrix[nowBar + 1];

		for(itr = pB->begin(); itr != pB->end(); itr++){
			if(itr->second->pPhrase->IsKindOf(RUNTIME_CLASS(CJakkiPhrase))){
				jpPhrase = (CJakkiPhrase *)itr->second->pPhrase;
				j.insert(MelodyMap::value_type(itr->first - 1
								, &(jpPhrase->getJakkiEvents())));
			}
		}

		nowBar++;
	}


	getMh().dwBytesRecorded = BMaker::mergeMelody(m, j, getBuf());

	if(nowBar == 1){//一番初めだけ、先頭の⊿タイムを3小節分カットする。
		//nowBar++としているからnowBar　== 1としてある
		
		ShortMIDIEVENT *p = (ShortMIDIEVENT *)getBuf();
		p->dwDeltaTime -= 5760;
	}

}

void CMScore_player::initChannel(CScore *pSc)
{
	vector<CChannel *>::const_iterator itr = pSc->getCh().begin();

	//テンポ設定
	tempo(pSc->getTempo());
	vector<DWORD> paramArray;
	paramArray.reserve(30);

	int i;
	for(; itr != pSc->getCh().end(); itr++){
		(*itr)->getChangedParam(paramArray);
		for(i = 0; i < paramArray.size(); i++)
			shortMsg(paramArray[i]);
		paramArray.clear();
	}

}



