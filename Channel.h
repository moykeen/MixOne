#pragma once

#include <vector>
using namespace std;


struct Inst{

	Inst( byte pro, byte m, byte l,const CString &n = CString()) :name(n), program(pro), msb(m), lsb(l){}
	
	inline bool operator==( const Inst &y)
	{	return (this->program == y.program) && (this->msb == y.msb);	}
	bool operator<(const Inst &y)
	{	
		if(this->program < y.program)
			return true;
		if(this->program > y.program)
			return false;
		return this->msb < y.msb;
	}
	inline bool operator>(const Inst &y)
	{	return !((*this) < y);	}

	CString name;
	byte program;
	byte msb;
	byte lsb;
};





class CToneTable : public vector<Inst>{
public:
	CToneTable() : created(false){}
	CToneTable(char *file){	create(file);	}
	virtual ~CToneTable(){}
	bool create(char *tonefile);
private:
	bool created;
};

class CDrumTable : public vector<Inst>{
public:
	CDrumTable() : created(false){}
	CDrumTable(char *file){	create(file);	}
	virtual ~CDrumTable(){}
	bool create(char *drumfile);
private:
	bool created;
};

//------各パラメタの規定値------
#define DEF_PAN 64
#define DEF_VOLUME 100
#define DEF_REVERB 40
#define DEF_CHORUS 0
#define DEF_NRPN 64 //frequency からvib_timeまで、規定値は全部６４


class CChannel : public CObject{//普通のチャネルとドラムチャネルに共通する属性をもつ
public:
	CChannel(int n);
	virtual ~CChannel(){}
	virtual const Inst &getInst(void) = 0;
	//virtual bool isChanged(void);
	virtual void getChangedParam(vector<DWORD> &) ;//
public:
	//効率を考えて、パラメタはpublicにする
	int n_ch;
	int instNumber;
	int volume;
	int reverb;
	int chorus;
	bool mute;

	virtual void Serialize(CArchive& ar);
};


class CNormalChannel : public CChannel{
	DECLARE_DYNAMIC(CNormalChannel)
public:
	CNormalChannel(int n, CToneTable *pt);
		
	virtual ~CNormalChannel(){}
	virtual const Inst &getInst(void){ return (*pToneTable)[instNumber];}
	//virtual bool isChanged(void);
	virtual void getChangedParam(vector<DWORD> &) ;//
public:
	int pan;
	int frequency;
	int resonance;
	int attack_time;
	int decay_time;
	int release_time;

	int vib_rate;
	int vib_depth;
	int vib_time;

private:
	CToneTable *pToneTable;
public:
	virtual void Serialize(CArchive& ar);
};

class CDrumChannel : public CChannel{
	DECLARE_DYNAMIC(CDrumChannel)
public:
	CDrumChannel(int n, CDrumTable *pd);
	virtual ~CDrumChannel(){}
	virtual const Inst &getInst(void){ return (*pDrumTable)[instNumber]; }
private:
	CDrumTable *pDrumTable;

};


