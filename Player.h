#pragma once
#include "Mout.h"
#include "Phrase.h"
#include "Score.h"
#include <map>
using namespace std;

struct ShortMIDIEVENT{//MIDIEVENT構造体はdwParam[1]という余分なものがあるから、つくった
	ShortMIDIEVENT(DWORD d, DWORD e) : dwDeltaTime(d), dwStreamID(0), dwEvent(e){}

	int dwDeltaTime;
	DWORD dwStreamID;
	DWORD dwEvent;
};

inline bool operator==(const ShortMIDIEVENT &x, const ShortMIDIEVENT &y)
{	return x.dwDeltaTime == y.dwDeltaTime;	}
inline bool operator<(const ShortMIDIEVENT &x, const ShortMIDIEVENT &y)
{	return x.dwDeltaTime < y.dwDeltaTime;	}
inline bool operator>(const ShortMIDIEVENT &x, const ShortMIDIEVENT &y)
{	return x.dwDeltaTime > y.dwDeltaTime;	}

typedef map<int, const Melody *> MelodyMap;

class BMaker//バッファを作るのに必要な関数をいろいろ持っている
{
public:
	//メロディをマージしてバッファを作る。バッファに入ったデータの長さを返す
	static int mergeMelody(const MelodyMap& meloMap, byte *buf);
	static int mergeMelody(const MelodyMap& meloMap	, const MelodyMap& jakkiMap, byte *buf);

protected:
	static int sub_merge(const MelodyMap &meloMap, byte * buf, int preSize = 0, bool isJakki = false);
	static void sub_toDT(int size, byte *buf);
};


class CMPhrase_player : public CMfin_player{
public:
	CMPhrase_player(int size);
	virtual ~CMPhrase_player();
	virtual void setData(void *);
	void setWithRythm(bool b){	withRythm = b;	}
protected:
	void fillBuf(void);

private:
	CPhrase *pPhrase;
	bool withRythm;//リズムといっしょに再生するかどうか

public:
	CPhrase* rythm;//メトロノーム。
		//フレーズツリーが構築された後に、メトロノームフレーズを入れてもらう


};



class CMScore_player : public CMinf_player
{
public:
	CMScore_player(int size);
	virtual ~CMScore_player();
	virtual void setData(void *);
	virtual int getNowBar(void){	return nowBar;	}
	void initChannel(CScore *);
	//演奏を開始するバーを指定する。
	void setStartBar(int i = 0){	startBar = i;	}


protected:
	void fillBuf(void);
private:
	CScore *pScore;
	
	MelodyMap m;
	MelodyMap j;
	int nowBar;//現在、演奏をしているバー

	int startBar;//演奏を開始するバー


public:
	CPhrase* tk;//タイムキーパ。
		//フレーズツリーが構築された後に、タイムキーパフレーズを入れてもらう
	

};

