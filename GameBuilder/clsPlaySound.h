#pragma once

#include<XAudio2.h>

#include "VoiceCallBack.h"

class clsPlaySound
{
public:
	clsPlaySound(void);
	~clsPlaySound(void);

	void createSound(IXAudio2* pXA2,LPCWSTR sound);

	void playSound(IXAudio2* pXA2, LPCWSTR sound);
	void StopSound(IXAudio2* pXA2);
	void playBling(IXAudio2* pXA2); 

protected:
	XAUDIO2_BUFFER buffer;
	WAVEFORMATEXTENSIBLE wfx;
	IXAudio2SourceVoice* pSourceVoice;

	VoiceCallBack* pVCB;
};

