#include "StdAfx.h"
#include "soundStart.h"


HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );
            break;

        default:
            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
            return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;
        
        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    
    return S_OK;
    
}

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}

void createSound2(IXAudio2* pXA, IXAudio2SourceVoice** pSrcVoice, LPCWSTR fileName, 
		XAUDIO2_BUFFER* pBuffer, WAVEFORMATEXTENSIBLE* pWfx)
{
	WAVEFORMATEXTENSIBLE tempW = {0};
	XAUDIO2_BUFFER tempB = {0};

	*pWfx = tempW;
	*pBuffer = tempB;
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	DWORD filetype;

	// Open the file
	HANDLE hFile = CreateFile(fileName, GENERIC_READ,
    FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile,fourccRIFF,dwChunkSize, dwChunkPosition );
	ReadChunkData(hFile,&filetype,sizeof(DWORD),dwChunkPosition);
	FindChunk(hFile,fourccFMT, dwChunkSize, dwChunkPosition );
	ReadChunkData(hFile, pWfx, dwChunkSize, dwChunkPosition );

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile,fourccDATA,dwChunkSize, dwChunkPosition );
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	pBuffer->AudioBytes = dwChunkSize;  //buffer containing audio data
	pBuffer->pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	pBuffer->Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		
	/*pXA->CreateSourceVoice( pSrcVoice, (WAVEFORMATEX*)&wfx,
				  0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL );*/
	//(*pSrcVoice)->SubmitSourceBuffer( buffer );
	//(*pSrcVoice)->Start( 0, XAUDIO2_COMMIT_NOW );
}

