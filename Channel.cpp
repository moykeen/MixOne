// Channel.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "MixOne.h"
#include "Channel.h"
#include <fstream>
#include "Mout.h"
#include ".\channel.h"
using namespace std;



//CToneTable
bool CToneTable::create(char *filename)
{
	if(created)
		return false;
	ifstream tonefile;
	tonefile.open(filename, ios::in);
	if(!tonefile)
		return false;

	int numTones, p = -1, m, l;
	char nameBuf[25];
	
	tonefile >> nameBuf >> l >> numTones;
	if(strcmp(nameBuf, "GS") != 0){
		tonefile.close();
		return false;
	}

	this->reserve(numTones);
	
	while(tonefile >> nameBuf >> m){
		if(m == 0)
			p++;
		this->push_back(Inst( p, m, l,CString(nameBuf)));
	}

	tonefile.close();
	created = true;
	return true;
}

//CDrumTable
bool CDrumTable::create(char *filename)
{
	if(created)
		return false;
	ifstream drumfile;
	drumfile.open(filename, ios::in);
	if(!drumfile)
		return false;

	int numDrums, p, m, l;
	char nameBuf[25];
	
	drumfile >> nameBuf >> m >> l >> numDrums;
	if(strcmp(nameBuf, "GS") != 0){
		drumfile.close();
		return false;
	}

	this->reserve(numDrums);
	
	while(drumfile >> nameBuf >> p){
		this->push_back(Inst(p, m, l, CString(nameBuf)));
	}

	drumfile.close();
	created = true;

	return true;
}

// CChannel

CChannel::CChannel(int n) : n_ch(n), instNumber(0), volume(DEF_VOLUME)
		, reverb(DEF_REVERB), chorus(DEF_CHORUS), mute(false)
{}

void CChannel::getChangedParam(vector<DWORD> &array)
{
	//if(instNumber != 0){
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_BANK_MSB, getInst().msb));
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_BANK_LSB, getInst().lsb));
		array.push_back(CMout::makeShortMsg(MMT_PROGRAM, n_ch, getInst().program ));
	//}
	//if(volume != DEF_VOLUME)
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_VOLUME, volume));
	//if(reverb != DEF_REVERB)
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_REVERB, reverb));
	//if(chorus != DEF_CHORUS)
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_CHORUS, chorus));


}

//CNormalChannel
IMPLEMENT_DYNAMIC(CNormalChannel, CChannel)

CNormalChannel::CNormalChannel(int n , CToneTable *pt) : CChannel(n), pan(DEF_PAN), frequency(DEF_NRPN), resonance(DEF_NRPN)
		, attack_time(DEF_NRPN), decay_time(DEF_NRPN), release_time(DEF_NRPN)
		, vib_rate(DEF_NRPN), vib_depth(DEF_NRPN), vib_time(DEF_NRPN), pToneTable(pt)
{}
 void CNormalChannel::getChangedParam(vector<DWORD> &array)
 {
	 CChannel::getChangedParam(array);

	 //if(volume != DEF_PAN)
		array.push_back(CMout::makeShortMsg(MMT_CONTROL, n_ch, MCT_PAN, pan));

	 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_MSB, 1));
	 //if(frequency != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_FREQUENCY));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, frequency));
	 //}
	 //if(resonance != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_RESONANCE));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, resonance));
	 //}
	 //if(attack_time != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_ATTACK));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, attack_time));
	 //}
	 //if(decay_time != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_DECAY));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, decay_time));
	 //}
	 //if(release_time != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_RELEASE));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, release_time));
	 //}
	 //if(vib_rate != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_VIBRATE));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, vib_rate));
	 //}
	 //if(vib_depth != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_VIBDEPTH));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, vib_depth));
	 //}
	 //if(vib_time != DEF_NRPN){
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_NRPN_LSB, MNT_VIBTIME));
		 array.push_back(CMout::makeShortMsg(MMT_CONTROL , n_ch, MCT_DATAENTRY, vib_time));
	 //}
 }


//CDrumChannel
IMPLEMENT_DYNAMIC(CDrumChannel, CChannel)

CDrumChannel::CDrumChannel(int n, CDrumTable *pd) : CChannel(n), pDrumTable(pd)
{}



void CChannel::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << n_ch << instNumber << volume << reverb << chorus;
	}
	else
	{	// loading code
		ar >> n_ch >> instNumber >> volume >> reverb >> chorus;
	}
}

void CNormalChannel::Serialize(CArchive& ar)
{
	CChannel::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar << pan << frequency << resonance << attack_time << decay_time << release_time
			<< vib_rate << vib_depth << vib_time;
	}
	else
	{	// loading code
		ar >> pan >> frequency >> resonance >> attack_time >> decay_time >> release_time
			>> vib_rate >> vib_depth >> vib_time;
	}
}

