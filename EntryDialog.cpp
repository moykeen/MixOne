// EntryDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MixOne.h"
#include "EntryDialog.h"
#include ".\entrydialog.h"
#include "MainFrm.h"


// CEntryDialog ダイアログ

IMPLEMENT_DYNAMIC(CEntryDialog, CDialog)
CEntryDialog::CEntryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntryDialog::IDD, pParent)
	, name(_T(""))

	, isJakki(FALSE)
	
	,fileDialog(TRUE)
	, genre(_T(""))
	
{
}

CEntryDialog::~CEntryDialog()
{
}

void CEntryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PHRASENAME, name);
	DDV_MaxChars(pDX, name, 31);
	//DDX_Text(pDX, IDC_EDIT_CHORD, chord);
	DDX_Check(pDX, IDC_CHECK_JAKKI, isJakki);
	//DDX_CBString(pDX, IDC_COMBO_KEY, key);
	//DDX_CBString(pDX, IDC_COMBO_TYPE, type);
	DDX_Control(pDX, IDC_BUTTON_PRELISTEN, listenButton);
	DDX_Control(pDX, IDC_BUTTON_ENTRY, entryButton);
	DDX_Control(pDX, IDC_EDIT_FILENAME, midiFile);
	DDX_Text(pDX, IDC_EDIT_GENCATE, genre);
}


BEGIN_MESSAGE_MAP(CEntryDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, OnBnClickedButtonSelfile)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PRELISTEN, OnBnClickedButtonPrelisten)
	ON_BN_CLICKED(IDC_BUTTON_ENTRY, OnBnClickedButtonEntry)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CEntryDialog メッセージ ハンドラ

void CEntryDialog::OnBnClickedButtonSelfile()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if( fileDialog.DoModal () == IDOK ){
      CString pathName = fileDialog.GetPathName();

	  midiFile.SetWindowText(pathName);
	  listenButton.EnableWindow();
	  entryButton.EnableWindow();
      
	}

}

void CEntryDialog::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//ダミーのOnOK関数
	//CDialog::OnOK();
}



void CEntryDialog::OnBnClickedOk()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CDialog::OnOK();
}

BOOL CEntryDialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	CComboBox *pKeyCombo;
	CComboBox *pTypeCombo;
	for(int i = 0; i < 8; i++){
		pKeyCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_KEY + i);
		pTypeCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_TYPE + i);
		pKeyCombo->SetCurSel( i % 2 );
		pTypeCombo->SetCurSel( i % 2);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CEntryDialog::OnBnClickedButtonPrelisten()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	/*CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();
	const CMPhrase_player &player = pMainFrame->getPlayer();
	*/
	MessageBox("試聴機能はまだ作っていない");

}

void CEntryDialog::OnBnClickedButtonEntry()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	if(name.IsEmpty()){
		MessageBox("フレーズ名を入力してください");
		return;
	}
	if(genre.IsEmpty()){
		MessageBox("ジャンルを選択してください");
		return;
	}
	::SetCurrentDirectory(managePath);

	CFileFind finder;
			
	if(finder.FindFile(name + ".phr") != 0){
		MessageBox("すでに同じ名前のフレーズが存在しています");
		return;
	}
    
	fstream manageFile(CPhrase::manager, ios::in | ios::out);

	if(!manageFile){
		MessageBox("マネージャーファイルが存在していません");
		return;
	}
	PhraseInfo  blank;
	CString mid;
	midiFile.GetWindowText(mid);

	if(!CPhrase::midi2phrase(mid, name + ".phr"
		, &(info.pro), &(info.msb), &(info.lsb), &(info.isJakki), &(info.isDrum))){
		MessageBox("MIDIファイルが不正です");
		return;
	}
	sprintf(info.name, "%s", name);
	
	//コード入れる
	CComboBox *pKeyCombo;
	CComboBox *pTypeCombo;
	for(int i = 0; i < 8; i++){
		pKeyCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_KEY + i);
		pTypeCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_TYPE + i);
		info.cArray[i].key = pKeyCombo->GetCurSel();
		info.cArray[i].type = pTypeCombo->GetCurSel();
	}
	/*for(int i = 0; i < 8; i++){
		info.cArray[i] = c;
	}*/
    
	//空いているレコードを探す
	manageFile.seekg(0);
	info.id = 1;
	while(true){
		blank.id = 0;
		manageFile.read(reinterpret_cast<char *>(&blank), sizeof blank);
		if(blank.id == 0){
			break;
		}
		info.id++;
	}

	manageFile.clear();//ファイルの最後に達しているかもしれないのでclearしておく
	manageFile.seekp((info.id - 1) * sizeof info);
	
	manageFile.write(reinterpret_cast<const char*>(&info), sizeof info);

	manageFile.close();
	EndDialog(DIDENTRY);//フレーズを登録したことを伝える

		
}

void CEntryDialog::OnBnClickedButtonDelete()
{

	CComboBox *pKeyCombo;
	CComboBox *pTypeCombo;
	for(int i = 0; i < 8; i++){
		pKeyCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_KEY + i);
		pTypeCombo = (CComboBox*)GetDlgItem(FRONT_COMBO_TYPE + i);
		pKeyCombo->SetCurSel( i % 2 );
		pTypeCombo->SetCurSel( i % 2);
	}
	name.Empty();
	UpdateData(FALSE);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
}
