#pragma once

#include<mmsystem.h>


//�V���[�g���b�Z�[�W�̃^�C�v��\���萔
#define MMT_NOTEOFF 0x80 //MMT�E�E�E���b�Z�[�W�^�C�v�̗�
#define MMT_NOTEON 0x90 
#define MMT_CONTROL 0xB0 
#define MMT_PROGRAM 0xC0 
#define MMT_PITCH 0xE0

#define MCT_PAN 10  //MCT�E�E�E�e��R���g���[���̈Ӗ�
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

#define MNT_VIBRATE  8//NRPN���b�Z�[�W
#define MNT_VIBDEPTH 9
#define MNT_VIBTIME 10
#define MNT_FREQUENCY 32
#define MNT_RESONANCE 33
#define MNT_ATTACK 99
#define MNT_DECAY 100
#define MNT_RELEASE 102



// CMout �R�}���h �^�[�Q�b�g

class CMout : public CObject
{
public:
	CMout();
	virtual ~CMout();
private:
	private:
	HMIDIOUT hmo;
	bool connected;//�f�o�C�X�Ɍq�����Ă��邩�ǂ���
protected:
	HMIDIOUT& getHmo(){	return hmo;	}//�v���C���N���X��hmo��hms�������N�����邽��
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

	//�V�X�e���ɑ��݂���MIDI�o�̓f�o�C�X�̐���Ԃ�
	static int getNumDevs(void)
	{
		return midiOutGetNumDevs();
	}
	//�G���[�e�L�X�g���擾
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
	MMRESULT tempo(int t = 120);//tempo�̒P�ʂ�[��/��]
protected:
	void ready(void);
	void endPlaying(void)//�R�[���΂�������Ă΂��֐��B
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

class CMfin_player : public CMplayer{//���̒��������Đ��ł��Ȃ��v���C��
public:
	CMfin_player(int size);
	virtual ~CMfin_player();
	//�I�[�v�����āA�^�C���f�B�r�W�����ƃe���|��ݒ�B�X�g���[���o���t�@�̏���������
	virtual MMRESULT open(int id = MIDIMAPPER);
	//�X�g���[���o�b�t�@�̐ڑ����͂����ăN���[�Y
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

class CMinf_player : public CMplayer{//�C�ӂ̒������Đ��ł���v���C��
public:
	CMinf_player(int size);
	virtual ~CMinf_player();
	virtual MMRESULT open(int id = MIDIMAPPER);
	virtual MMRESULT close(void);
	virtual void stop(void);
		
protected:
	void ready(void);
	virtual void fillBuf(void) = 0;//�R�[���o�b�N������Ă΂��
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
	bool allEnqueued;//���ׂẴf�[�^���X�g���[���ɃG���L���[���ꂽ���H
};