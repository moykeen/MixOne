#pragma once
#include "Mout.h"
#include "Phrase.h"
#include "Score.h"
#include <map>
using namespace std;

struct ShortMIDIEVENT{//MIDIEVENT�\���̂�dwParam[1]�Ƃ����]���Ȃ��̂����邩��A������
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

class BMaker//�o�b�t�@�����̂ɕK�v�Ȋ֐������낢�뎝���Ă���
{
public:
	//�����f�B���}�[�W���ăo�b�t�@�����B�o�b�t�@�ɓ������f�[�^�̒�����Ԃ�
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
	bool withRythm;//���Y���Ƃ�������ɍĐ����邩�ǂ���

public:
	CPhrase* rythm;//���g���m�[���B
		//�t���[�Y�c���[���\�z���ꂽ��ɁA���g���m�[���t���[�Y�����Ă��炤


};



class CMScore_player : public CMinf_player
{
public:
	CMScore_player(int size);
	virtual ~CMScore_player();
	virtual void setData(void *);
	virtual int getNowBar(void){	return nowBar;	}
	void initChannel(CScore *);
	//���t���J�n����o�[���w�肷��B
	void setStartBar(int i = 0){	startBar = i;	}


protected:
	void fillBuf(void);
private:
	CScore *pScore;
	
	MelodyMap m;
	MelodyMap j;
	int nowBar;//���݁A���t�����Ă���o�[

	int startBar;//���t���J�n����o�[


public:
	CPhrase* tk;//�^�C���L�[�p�B
		//�t���[�Y�c���[���\�z���ꂽ��ɁA�^�C���L�[�p�t���[�Y�����Ă��炤
	

};

