#pragma once

#include <XAudio2.h>

class VoiceCallBack : public IXAudio2VoiceCallback
{
public:
    HANDLE hBufferEndEvent;

	bool endOfStream;

    VoiceCallBack(): hBufferEndEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) )
	{
		endOfStream = true;	
	}
    ~VoiceCallBack(){ CloseHandle( hBufferEndEvent ); }

    //Called when the voice has just finished playing a contiguous audio stream.
	STDMETHOD_(void, OnStreamEnd)()
	{
		endOfStream = true;
	}

    //Unused methods are stubs
    STDMETHOD_(void, OnVoiceProcessingPassEnd)() { }
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 SamplesRequired) {    }
    STDMETHOD_(void, OnBufferEnd)(void * pBufferContext)    { }
    STDMETHOD_(void, OnBufferStart)(void * pBufferContext) {    }
    STDMETHOD_(void, OnLoopEnd)(void * pBufferContext) {    }
    STDMETHOD_(void, OnVoiceError)(void * pBufferContext, HRESULT Error) { }	

};