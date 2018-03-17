#pragma once


#include "Phrase.h"
#include <map>
#include "Channel.h"
using namespace std;


// CScore コマンド ターゲット
class CBox : public CObject//マスクラス
{
	friend class CMScore_player;
public: 
	CBox(CPhrase *p = 0) : pPhrase(p){}
	virtual ~CBox(){}
	virtual void setPhrase(CPhrase *p){	pPhrase = p;	}
	virtual CPhrase& getPhrase(){	return *pPhrase;	}


private:
	
	CPhrase *pPhrase;
};



typedef map<int, CBox *> Bar;
typedef map<int, Bar *> Matrix;



// CScore ドキュメント

class CMainFrame;

class CScore : public CDocument
{
	DECLARE_DYNCREATE(CScore)
	friend class CMScore_player;
private:

	CScore();
public:
	virtual ~CScore();
	
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされました。
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()


public:
	


	virtual void pastePhrase(int bar, int ch,CPhrase *p)
	{
		createBox(bar, ch);
		(*this)(bar, ch).setPhrase(p);
	}

	virtual void deletePhrase(int bar, int ch)
	{
		destroyBox(bar, ch);
	}

	virtual void movePhrase(int n_bar, int n_ch, int d_bar, int d_ch)
	{
		pastePhrase(d_bar, d_ch, &((*this)(n_bar, n_ch).getPhrase()));
		deletePhrase(n_bar, n_ch);
	}

	virtual void copyPhrase(int n_bar, int n_ch, int d_bar, int d_ch)
	{
		pastePhrase(d_bar, d_ch, &((*this)(n_bar, n_ch).getPhrase()));
	}
	
	virtual bool newScore();//すべてのマスを消去
	virtual bool midiFileOut(CString fileName);//MIDIファイルを出力
	virtual void setName(const CString &s){	musicName = s;	}
	virtual const CString& getName(){	return musicName;	}
	virtual void setTempo(int t){	tempo = t;	}
	virtual int getTempo(){	return tempo;	}

	int getNumOfCh(void){	return num_channels;	}
	int getNumOfBar(void){	return matrix.size();	}
	vector<CChannel *> &getCh(){	return channels;	}//for SynthContloer

	CBox &operator()(int bar, int ch)
	{	
		return *((matrix[bar])->operator[](ch));
	}

	int getNumOfEvents(int n_bar);
	
	bool existBox(int bar, int ch){	return (haveBox(bar, ch) == EXIST);	}

protected:
	void createChannels(void);
	
	int haveBox(int bar, int ch);//指定したマスがあるかないか
	
	bool createBox(int bar, int ch);//指定した所にますを生成。もともとマスがあれば何もしない
	bool destroyBox(int bar, int ch);//指定したところからますを消去。もともとなければ何もしない

private:
	enum{NOBAR, NOCH, EXIST};//それぞれ、「指定したバーはない」「バーはあるが指定したチャンネルがない」「マスがある」
	Matrix matrix; 
	CString musicName;
	vector<CChannel*> channels;
	int tempo;

	static const int num_channels;

public:
	//static CToneTable tones;
	//static CDrumTable drums;

	
};
