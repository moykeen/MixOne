// Phrase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MixOne.h"
#include "Phrase.h"
#include <iomanip>
#include "io.h"
#include "fcntl.h"


#define DEBUG

const char *ChordName::keys[] = {"？", "→", "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
const char *ChordName::types[] = {"？", "→", "M", "m", "aug", "dim", "sus4", "7", "m7", "M7", "mM7", "75", "m7_5", "7_sus4", "dim7", "add9", "madd9", "6", "m6", "69"};

//CNormalEvent---------------------------------------
CNormalEvent::CNormalEvent(int t, byte array[]) : CEvent(t)
{
	switch(array[0]){
		case 0x90: case 0x99://ドラムの場合。
			type = NOTE;
			break;
		case 0xE0:
			type = PITCH;
			break;
		case 0xB0:
			type = CONTROL;
			break;
		default:
			char buf[50];
			sprintf(buf, "異常なデータがありました.%x.%x.%x, 時間＝%d",array[0], array[1], array[2], time);
			CString err(buf);
			throw buf;//MessageBox(0, buf, 0,0);
			//exit(1);
	}
	note = array[1];
	velocity = array[2];
}
//CTNode-------------------------------------------------
IMPLEMENT_DYNAMIC(CTNode, CObject)


// CPhrase-----------------------------------------------
IMPLEMENT_DYNAMIC(CPhrase, CTNode)

const int CPhrase::unnecessaryBytes = 19;
const int CPhrase::maxTicks = 7680;
//const CString CPhrase::phrasesFolder = "C:\\Documents and Settings\\棚橋　誠\\My Documents\\Visual Studio Projects\\project_swan\\Phrases";
const CString CPhrase::manager = "phraseManager.mng";
//CStringArray CPhrase::categorys;

DWORD CPhrase::readLongValue(void *p)
{
	int i;
	BYTE buf[4];
	for (i = 0; i < 4; i++) {
		buf[i] = ((LPBYTE)p)[3 - i];
	}
	return *(DWORD *)buf;
}

WORD CPhrase::readShortValue(void *p)
{
	return (((WORD)*(LPBYTE)p) << 8) + *((LPBYTE)p + 1);
}

CPhrase::CPhrase(const PhraseInfo &info, CCategory *p) 
:  pParent(p), loaded(false)
, CTNode(CString(info.name)), id(info.id), isDrum((bool)info.isDrum), pro(info.pro)
, msb(info.msb), lsb(info.lsb)
{
	memcpy(defaultChord, info.cArray, sizeof(struct Chord) * 8);
}

CPhrase::~CPhrase()
{
	vector<CNormalEvent* >::iterator pv;
	for(pv = melody.begin(); pv != melody.end(); pv++){
		delete *pv;
	}
}

bool CPhrase::checkHeader(ifstream& infile)
{
	//ヘッダチャンクのチェック-------------------------------------------------
	
	char buf[4];

	infile.read(buf, 4);
	if(memcmp(buf, "MThd", 4) != 0){//"MThd"
		infile.close();
		return false;
	}
	infile.read(buf, 4);
	if(buf[3] != 6){//"ヘッダのバイト数"
		infile.close();
		cerr << buf << "err 0000006" << endl;
		return false;
	}
	infile.read(buf, 2);
	if(buf[1] != 0){//format
		infile.close();
		cerr << "err format" << endl;
		return false;
	}
	infile.read(buf, 2);
	if(buf[1] != 1){//トラックチャンク数
		infile.close();
		cerr << "err tracknum" << endl;
		return false;
	}
	infile.read(buf, 2);
	if((unsigned)readShortValue(buf) != 480){//⊿タイム
		infile.close();
		cerr << "err delta" << endl;
		return false;
	}
	//トラックチャンクのチェック----------------------------------
	infile.read(buf, 4);
	if(memcmp(buf, "MTrk", 4) != 0){//"MTrk"
		infile.close();
		cerr << "err MTrk" << endl;
		return false;
	}
	return true;
}
int CPhrase::skipTrackHeader(ifstream &infile)
{
	char buf[4];
	infile.read(buf, 4);
	unsigned number_of_bytes = (unsigned)readLongValue(buf);//以降のデータのバイト数
	//トラックの先頭には不要なメタイベントがあるので読み飛ばす。（「SMFの条件」を参照）
	infile.ignore(unnecessaryBytes);
	number_of_bytes -= unnecessaryBytes;
	return number_of_bytes;
}

bool CPhrase::loadEvents()
{
#ifdef DEBUG
	ofstream logfile(pParent->getPathName()+ "読み込みログ.log", ios::out);
#endif


	//ifstream infile(getPathName() ,ios::in); 
	//CFile in(getPathName(), CFile::modeRead );

	int fd = open(getPathName(), O_RDONLY | O_BINARY);
	if(fd == -1)
		exit(1);

	int fLen = _filelength(fd);
	BYTE *buf = new BYTE[fLen];
	if(read(fd, buf, fLen) < fLen)
		exit(1);
	close(fd);
	int number_of_bytes;
	int pos = 0;

	number_of_bytes = buf[0] | (buf[1] << 8);
	pos += 4;
#ifdef DEBUG
	logfile << hex;
	for(int i = 0; i < number_of_bytes; i++){
		logfile << dec << i << "  " << hex <<(int)buf[i] << endl;
		
	}
#endif

	//read(fd, &number_of_bytes, 4);
	//in.Read(&number_of_bytes, 4);
	//fread(&number_of_bytes, 4, 4, phr);
	//infile.read(reinterpret_cast<char *>(&number_of_bytes), 4);
#ifdef DEBUG
	logfile << dec << number_of_bytes << endl;
#endif
    melody.clear();//melodyの中身を空っぽにする
	ticks = -1920;

	BYTE b;
	BYTE array[3];
	unsigned dt;

	while(number_of_bytes > 4){//トラックの最後にはエンドオブトラック（3バイトがある）
								//＋直前の⊿タイム
		dt = 0;
		do{
			dt <<= 7;
			//in.Read(&b, 1);
			//MessageBox(0, "", 0, 0);
			//infile.read(reinterpret_cast<char *>(&b), 1);
			//read(fd, &b, 1);
			b = buf[pos++];
 			number_of_bytes--;
			dt |= (b & 0x7F);
		}while(b > 0x80);


		//read(fd, array, 3);
		for(int i = 0; i < 3; i++)
			array[i] = buf[pos++];

		number_of_bytes -= 3;

		/*if(array[0] == 0x0a){
			array[0] = array[1];
			array[1] = array[2];
			read(fd, &(array[2]), 1);
			number_of_bytes --;
		}*/
		//infile.read(reinterpret_cast<char *>(array), 3);
		
		ticks += dt;
#ifdef DEBUG
		logfile << dec << pos << "  " << ticks << " " << dt << ":" << hex <<(int)array[0] 
			<< "" << (int)array[1] << ""  << (int)array[2] << endl;
#endif
		try{
			melody.push_back(new CNormalEvent(ticks, (byte *)array));
		}catch(CString err){
			close(fd);
			delete[] buf;
			melody.clear();
			return false;
		}
		if(ticks > maxTicks){
			melody.clear();
			ticks = 0;
			MessageBox(0, "このふーレズは長すぎる", 0, 0);
			delete[] buf;
			return false;
		}
	}
	loaded = true;
	melody.resize(melody.size());
	
	delete[] buf;
	return true;
}

bool CPhrase::midi2phrase(const CString &midiFile, const CString &phraseFile
						  , byte *p, byte *m, byte *l, byte *isJakki, byte *isDrum)
{
	ifstream mid(midiFile, ios::in);
	//ofstream phr(phraseFile, ios::out);

	
	CFile phr(phraseFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	if(!mid /*|| !phr*/)
		return false;
	if(!checkHeader(mid))
		return false;

	int number_of_bytes = skipTrackHeader(mid);
	
	
	char *copyBuf;

	//インストゥルメントデータを読み込む
	mid.ignore(3);//deltatime=0とB0 00を読み飛ばす
	mid.read(reinterpret_cast<char *>(m), 1);

	mid.ignore(3);//deltatime=0とB0 20を読み飛ばす
	mid.read(reinterpret_cast<char *>(l), 1);

	mid.ignore(2);//deltatime=0とC0 を読み飛ばす
	mid.read(reinterpret_cast<char *>(p), 1);
	number_of_bytes -= 11;

	mid.close();
	//CFile midi(midiFile, CFile::modeRead | CFile::typeBinary);
	//midi.Seek(33 + CPhrase::unnecessaryBytes, CFile::begin);
	//FILE *midi;
	char buf[MAX_PATH];
	sprintf(buf, "%s", midiFile);
	FILE *midi = fopen(buf, "rb");
	fseek(midi, 33+ CPhrase::unnecessaryBytes, SEEK_SET);
	

	phr.Write(&number_of_bytes, 4);
	//phr.write(reinterpret_cast<const char *>(&number_of_bytes), 4);

	//弱起かどうか判定。判定するために読み込んだバイトはすぐに書き込む
	int firstDt = 0;
	byte b;
	do{
		firstDt <<= 7;
		//midi.Read(&b, 1);
		fread(&b, 1,1, midi);
		//mid.read(reinterpret_cast<char *>(&b), 1);
		phr.Write(&b, 1);
		//phr.write(reinterpret_cast<const char *>(&b), 1);
 		number_of_bytes--;
		firstDt |= (b & 0x7F);
	}while(b > 0x80);

	//mid.read(reinterpret_cast<char *>(&b), 1);
	//midi.Read(&b, 1);
	fread(&b, 1,1, midi);
	phr.Write(&b, 1);
	//phr.write(reinterpret_cast<const char *>(&b), 1);
	number_of_bytes--;

	*isDrum = (b == 0x99);//*isDrum = ((b | 0x0f) == 9);

	if(firstDt < 1920)
		*isJakki = 1;
	else
		*isJakki = 0;

	
	
	copyBuf = new char[number_of_bytes];
	//copyBuf = new char[midi.GetLength()];
	//mid.read(reinterpret_cast<char *>(co), number_of_bytes);
	//midi.Read(copyBuf,number_of_bytes);
	fread(copyBuf, 1,number_of_bytes, midi);
	phr.Write(copyBuf,number_of_bytes);
	//phr.write(reinterpret_cast<const char *>(copyBuf), number_of_bytes);
	delete[] copyBuf;

	fclose(midi);
	return true;
}

CString CPhrase::getInfo()
{
	CString s("フレーズ名:" + name);
	if(isDrum)
		return CString(s + "\r\nドラムフレーズ");
	return s;

}

//CJakkiPhrase-----------------------------------------

IMPLEMENT_DYNAMIC(CJakkiPhrase, CPhrase)

CJakkiPhrase::CJakkiPhrase(const PhraseInfo &info,CCategory *p/*int gen, int cate*/) 
: CPhrase(info, p), jakkiLoaded(false)
{
}

CJakkiPhrase::~CJakkiPhrase()
{
}

bool CJakkiPhrase::loadEvents()
{
	bool result = CPhrase::loadEvents();

	zeroItr = getEvents().begin();
	while((*zeroItr)->getTime() < 0)
		zeroItr++;

	zeroItr++;//zeroItrは絶対時間が始めて正になるイベントを指している

	

	return result;
}

bool CJakkiPhrase::loadJakki(void)
{

	copy(getEvents().begin(), zeroItr, back_inserter(jakkiMelo));
	copy(zeroItr, getEvents().end(), back_inserter(mainMelo));
	jakkiLoaded = true;
	return jakkiLoaded;
}
CString CJakkiPhrase::getInfo()
{
	return CPhrase::getInfo() + CString("\r\n食い込みあり");

}

//CCategory-----------------------------
IMPLEMENT_DYNAMIC(CCategory, CTNode)

CCategory::~CCategory()
{
	const_iterator itr;
	for(itr = begin(); itr != end(); itr++){
		delete (*itr);
	}
}

bool CCategory::entry(CTNode *pTNode)
{
	if(!pTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		return false;
	push_back((CPhrase*)pTNode);
	return true;

}

CString CCategory::getInfo()
{
	return CString("カテゴリ名:" + name);

}

bool CCategory::remove(CTNode *pTNode)
{
	if(!pTNode->IsKindOf(RUNTIME_CLASS(CPhrase)))
		return false;
	CPhrase *p = (CPhrase*)pTNode;
	CCategory::iterator location;
	location = find(begin(), end(), p);
	if(location == end())//見つからなかった場合
		return false;

	//マネージャファイルからレコードを削除
	PhraseInfo blank = {0};
	fstream manageFile(this->getPathName() + CPhrase::manager, ios::in | ios::out);
	manageFile.seekp((p->getID() - 1) * sizeof(PhraseInfo));
	manageFile.write(reinterpret_cast<const char *>(&blank), sizeof blank);
	//フレーズファイルを削除
	CFile::Remove(p->getPathName());


	delete *location;
	erase(location);
	return true;

}
//CGenre-------------------------------------------------
IMPLEMENT_DYNAMIC(CGenre, CTNode)

CGenre::~CGenre()
{
	const_iterator itr;
	for(itr = begin(); itr != end(); itr++){
		delete (*itr);
	}
}

CString CGenre::getInfo()
{
	return CString("ジャンル名:" + name);

}

//CPhraseTree-------------------------------------
IMPLEMENT_DYNAMIC(CPhraseTree, CTNode)
CPhraseTree::~CPhraseTree()
{
	const_iterator itr;
	for(itr = begin(); itr != end(); itr++){
		delete (*itr);
	}
}

bool CPhraseTree::create()
{
	if(!::SetCurrentDirectory(name))
		return false;

	CFileFind finder1, finder2;
	CString genre, cate;
	int g = 0, c = 0;
	CString kugiri = "\\";
	//ifstream infile;
	FILE *in;
	int  i = 0, x = 0;
	PhraseInfo info;

	CCategory *pc;
	CGenre *pg;
	
	BOOL bWorking1, bWorking2;
	bWorking1 = finder1.FindFile("*.*");
	while (bWorking1)
	{
		bWorking1 = finder1.FindNextFile();
		genre = finder1.GetFileName();

		if(genre[0] == '.')//"."と".."は飛ばす
			continue;

		::SetCurrentDirectory(genre);
		push_back(new CGenre(genre, this));
		
		bWorking2 = finder2.FindFile("*.*");

		while(bWorking2){
			bWorking2 = finder2.FindNextFile();
			cate = finder2.GetFileName();

			if(cate[0] == '.')//"."と".."は飛ばす
			continue;

			::SetCurrentDirectory(cate);
			pg = at(g);
			at(g)->push_back(new CCategory(cate, pg));

			//infile.clear();
			in = fopen(CPhrase::manager, "rb");
			//infile.open(CPhrase::manager);
			if(in == 0)
				continue;
			fread(&info, sizeof info, 1, in);
			//infile.read(reinterpret_cast<char *>(&info), sizeof info);
			while(!feof(in)){
				if(info.id != 0){
					pc = pg->at(c);
					if(info.isJakki == 0)
						at(g)->at(c)->push_back(new CPhrase(info, pc));
					else
						at(g)->at(c)->push_back(new CJakkiPhrase(info, pc));
				}
				fread(&info, sizeof info, 1, in);
				//infile.read(reinterpret_cast<char *>(&info), sizeof info);

			}
			fclose(in);
			//infile.close();
			::SetCurrentDirectory("..");
			c++;
		}
		::SetCurrentDirectory("..");
		g++;
		c = 0;
	}
	return true;
}

CPhrase *CPhraseTree::getPhrase(const CString &gen, const CString &cate, int phraseID)
{
	CPhraseTree::const_iterator itr1;
	CGenre::const_iterator itr2;
	CCategory::const_iterator itr3;
	for(itr1 = begin(); itr1 != end(); itr1++){
		if((*itr1)->getName() == gen)
			break;
	}
	if(itr1 == end())
		return 0;//at(0)->at(0)->at(0);

	for(itr2 = (*itr1)->begin(); itr2 != (*itr1)->end(); itr2++){
		if((*itr2)->getName() == cate)
			break;
	}
	if(itr2 == (*itr1)->end())
		return 0;//(*itr1)->at(0)->at(0);
	
	for(itr3 = (*itr2)->begin(); itr3 != (*itr2)->end(); itr3++){
		if((*itr3)->getID() == phraseID)
			break;
	}
	if(itr3 == (*itr2)->end())
		return 0;//(*itr2)->at(0);
	return (*itr3);
}
