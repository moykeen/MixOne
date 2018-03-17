#pragma once

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "Channel.h"
using namespace std;

class CGenre;
class CCategory;
class CPhraseTree;
class CPhrase;

#define NOTE 0x900000
#define PITCH 0xE00000
#define CONTROL 0xB00000

struct Chord{//コード構造体
	short key;//key[8];
	short type;//type[8];
};
class ChordName{
public:
	static const char *keys[];
	static const char *types[];
};

enum ChordKey{UnKnownKey = 0,KeepPreviousKey,  C, Cs, D,Eb, E, F,Fs, G,Ab, A,Bb, B};//s---シャープ、b---フラット
enum ChordType{UnKnownType = 0,KeepPreviousType, M, m, aug, dim, sus4, seventh, m7, M7, mM7, seventh_5, m7_5
			, seventh_sus4, dim7, add9, madd9, sixth, m6, sixth9};


struct PhraseInfo{//フレーズの情報をもつ
	int id;
	char name[32];
	byte isDrum;
	byte isJakki;
	byte pro;
	byte msb;
	byte lsb;
	struct Chord cArray[8];
	DWORD reserved1;
	DWORD reserved2;
};

class CEvent{
public:
	CEvent(int t) : time(t) {}
	virtual ~CEvent(){}
	int getTime(){		return time;	}
protected:
	int time;//フレーズ内での絶対時間
};

class CNormalEvent : public CEvent{
public:
	CNormalEvent(int t, byte a[]);
	virtual ~CNormalEvent(){}
	byte getNote(){	return note;	}
	byte getVel(){	return velocity;	}
	int getType(){	return type;	}
private:
	byte note;
	byte velocity;
	int type;//NOTE or PITCH or CONTROL
};

typedef vector<CNormalEvent *> Melody;

class CTNode : public CObject{
	DECLARE_DYNAMIC(CTNode)
public:
	CTNode(){}
	CTNode(const CString &n) : name(n){}
	virtual ~CTNode(){}
	virtual bool entry(CTNode *){return false;}
	virtual bool remove(CTNode *){return false;}
	virtual CString getInfo(){ return name;}
	const CString &getName(){	return name;	}
	virtual const CString getPathName() = 0;
protected:
	CString name;
};

class CPhraseTree : public vector<CGenre * >, public CTNode{
	DECLARE_DYNAMIC(CPhraseTree)
public:
	CPhraseTree(){}
	CPhraseTree(const CString &n) : CTNode(n){}
	virtual bool create();
	virtual ~CPhraseTree();
	virtual const CString getPathName(){	return name;	}
	CPhrase *getPhrase(const CString &gen, const CString &cate, int phraseID);
	
private:
	//CString name;
	
};

class CGenre : public vector<CCategory *>, public CTNode{
	DECLARE_DYNAMIC(CGenre)
public:
	CGenre(){}
	CGenre(const CString &n, CPhraseTree *p) : CTNode(n), pParent(p){}
	virtual const CString getPathName()
	{	return pParent->getPathName() + name + "\\";	}
	virtual ~CGenre();
	virtual CString getInfo();
	//const CString &getName(void){	return name;	}
private:
	//CString name;
	CPhraseTree *pParent;
	
};

class CCategory : public vector<CPhrase *>,public CTNode{
	DECLARE_DYNAMIC(CCategory)
public:
	CCategory(){}
	CCategory(const CString &n, CGenre *p) : CTNode(n), pParent(p){}
	virtual ~CCategory();
	virtual const CString getPathName()
	{	return pParent->getPathName() + name + "\\";	}
	const CString &getGenre(){	return pParent->getName();	}
	const CString getGenAndCate(){	return pParent->getName() + "---" + name;	}
	virtual bool entry(CTNode *pTNode);
	virtual bool remove(CTNode *pTNode);
	virtual CString getInfo();
private:
	//CString name;
	CGenre *pParent;
	
};


class CPhrase : public CTNode{
	DECLARE_DYNAMIC(CPhrase)
public:
	CPhrase(){}
	//CPhrase(const CString& n, int cate);
	CPhrase(const PhraseInfo &info, CCategory *p/*int gen, int cate*/);
	virtual ~CPhrase();
	virtual const CString getName()
	{	return name;	}
	virtual const Melody& getEvents()
	{	if(!loaded)
			loadEvents();
		return melody;	
	}
	virtual const CString getPathName(){	return pParent->getPathName() + name + ".phr";	}
	const CString &getCategory(){	return pParent->getName();	}
	const CString &getGenre(){	return pParent->getGenre();	}
	virtual bool remove(CTNode*){	return pParent->remove(this);	}
	static const int getMaxTicks(){	return maxTicks;	}
	const int getTicks(){	return ticks;	}
	const int getRestTicks(){	return maxTicks - ticks;	}
	int getNumOfEvents(void){	return melody.size();	}
	bool melodyIsDrum(void){	return isDrum;	}
	int getID(void){	return id;	}
	virtual CString getInfo();
	Inst getDefInst(){	return Inst(pro, msb, lsb);	}
	const struct Chord &getChord(int i){	return defaultChord[i];	};
	
protected:
	virtual bool loadEvents();
	static DWORD readLongValue(void *);//ユーティリティー
	static WORD readShortValue(void *);//ユーティリティー
	static bool checkHeader(ifstream&);//SMFのヘッダをチェック
	static int skipTrackHeader(ifstream&);//トラックヘッダをスキップ。イベント数を返す
	bool loaded;//イベントがロードされているかどうか
private:
	
	CCategory *pParent;
	int id;
	int pro;
	int msb;
	int lsb;
	
	struct Chord defaultChord[8];
	bool isDrum;
	Melody melody;
	int ticks;//データの長さ。単位はtick。１フレーズは４小節と決めたので、
					//1920×4 = 7680でちょうど。
	const static int unnecessaryBytes;//トラックの先頭に位置する余分なバイト
	const static int maxTicks;
public:
	
	const static CString CPhrase::manager;
	//midiからフレーズファイルを作る。成功したらtrue、program,msb,lsb、isJakkiが入る
	static bool midi2phrase(const CString &midiFile,const CString &phraseFile
		, byte *pProgram, byte *pMsb, byte *pLsb, byte *pisJakki, byte *pisDrum);	
};

class CJakkiPhrase : public CPhrase{//弱起のあるフレーず
	DECLARE_DYNAMIC(CJakkiPhrase)
public:
	CJakkiPhrase(){}
	//CJakkiPhrase(const CString& n, int cate);
	CJakkiPhrase(const PhraseInfo &info, CCategory *p);
	virtual ~CJakkiPhrase();
	const Melody& getMainEvents(void)
	{	if(!loaded)
			loadEvents();
		if(!jakkiLoaded)
			loadJakki();
		return mainMelo;	
	}
	const Melody& getJakkiEvents(void)
	{	
		if(!loaded)
			loadEvents();
		if(!jakkiLoaded)
			loadJakki();	
		return jakkiMelo;
	}
	virtual CString getInfo();
protected:
	bool loadEvents();
	bool loadJakki(void);
private:
	bool jakkiLoaded;
	Melody mainMelo;
	Melody jakkiMelo;
	Melody::const_iterator zeroItr;
};



















