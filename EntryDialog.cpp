// EntryDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MixOne.h"
#include "EntryDialog.h"
#include ".\entrydialog.h"
#include "MainFrm.h"


// CEntryDialog �_�C�A���O

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


// CEntryDialog ���b�Z�[�W �n���h��

void CEntryDialog::OnBnClickedButtonSelfile()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if( fileDialog.DoModal () == IDOK ){
      CString pathName = fileDialog.GetPathName();

	  midiFile.SetWindowText(pathName);
	  listenButton.EnableWindow();
	  entryButton.EnableWindow();
      
	}

}

void CEntryDialog::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//�_�~�[��OnOK�֐�
	//CDialog::OnOK();
}



void CEntryDialog::OnBnClickedOk()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CEntryDialog::OnBnClickedButtonPrelisten()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	/*CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();
	const CMPhrase_player &player = pMainFrame->getPlayer();
	*/
	MessageBox("�����@�\�͂܂�����Ă��Ȃ�");

}

void CEntryDialog::OnBnClickedButtonEntry()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData();
	if(name.IsEmpty()){
		MessageBox("�t���[�Y������͂��Ă�������");
		return;
	}
	if(genre.IsEmpty()){
		MessageBox("�W��������I�����Ă�������");
		return;
	}
	::SetCurrentDirectory(managePath);

	CFileFind finder;
			
	if(finder.FindFile(name + ".phr") != 0){
		MessageBox("���łɓ������O�̃t���[�Y�����݂��Ă��܂�");
		return;
	}
    
	fstream manageFile(CPhrase::manager, ios::in | ios::out);

	if(!manageFile){
		MessageBox("�}�l�[�W���[�t�@�C�������݂��Ă��܂���");
		return;
	}
	PhraseInfo  blank;
	CString mid;
	midiFile.GetWindowText(mid);

	if(!CPhrase::midi2phrase(mid, name + ".phr"
		, &(info.pro), &(info.msb), &(info.lsb), &(info.isJakki), &(info.isDrum))){
		MessageBox("MIDI�t�@�C�����s���ł�");
		return;
	}
	sprintf(info.name, "%s", name);
	
	//�R�[�h�����
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
    
	//�󂢂Ă��郌�R�[�h��T��
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

	manageFile.clear();//�t�@�C���̍Ō�ɒB���Ă��邩������Ȃ��̂�clear���Ă���
	manageFile.seekp((info.id - 1) * sizeof info);
	
	manageFile.write(reinterpret_cast<const char*>(&info), sizeof info);

	manageFile.close();
	EndDialog(DIDENTRY);//�t���[�Y��o�^�������Ƃ�`����

		
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
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}
