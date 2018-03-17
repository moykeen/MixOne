// SynthControlerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MixOne.h"
#include "SynthControlerDlg.h"
#include ".\synthcontrolerdlg.h"


// CSynthControlerDlg ダイアログ

IMPLEMENT_DYNAMIC(CSynthControlerDlg, CDialog)
CSynthControlerDlg::CSynthControlerDlg(CMplayer &m, CToneTable &t, CDrumTable &d /*,CView *pv*/,CWnd* pParent /*=NULL*/)
	: CDialog(CSynthControlerDlg::IDD, pParent)/*, pView(pv)*/, pWnd(pParent)
	, currentChannel(0)	, nowPan(0)
	, nowVolume(0)
	, nowReverb(0)
	, nowChorus(0)
	, nowFrequency(0)
	, nowResonance(0)
	, nowAttackTime(0)
	, nowDecayTime(0)
	, nowReleaseTime(0)
	, nowVibRate(0)
	, nowVibDepth(0)
	, nowVibDecay(0)
	, nowMute(FALSE)
	, nowSolo(FALSE)
	, nowInst(0)
	, mout(m), tones(t), drums(d), channels(0)
	, nowTempo(0)
{
}

CSynthControlerDlg::~CSynthControlerDlg()
{
}

BOOL CSynthControlerDlg::Create()
{
	return CDialog::Create(CSynthControlerDlg::IDD, pWnd/*, pView*/);
}

void CSynthControlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CHANNELS, tab);
	DDX_Control(pDX, IDC_SLIDER_PAN, sliderPan);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, sliderVolume);
	DDX_Control(pDX, IDC_SLIDER_REVERB, sliderReverb);
	DDX_Control(pDX, IDC_SLIDER_CHORUS, sliderChorus);
	DDX_Control(pDX, IDC_SLIDER_FREQUENCY, sliderFrequency);
	DDX_Control(pDX, IDC_SLIDER_RESONANCE, sliderResonance);
	DDX_Control(pDX, IDC_SLIDER_ATTACKTIME, sliderAttackTime);
	DDX_Control(pDX, IDC_SLIDER_DECYATIME, sliderDecayTime);
	DDX_Control(pDX, IDC_SLIDER_RELEASETIME, sliderReleaseTime);
	DDX_Control(pDX, IDC_SLIDER_VIBRATE, sliderVibRate);
	DDX_Control(pDX, IDC_SLIDER_VIBDEPTH, sliderVibDepth);
	DDX_Control(pDX, IDC_SLIDER_VIBDELAY, sliderVibDecay);
	DDX_Control(pDX, IDC_TREE_TONE, toneTree);
	DDX_Control(pDX, IDC_TREE_DRUM, drumTree);
	DDX_Text(pDX, IDC_EDIT_PAN, nowPan);
	DDV_MinMaxInt(pDX, nowPan, 0, 127);
	DDX_Text(pDX, IDC_EDIT_VOLUME, nowVolume);
	DDV_MinMaxInt(pDX, nowVolume, 0, 127);
	DDX_Text(pDX, IDC_EDIT_REVERB, nowReverb);
	DDV_MinMaxInt(pDX, nowReverb, 0, 127);
	DDX_Text(pDX, IDC_EDIT_CHORUS, nowChorus);
	DDV_MinMaxInt(pDX, nowChorus, 0, 127);
	DDX_Text(pDX, IDC_EDIT_FREQUENCY, nowFrequency);
	DDV_MinMaxInt(pDX, nowFrequency, 0, 127);
	DDX_Text(pDX, IDC_EDIT_RESONANCE, nowResonance);
	DDV_MinMaxInt(pDX, nowResonance, 0, 127);
	DDX_Text(pDX, IDC_EDIT_ATTACKTIME, nowAttackTime);
	DDV_MinMaxInt(pDX, nowAttackTime, 0, 127);
	DDX_Text(pDX, IDC_EDIT_DECAYTIME, nowDecayTime);
	DDV_MinMaxInt(pDX, nowDecayTime, 0, 127);
	DDX_Text(pDX, IDC_EDIT_RELEASETIME, nowReleaseTime);
	DDV_MinMaxInt(pDX, nowReleaseTime, 0, 127);
	DDX_Text(pDX, IDC_EDIT_VIBRATE, nowVibRate);
	DDV_MinMaxInt(pDX, nowVibRate, 0, 127);
	DDX_Text(pDX, IDC_EDIT_VIBDEPTH, nowVibDepth);
	DDV_MinMaxInt(pDX, nowVibDepth, 0, 127);
	DDX_Text(pDX, IDC_EDIT_VIBDECAY, nowVibDecay);
	DDV_MinMaxInt(pDX, nowVibDecay, 0, 127);
	DDX_Check(pDX, IDC_CHECK_MUTE, nowMute);
	DDX_Check(pDX, IDC_CHECK_SOLO, nowSolo);
	DDX_Text(pDX, IDC_EDIT_TEMPO, nowTempo);
	DDV_MinMaxInt(pDX, nowTempo, 40, 250);
	DDX_Control(pDX, IDC_SPIN_TEMPO, spinTempo);
}

BEGIN_MESSAGE_MAP(CSynthControlerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHANNELS, OnTcnSelchangeTabChannels)
	ON_WM_HSCROLL()
	
	ON_BN_CLICKED(IDC_CHECK_MUTE, OnBnClickedCheckMute)
//	ON_LBN_SELCHANGE(IDC_LIST_INST, OnLbnDbclicked)
//	ON_WM_DESTROY()
//ON_WM_DESTROY()
ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TONE, OnTvnSelchangedTreeInst)
ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DRUM, OnTvnSelchangedTreeInst2)
//ON_EN_CHANGE(IDC_EDIT_TEMPO, OnEnChangeEditTempo)
//ON_EN_UPDATE(IDC_EDIT_TEMPO, OnEnUpdateEditTempo)
ON_BN_CLICKED(IDC_BUTTON_TEMPO, OnBnClickedButtonTempo)
END_MESSAGE_MAP()


BOOL CSynthControlerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	//// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	CString strAboutMenu;
	//	strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	////  Framework は、この設定を自動的に行います。
	//SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	//SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	
	

	//タブを16個作成する
	char buf[8];
	for(int i = 0; i < 16; i++){
		sprintf(buf, "%d", i);
		//itoa(i + 1, buf, 10);
		tab.InsertItem(i, buf);
	}

	//各スライダの設定
	sliderPan.SetRange(0, 127);
	sliderVolume.SetRange(0, 127);
	sliderReverb.SetRange(0, 127);
	sliderChorus.SetRange(0, 127);
	sliderFrequency.SetRange(0, 127);
	sliderResonance.SetRange(0, 127);
	sliderAttackTime.SetRange(0, 127);
	sliderDecayTime.SetRange(0, 127);
	sliderReleaseTime.SetRange(0, 127);
	sliderVibRate.SetRange(0, 127);
	sliderVibDepth.SetRange(0, 127);
	sliderVibDecay.SetRange(0, 127);

	if(channels == 0)
		MessageBox("");
	//ASSERT(channels != 0);

	//テンポ設定
	nowTempo = pSc->getTempo();
	spinTempo.SetRange(40, 250);
	spinTempo.SetPos(nowTempo);

	//チャネルのパラメータを表示する
	loadChannels(currentChannel);
	UpdateData(FALSE);
	setSliders();

	

	//音色ツリーをつくる
	makeTree();
	
	//MessageBox("");
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


// CSynthControlerDlg メッセージ ハンドラ


void CSynthControlerDlg::OnTcnSelchangeTabChannels(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	int nextCh = tab.GetCurSel();
	//ドラムチャネルなら、必要のないコントロールを使用不可にする
	if((*channels)[nextCh]->IsKindOf(RUNTIME_CLASS(CDrumChannel))){
		sliderPan.EnableWindow(FALSE);
		drumTree.ShowWindow(SW_SHOW);
		drumTree.EnableWindow();
		toneTree.ShowWindow(SW_HIDE);
		toneTree.EnableWindow(FALSE);

	
	}

	saveChannels();
	loadChannels(nextCh);

	//今まで表示していたチャネルがドラムチャネルならコントロールを使用可能にする
	if((*channels)[currentChannel]->IsKindOf(RUNTIME_CLASS(CDrumChannel))){
		sliderPan.EnableWindow();
		drumTree.ShowWindow(SW_HIDE);
		drumTree.EnableWindow(FALSE);
		toneTree.ShowWindow(SW_SHOW);
		toneTree.EnableWindow();

	
	}

	currentChannel = nextCh;
	UpdateData(FALSE);
	setSliders();

	////音色を選択する
	if((*channels)[currentChannel]->IsKindOf(RUNTIME_CLASS(CDrumChannel)))
		drumTree.Select(drumItems[currentChannel], TVGN_FIRSTVISIBLE);
	else
		toneTree.Select(toneItems[nowInst], TVGN_CARET);

	

	
	*pResult = 0;
}

// 現在のチャネルの状態を保存する
bool CSynthControlerDlg::saveChannels(void)
{
	CChannel *pch = (*channels)[currentChannel];

	if(pch->IsKindOf(RUNTIME_CLASS(CNormalChannel))){
		CNormalChannel *pNch = (CNormalChannel*)pch;
		pNch->pan = nowPan;
		pNch->frequency = nowFrequency;
		pNch->resonance = nowResonance;
		pNch->attack_time = nowAttackTime;
		pNch->decay_time = nowDecayTime;
		pNch->release_time = nowReleaseTime;
		pNch->vib_rate = nowVibRate;
		pNch->vib_depth = nowVibDepth;
		pNch->vib_time = nowVibDecay;
	}

	pch->mute = nowMute;
	pch->instNumber = nowInst;
	pch->volume = nowVolume;
	pch->reverb = nowReverb;
	pch->chorus = nowChorus;


	return true;
}

// 新しく表示するチャネルの状態を読み込む
bool CSynthControlerDlg::loadChannels(int nextChannel)
{
	CChannel *pch = (*channels)[nextChannel];

	if(pch->IsKindOf(RUNTIME_CLASS(CNormalChannel))){
		CNormalChannel *pNch = (CNormalChannel*)pch;
		nowPan = pNch->pan;
		nowFrequency = pNch->frequency;
		nowResonance = pNch->resonance ;
		nowAttackTime = pNch->attack_time;
		nowDecayTime = pNch->decay_time;
		nowReleaseTime = pNch->release_time;

		nowVibRate = pNch->vib_rate ;
		nowVibDepth = pNch->vib_depth ;
		nowVibDecay = pNch->vib_time ;
	}
	
	nowMute = pch->mute;
	nowInst = pch->instNumber;
	nowVolume = pch->volume ;
	nowReverb = pch->reverb ;
	nowChorus = pch->chorus ;


	return true;
}


// スライダの位置を設定する
bool CSynthControlerDlg::setSliders(void)
{
	sliderPan.SetPos(nowPan);
	sliderVolume.SetPos(nowVolume);
	sliderReverb.SetPos(nowReverb);
	sliderChorus.SetPos(nowChorus);
	sliderFrequency.SetPos(nowFrequency);
	sliderResonance.SetPos(nowResonance);
	sliderAttackTime.SetPos(nowAttackTime);
	sliderDecayTime.SetPos(nowDecayTime);
	sliderReleaseTime.SetPos(nowReleaseTime);
	sliderVibRate.SetPos(nowVibRate);
	sliderVibDepth.SetPos(nowVibDepth);
	sliderVibDecay.SetPos(nowVibDecay);

	return true;
}

void CSynthControlerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if((CSliderCtrl*)pScrollBar == &sliderPan){
		nowPan = sliderPan.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_PAN, nowPan);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderVolume){
		nowVolume = sliderVolume.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_VOLUME, nowVolume);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderReverb){
		nowReverb = sliderReverb.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_REVERB, nowReverb);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderChorus){
		nowChorus = sliderChorus.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_CHORUS, nowChorus);
		UpdateData(FALSE);
		return;
	}
	//NRPN_MSB = 1は以降共通なので。
	mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_MSB, 1);
	if((CSliderCtrl*)pScrollBar == &sliderFrequency){
		nowFrequency = sliderFrequency.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_FREQUENCY);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowFrequency);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderResonance){
		nowResonance = sliderResonance.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_RESONANCE);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowResonance);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderAttackTime){
		nowAttackTime = sliderAttackTime.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_ATTACK);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowAttackTime);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderDecayTime){
		nowDecayTime = sliderDecayTime.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_DECAY);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowDecayTime);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderReleaseTime){
		nowReleaseTime = sliderReleaseTime.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_RELEASE);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowReleaseTime);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderVibRate){
		nowVibRate = sliderVibRate.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_VIBRATE);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowVibRate);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderVibDepth){
		nowVibDepth = sliderVibDepth.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_VIBDEPTH);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowVibDepth);
		UpdateData(FALSE);
		return;
	}
	if((CSliderCtrl*)pScrollBar == &sliderVibDecay){
		nowVibDecay = sliderVibDecay.GetPos();
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_NRPN_LSB, MNT_VIBTIME);
		mout.shortMsg(MMT_CONTROL , currentChannel, MCT_DATAENTRY, nowVibDecay);
		UpdateData(FALSE);
		return;
	}

	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);//←基底クラスでは何にもやってないそうなので、コメントアウトTしておいた
}


bool CSynthControlerDlg::makeTree()
{
	
	int p = -1, m, l, i = 0;//i:通し番号
	TV_INSERTSTRUCT TreeCtrlItem;
	HTREEITEM hTreeItem;

	char nameBuf[25];

	numTones = tones.size();
	numDrums = drums.size();

	
	toneItems.reserve(numTones);
	drumItems.reserve(numDrums);
	
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;

  
	for(int i = 0; i < numTones; i++){
		TRACE("kita");
		sprintf(nameBuf, "%s", tones[i].name.GetString());
		if(tones[i].msb == 0){
			TreeCtrlItem.hParent = TVI_ROOT;
			TreeCtrlItem.item.lParam = -1;
			TreeCtrlItem.item.pszText = nameBuf;
			hTreeItem = toneTree.InsertItem(&TreeCtrlItem);
			TreeCtrlItem.hParent = hTreeItem;
		}
		TreeCtrlItem.item.lParam = i;
		TreeCtrlItem.item.pszText = nameBuf;
		toneItems.push_back(toneTree.InsertItem(&TreeCtrlItem));
	}

	
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.lParam = -1;
	TreeCtrlItem.item.pszText = "DRUMS";
	hTreeItem = drumTree.InsertItem(&TreeCtrlItem);
	TreeCtrlItem.hParent = hTreeItem;

	for(int i = 0; i < numDrums; i++){
		sprintf(nameBuf, "%s", drums[i].name.GetString());
		TreeCtrlItem.item.lParam = i;
		TreeCtrlItem.item.pszText = nameBuf;
		drumItems.push_back(drumTree.InsertItem(&TreeCtrlItem));
	}





	return true;

}

void CSynthControlerDlg::OnTvnSelchangedTreeInst(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。


	if(pNMTreeView->itemNew.lParam == -1)
		return;
	

	(*channels)[currentChannel]->instNumber = nowInst = pNMTreeView->itemNew.lParam;

	mout.shortMsg(MMT_CONTROL, currentChannel,MCT_BANK_MSB
		,(*channels)[currentChannel]->getInst().msb);//tones[nowInst].msb);
	mout.shortMsg(MMT_CONTROL, currentChannel,MCT_BANK_LSB
		, (*channels)[currentChannel]->getInst().lsb);//tones[nowInst].lsb);
	mout.shortMsg(MMT_PROGRAM, currentChannel
		, (*channels)[currentChannel]->getInst().program);//tones[nowInst].program);

	*pResult = 0;
}

void CSynthControlerDlg::OnTvnSelchangedTreeInst2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	(*channels)[currentChannel]->instNumber = nowInst = pNMTreeView->itemNew.lParam;

	mout.shortMsg(MMT_CONTROL, currentChannel,MCT_BANK_MSB
		,(*channels)[currentChannel]->getInst().msb);//tones[nowInst].msb);
	mout.shortMsg(MMT_CONTROL, currentChannel,MCT_BANK_LSB
		, (*channels)[currentChannel]->getInst().lsb);//tones[nowInst].lsb);
	mout.shortMsg(MMT_PROGRAM, currentChannel
		, (*channels)[currentChannel]->getInst().program);//tones[nowInst].program);


	*pResult = 0;
}
void CSynthControlerDlg::OnBnClickedCheckMute()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
}



void CSynthControlerDlg::OnBnClickedButtonTempo()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	pSc->setTempo(nowTempo);
	mout.tempo(nowTempo);

}

void CSynthControlerDlg::OnCancel()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	saveChannels();
	CDialog::OnCancel();
}
