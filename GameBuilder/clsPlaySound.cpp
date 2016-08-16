#include "StdAfx.h"
#include "clsPlaySound.h"

#include "soundStart.h"

clsPlaySound::clsPlaySound(void)
{
}


clsPlaySound::~clsPlaySound(void)
{
}


void clsPlaySound::createSound(IXAudio2* pXA2, LPCWSTR sound)
{
	pSourceVoice = NULL;
	createSound2(pXA2, &pSourceVoice,sound, &buffer, &wfx);

	pVCB = new VoiceCallBack;
}


void clsPlaySound::playSound(IXAudio2* pXA2, LPCWSTR sound)
{
	if(!pSourceVoice)
	{
		createSound2(pXA2, &pSourceVoice,sound, &buffer, &wfx);
		pVCB = new VoiceCallBack;
	}

	if(pVCB->endOfStream)
	{
		pVCB->endOfStream = false;

		if(pSourceVoice != NULL)
		{
			pSourceVoice->DestroyVoice();
		}
	
		pXA2->CreateSourceVoice( &pSourceVoice, (WAVEFORMATEX*)&wfx,
              0, XAUDIO2_DEFAULT_FREQ_RATIO, pVCB, NULL, NULL );
		pSourceVoice->SubmitSourceBuffer( &buffer );

		pSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
	}
}

void clsPlaySound::StopSound(IXAudio2* pXA2)
{
	if(pSourceVoice)
	{
		pSourceVoice->Stop();
		pSourceVoice = NULL;
	}
}

void clsPlaySound::playBling(IXAudio2* pXA2)
{
		if(pSourceVoice != NULL)
		{
			pSourceVoice->DestroyVoice();
		}
	
		pXA2->CreateSourceVoice( &pSourceVoice, (WAVEFORMATEX*)&wfx,
              0, XAUDIO2_DEFAULT_FREQ_RATIO, pVCB, NULL, NULL );
		pSourceVoice->SubmitSourceBuffer( &buffer );

		pSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
}