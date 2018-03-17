#pragma once
#include "afxcmn.h"
#include "Score.h"
#include "Mout.h"

#define WN_GOODBY WM_USER + 6


// CSynthControlerDlg �_�C�A���O

class CSynthControlerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSynthControlerDlg)

public:
	CSynthControlerDlg( CMplayer &, CToneTable &, CDrumTable &/*, CView * pv*/, CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSynthControlerDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SYNTHCONTROLERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	//�X�R�A�̃|�C���^�󂯎����channels��ݒ�
	void setChannels(CScore &ref){	channels = &(ref.getCh());	}
	void setChannels(vector<CChannel *> &ref){	channels = &ref;	}
	void setScore(CScore *ptr){		pSc = ptr;	}

	BOOL Create();

public:

	int currentChannel;

	CMplayer &mout;

	CToneTable &tones;
	CDrumTable &drums;
	vector<CChannel *> *channels;
	CScore *pSc;
	int numTones;
	int numDrums;

	CTabCtrl tab;
	CSliderCtrl sliderPan;
	CSliderCtrl sliderVolume;
	CSliderCtrl sliderReverb;
	CSliderCtrl sliderChorus;
	CSliderCtrl sliderFrequency;
	CSliderCtrl sliderResonance;
	CSliderCtrl sliderAttackTime;
	CSliderCtrl sliderDecayTime;
	CSliderCtrl sliderReleaseTime;
	CSliderCtrl sliderVibRate;
	CSliderCtrl sliderVibDepth;
	CSliderCtrl sliderVibDecay;

	CSpinButtonCtrl spinTempo;


	CTreeCtrl toneTree;
	CTreeCtrl drumTree;
	vector<HTREEITEM> toneItems;//�c���[�̃A�C�e����ۑ����Ă����B
	vector<HTREEITEM> drumItems;


	int nowPan;
	int nowVolume;
	int nowReverb;
	int nowChorus;
	int nowFrequency;
	int nowResonance;
	int nowAttackTime;
	int nowDecayTime;
	int nowReleaseTime;
	int nowVibRate;
	int nowVibDepth;
	int nowVibDecay;
	BOOL nowMute;
	BOOL nowSolo;

	int nowInst;
	int nowTempo;

	//CView *pView;
	CWnd *pWnd;

	
public:
	// ���݂̃`���l���̏�Ԃ�ۑ�����
	bool saveChannels(void);//�X�R�A���ۑ�����鎞�K���Ăяo���Ȃ���΂Ȃ�Ȃ����Ƃ��Ȃ�
protected:
	// �V�����\������`���l���̏�Ԃ�ǂݍ���
	bool loadChannels(int);
	// �X���C�_�̈ʒu��ݒ肷��
	bool setSliders(void);
	//�y��A�C�R�����擾
	//int getInstIcon(int i);
	//���F�c���[�̐���
	bool makeTree(void);
	

public:
	afx_msg void OnTcnSelchangeTabChannels(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckEfx();
	afx_msg void OnBnClickedCheckEq();
	afx_msg void OnBnClickedCheckMute();
//	afx_msg void OnLbnDbclicked();   

	//CBitmapButton buttonStart;
	
	afx_msg void OnTvnSelchangedTreeInst(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnTvnSelchangedTreeInst2(NMHDR *pNMHDR, LRESULT *pResult);
	
//	afx_msg void OnEnChangeEditTempo();
//	afx_msg void OnEnUpdateEditTempo();
	afx_msg void OnBnClickedButtonTempo();
	
protected:
	virtual void OnCancel();
};
