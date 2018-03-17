#pragma once

#include<mmsystem.h>


//ショートメッセージのタイプを表す定数
#define MMT_NOTEOFF 0x80 //MMT・・・メッセージタイプの略
#define MMT_NOTEON 0x90 
#define MMT_CONTROL 0xB0 
#define MMT_PROGRAM 0xC0 
#define MMT_PITCH 0xE0

#define MCT_PAN 10  //MCT・・・各種コントロールの意味
#define MCT_VOLUME 7
#define MCT_EXPRESSION 11
#define MCT_REVERB 91
#define MCT_CHORUS 93
#define MCT_DELAY 94
#define MCT_MODULATION 1

#define MCT_BANK_MSB 0
#define MCT_BANK_LSB 32
#define MCT_NRPN_MSB 99
#define MCT_NRPN_LSB 98
#define MCT_RPN_MSB 101
#define MCT_RPN_LSB 100
#define MCT_DATAENTRY 6

#define MNT_VIBRATE  8//NRPNメッセージ
#define MNT_VIBDEPTH 9
#define MNT_VIBTIME 10
#define MNT_FREQUENCY 32
#define MNT_RESONANCE 33
#define MNT_ATTACK 99
#define MNT_DECAY 100
#define MNT_RELEASE 102



// CMout コマンド ターゲット

class CMout : public CObject
{
public:
	CMout();
	virtual ~CMout();
private:
	private:
	HMIDIOUT hmo;
	bool connected;//デバイスに繋がっているかどうか
protected:
	HMIDIOUT& getHmo(){	return hmo;	}//プレイヤクラスでhmoとhmsをリンクさせるため
	void setConnected(bool b){	connected = b; }
public:
	virtual MMRESULT open(int id = MIDIMAPPER);
	virtual MMRESULT close(void);
	virtual CString getDevName(void);
	virtual inline void shortMsg(int type, int channel, int first = 0, int second = 0);
	virtual inline void shortMsg(DWORD dw);
	virtual inline void longMsg();
	virtual inline void reset();


	static DWORD makeShortMsg(int type, int channel, int first = 0, int second = 0);
	bool isConnected(void)
	{	return connected;	}

	//システムに存在するMIDI出力デバイスの数を返す
	static int getNumDevs(void)
	{
		return midiOutGetNumDevs();
	}
	//エラーテキストを取得
	static CString getErrorText(MMRESULT);
};

class CMplayer : public CMout{
public:
	CMplayer() ;
	virtual ~CMplayer();
	virtual MMRESULT open(int id = MIDIMAPPER) = 0;
	virtual MMRESULT close(void);
	virtual void stop(void);
	virtual void play(void);
	virtual void setData(void *) = 0;
	
	int getStatus(void){	return status;	}
	MMRESULT tempo(int t = 120);//tempoの単位は[回/分]
protected:
	void ready(void);
	void endPlaying(void)//コールばっくから呼ばれる関数。
	{	status = UNPREPARED;
		midiStreamStop(hms);
	}

	MMRESULT timeDV(int t = 480);
	
	HMIDISTRM& getHms(){	return hms;	}
	
	enum{PAUSE, PLAY, UNPREPARED};
private:
	
	HMIDISTRM& hms;
	int status;
	
};

class CMfin_player : public CMplayer{//一定の長さしか再生できないプレイヤ
public:
	CMfin_player(int size);
	virtual ~CMfin_player();
	//オープンして、タイムディビジョンとテンポを設定。ストリームバっファの準備もする
	virtual MMRESULT open(int id = MIDIMAPPER);
	//ストリームバッファの接続をはずしてクローズ
	virtual MMRESULT close(void);
	
protected:
	void ready(void);
	virtual void fillBuf(void) = 0;
    static void CALLBACK midiProc(HMIDIOUT h, WORD uMsg, int dwUser, int dw1, int dw2);
	int getBufSize(){	return bufSize;	}
	byte *getBuf(){		return buf;	}
	MIDIHDR &getMh(){	return mh;	}
	
private:
	int bufSize;
	byte *buf;
	MIDIHDR mh;
};

class CMinf_player : public CMplayer{//任意の長さが再生できるプレイヤ
public:
	CMinf_player(int size);
	virtual ~CMinf_player();
	virtual MMRESULT open(int id = MIDIMAPPER);
	virtual MMRESULT close(void);
	virtual void stop(void);
		
protected:
	void ready(void);
	virtual void fillBuf(void) = 0;//コールバックからも呼ばれる
	int getBufSize(){	return bufSize;	}
	byte *getBuf(){		return buf[side];	}
	MIDIHDR &getMh(){	return mh[side];	}

	static void CALLBACK midiProc(HMIDIOUT h, WORD uMsg, int dwUser, int dw1, int dw2);
	void setAllEnqueued(bool b){	allEnqueued = b;	}
	bool isAllEnqueued(void){	return allEnqueued;	}
	int turnSide(){		side = !side;	return side;	}
private:
	int bufSize;
	int side;
	byte *buf[2];
	MIDIHDR mh[2];
	bool allEnqueued;//すべてのデータがストリームにエンキューされたか？
};