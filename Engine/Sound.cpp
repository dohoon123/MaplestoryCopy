#include <Windows.h>
#include <assert.h>

#include <map>
#include <string>

#include <xaudio2.h> 
#pragma comment(lib, "xaudio2.lib")

#include "Resource.h"
#include "Sound.h"

#if not defined _DEBUG
#define MUST(Expression) (      (         (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

namespace Sound
{
    namespace
    {
        IXAudio2* XAudio2 = nullptr;

        IXAudio2MasteringVoice* MasterVoice = nullptr;

        WAVEFORMATEXTENSIBLE wfx = { 0 };
    }

    HRESULT FindAudio(HANDLE hAudio, DWORD fourcc, DWORD& dwAudioSize, DWORD& dwAudioDataPosition)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hAudio, 0, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());

        DWORD dwAudioType;
        DWORD dwAudioDataSize;
        DWORD dwRIFFDataSize = 0;
        DWORD dwFileType;
        DWORD bytesRead = 0;
        DWORD dwOffset = 0;

        while (hr == S_OK)
        {
            DWORD dwRead;
            if (0 == ReadFile(hAudio, &dwAudioType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());

            if (0 == ReadFile(hAudio, &dwAudioDataSize, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());

            if (dwAudioType == fourccRIFF)
            {
                dwRIFFDataSize = dwAudioDataSize;
                dwAudioDataSize = 4;
                if (0 == ReadFile(hAudio, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                    hr = HRESULT_FROM_WIN32(GetLastError());
            }
            else
            {
                if (INVALID_SET_FILE_POINTER == SetFilePointer(hAudio, dwAudioDataSize, NULL, FILE_CURRENT))
                    return HRESULT_FROM_WIN32(GetLastError());
            }

            dwOffset += sizeof(DWORD) * 2;

            if (dwAudioType == fourcc)
            {
                dwAudioSize = dwAudioDataSize;
                dwAudioDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwAudioDataSize;

            if (bytesRead >= dwRIFFDataSize) return S_FALSE;

        }
        return S_OK;
    }

    HRESULT ReadAudioData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());
        DWORD dwRead;
        if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    struct Handle final {
        IXAudio2SourceVoice* SourceVoice = nullptr;
        XAUDIO2_BUFFER       buffer = { 0 };
                        };

    std::map<std::string, Handle*> Storage;

    void Import(std::string const& file)
    {
        HANDLE hFile = CreateFile(
            file.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (INVALID_HANDLE_VALUE == hFile) return;

        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) return;

        DWORD dwAudioSize;
        DWORD dwAudioPosition;
        MUST(FindAudio(hFile, fourccRIFF, dwAudioSize, dwAudioPosition));
        DWORD filetype;
        MUST(ReadAudioData(hFile, &filetype, sizeof(DWORD), dwAudioPosition));
        if (filetype != fourccWAVE) return;

        MUST(FindAudio(hFile, fourccFMT, dwAudioSize, dwAudioPosition));
        MUST(ReadAudioData(hFile, &wfx, dwAudioSize, dwAudioPosition));

        MUST(FindAudio(hFile, fourccDATA, dwAudioSize, dwAudioPosition));
        BYTE* pDataBuffer = new BYTE[dwAudioSize];
        MUST(ReadAudioData(hFile, pDataBuffer, dwAudioSize, dwAudioPosition));

        Handle* handle = new Handle();
        handle->buffer.AudioBytes = dwAudioSize;
        handle->buffer.pAudioData = pDataBuffer;
        handle->buffer.Flags = XAUDIO2_END_OF_STREAM;

        MUST(XAudio2->CreateSourceVoice(&handle->SourceVoice, (WAVEFORMATEX*)&wfx));

        {
            size_t const x = file.find_first_of('/') + sizeof(char);
            size_t const y = file.find_last_of('.');

            Storage.try_emplace(file.substr(x, y - x), handle);
        }
    }



	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
				MUST(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));     
				MUST(XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)); //XAUDIO2... crash prevention
				MUST(XAudio2->CreateMasteringVoice(&MasterVoice));			 

                Resource::Import("Sound", Import);

				return;
			}
			case WM_DESTROY: {
                for (std::pair<std::string, Handle*> elem : Storage)
                    delete elem.second;

                XAudio2->Release();
                return; 
            }
		}
	}
    void Sound::Play()
    {
        Handle* & sound = Storage.at(Content);
        
        if (loop) sound->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        
        if (pause)
        {
            pause = false;
            MUST(sound->SourceVoice->Start());
            return;
        }

        MUST(sound->SourceVoice->Stop());
        MUST(sound->SourceVoice->FlushSourceBuffers());
        MUST(sound->SourceVoice->SubmitSourceBuffer(&sound->buffer));
        MUST(sound->SourceVoice->SetVolume(volume));
        MUST(sound->SourceVoice->Start());
    }

    void Sound::Stop()
    {
        Handle* & sound = Storage.at(Content);
        MUST(sound->SourceVoice->Stop());
        MUST(sound->SourceVoice->FlushSourceBuffers());
    };

    void Sound::Pause()
    {
        if (pause) return;

        Handle* & sound = Storage.at(Content);
        pause = true;
        MUST(sound->SourceVoice->Stop());
    }

    void EndPlay() {
        for (std::pair<std::string, Handle*> elem : Storage)
        {
            elem.second->SourceVoice->Stop();
            elem.second->SourceVoice->FlushSourceBuffers();//must flush after stop music
        }
    }
}