#pragma once
#include<XAudio2.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


void initSound();

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
void createSound2(IXAudio2* pXA, IXAudio2SourceVoice** srcVoice, LPCWSTR fileName, 
		XAUDIO2_BUFFER* pBuffer, WAVEFORMATEXTENSIBLE* pWfx);