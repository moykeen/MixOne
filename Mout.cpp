// Mout.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MixOne.h"
#include "Mout.h"


// CMout

CMout::CMout(): connected(false)
{
}

CMout::~CMout()
{
	if(!connected)
		close();
}


CString CMout::getErrorText(MMRESULT result)
{
	char buf[128];
	midiOutGetErrorText(result, buf, 127);
	return CString(buf);

}

MMRESULT CMout::open(int id)
{
	MMRESULT result = midiOutOpen(&hmo, id, 0, 0, 0);
	
	connected = (result == MMSYSERR_NOERROR);

	return result;
}

MMRESULT CMout::close(void)
{
	//reset();
	MMRESULT result = midiOutClose(hmo);
	
	connected = (result != MMSYSERR_NOERROR);

	return result;
		
}

CString CMout::getDevName(void)
{
	if(connected){
		MIDIOUTCAPS caps;

		midiOutGetDevCaps((UINT)hmo, &caps, sizeof(MIDIOUTCAPS));

		return CString(caps.szPname);
	}
	else
		return CString();
}

inline void CMout::shortMsg(int type, int channel, int first, int second)
{
	//midiOutShortMsg(hmo, type | (second << 16) | (first << 8) | channel );
	shortMsg(makeShortMsg(type, channel, first, second));
}

void CMout::shortMsg(DWORD dw)
{
	midiOutShortMsg(hmo, dw);
}

 DWORD CMout::makeShortMsg(int type, int channel, int first, int second )
{
	return DWORD(type | (second << 16) | (first << 8) | channel);
}



inline void CMout::longMsg()
{
}

inline void CMout::reset()
{
	if(connected)
		midiOutReset(hmo);
}

//CMplayerクラス
CMplayer::CMplayer() : CMout(), hms((HMIDISTRM&)getHmo())
	, status(UNPREPARED)
{}//hmoとhmsをリンクさせた

CMplayer::~CMplayer()
{
	if(!isConnected())
		close();
}

MMRESULT CMplayer::close(void)
{
	//reset();
	stop();
	MMRESULT result = midiStreamClose(hms);
	
	setConnected(result != MMSYSERR_NOERROR);

	return result;
		
}


void CMplayer::stop(void)
{
	switch(status){
		case PLAY: case PAUSE:
			status = UNPREPARED;
			midiStreamStop(hms);
			break;
	}
}

void CMplayer::play(void)
{
	switch(status){
		case PAUSE:
			status = PLAY;
			midiStreamRestart(hms);
			break;
		case PLAY:
			status = PAUSE;
			midiStreamPause(hms);
			break;
	}
}

void CMplayer::ready(void)
{
	switch(status){
		case UNPREPARED:
			status = PAUSE;
			break;
	}
}

MMRESULT CMplayer::timeDV(int t)
{
	MIDIPROPTIMEDIV mptd;
	
	mptd.cbStruct = sizeof(mptd);
	mptd.dwTimeDiv = t;
	return midiStreamProperty(hms, (LPBYTE)&mptd, MIDIPROP_SET|MIDIPROP_TIMEDIV);
}

MMRESULT CMplayer::tempo(int t)
{
	MIDIPROPTEMPO mpt;

    DWORD tempo = 60000000 / t; //単位を[マイクロ秒/回]に変換
	mpt.cbStruct = sizeof(mpt);
	mpt.dwTempo = tempo;
	return midiStreamProperty(hms, (LPBYTE)&mpt, MIDIPROP_SET|MIDIPROP_TEMPO);
}

//CMfin_player
CMfin_player::CMfin_player(int size) : CMplayer(), bufSize(size)
{
	buf = new byte[bufSize];
}

CMfin_player::~CMfin_player()
{
	close();
	delete[] buf;
}


void CALLBACK CMfin_player::midiProc(HMIDIOUT h, WORD uMsg, int dwUser, int dw1, int dw2)
{
	if(uMsg == MOM_DONE){
		CMfin_player *pThis = (CMfin_player*)dwUser;
		if(pThis->getStatus() == PLAY)
			pThis->endPlaying();
	}
}
MMRESULT CMfin_player::open(int id)
{
	HMIDISTRM& h = getHms(); 
	MMRESULT result 
		= midiStreamOpen(&h, (UINT*)&id, 1, (DWORD)CMfin_player::midiProc, (DWORD)this, CALLBACK_FUNCTION);

	mh.lpData = (char*)buf;
	mh.dwBufferLength = bufSize;
	mh.dwFlags = 0;
	midiOutPrepareHeader(getHmo(), &mh, sizeof(MIDIHDR));

	setConnected(result == MMSYSERR_NOERROR);
	timeDV();
	tempo();
	return result;

	
}

MMRESULT CMfin_player::close(void)
{
	midiOutUnprepareHeader(getHmo(), &mh, sizeof(MIDIHDR));
	return CMplayer::close();
}


void CMfin_player::ready(void)
{
	if(getStatus() == UNPREPARED){
		midiStreamOut(getHms(), &mh, sizeof(MIDIHDR));
	}
	CMplayer::ready();

}



//CMinf_player
void CALLBACK CMinf_player::midiProc(HMIDIOUT h, WORD uMsg, int dwUser, int dw1, int dw2)
{
	if(uMsg == MOM_DONE){
		
		CMinf_player *pThis = (CMinf_player*)dwUser;

		//pThis->shortMsg(MMT_CONTROL, 0, MCT_REVERB, 99);

		if(pThis->allEnqueued){
			//if(pThis->getStatus() == PLAY){
				pThis->endPlaying();	
				pThis->side = 0;
			//}
		}else{
			pThis->fillBuf();
			midiStreamOut(pThis->getHms(), &(pThis->mh[pThis->side]), sizeof(MIDIHDR));
			pThis->turnSide();
		}
	}

}
CMinf_player::CMinf_player(int size) : CMplayer(), bufSize(size), side(0), allEnqueued(false)
{
	buf[0] = new byte[bufSize];
	buf[1] = new byte[bufSize];
}

CMinf_player::~CMinf_player()
{
	delete[] buf[0];
	delete[] buf[1];
}

MMRESULT CMinf_player::open(int id)
{
	HMIDISTRM& h = getHms(); 
	MMRESULT result 
		= midiStreamOpen(&h, (UINT*)&id, 1, (DWORD)CMinf_player::midiProc, (DWORD)this, CALLBACK_FUNCTION);

	for(int i = 0; i < 2; i++){
		mh[i].lpData = (char*)buf[i];
		mh[i].dwBufferLength = bufSize;
		mh[i].dwFlags = 0;
		midiOutPrepareHeader(getHmo(), &mh[i], sizeof(MIDIHDR));
	}

	setConnected(result == MMSYSERR_NOERROR);
	timeDV();
	tempo();
	return result;

	
}

MMRESULT CMinf_player::close(void)
{
	for(int i = 0; i < 2; i++)
		midiOutUnprepareHeader(getHmo(), &mh[i], sizeof(MIDIHDR));

	return CMplayer::close();
}

void CMinf_player::ready(void)
{
	allEnqueued = false;
	side = 0;

	if(getStatus() == UNPREPARED){
		midiStreamOut(getHms(), &mh[0], sizeof(MIDIHDR));
		//midiStreamOut(getHms(), &mh[1], sizeof(MIDIHDR));
	}
	CMplayer::ready();
}

void CMinf_player::stop(void)
{
	side = 0;
	allEnqueued = true;
	CMplayer::stop();

}