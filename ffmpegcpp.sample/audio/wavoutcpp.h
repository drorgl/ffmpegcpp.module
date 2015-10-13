

#ifndef _SAMPLE_WAVEOUT_
#define _SAMPLE_WAVEOUT_

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>

#define BLOCK_SIZE 8192
#define BLOCK_COUNT 20


class waveoutcpp
{
private:
	WAVEFORMATEX wfx;
	HWAVEOUT hWaveOut;

	volatile int waveFreeBlockCount;

	CRITICAL_SECTION waveCriticalSection;
	WAVEHDR* waveBlocks;
	int waveCurrentBlock;


	void waveOutProc(
		HWAVEOUT hWaveOut,
		UINT uMsg,
		DWORD dwInstance,
		DWORD dwParam1,
		DWORD dwParam2
		)
	{
		/*
		* pointer to free block counter
		*/
		waveoutcpp * inst = reinterpret_cast<waveoutcpp*>(dwInstance);
		
		/*
		* ignore calls that occur due to openining and closing the
		* device.
		*/
		if (uMsg != WOM_DONE)
			return;
		EnterCriticalSection(&waveCriticalSection);
		inst->waveFreeBlockCount++;
		LeaveCriticalSection(&waveCriticalSection);
	}

	static void CALLBACK waveOutProcWrapper(HWAVEOUT hWaveOut,
		UINT uMsg,
		DWORD dwInstance,
		DWORD dwParam1,
		DWORD dwParam2)
	{
		waveoutcpp * inst = reinterpret_cast<waveoutcpp*>(dwInstance);
		if (inst == NULL){
			throw std::runtime_error("no instance was passed!");
		}
		inst->waveOutProc(hWaveOut, uMsg, dwInstance, dwParam1, dwParam2);
	}

	WAVEHDR* allocateBlocks(int size, int count)
	{
		unsigned char* buffer;
		int i;
		WAVEHDR* blocks;
		DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
		/*
		* allocate memory for the entire set in one go
		*/
		if ((buffer = reinterpret_cast<unsigned char *> (HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,totalBufferSize))) == NULL) {
			fprintf(stderr, "Memory allocation error\n");
			ExitProcess(1);
		}
		/*
		* and set up the pointers to each bit
		*/
		blocks = (WAVEHDR*)buffer;
		buffer += sizeof(WAVEHDR)* count;
		for (i = 0; i < count; i++) {
			blocks[i].dwBufferLength = size;
			blocks[i].lpData = reinterpret_cast<LPSTR>(buffer);
			buffer += size;
		}
		return blocks;
	}
	void freeBlocks(WAVEHDR* blockArray)
	{
		/*
		* and this is why allocateBlocks works the way it does
		*/
		HeapFree(GetProcessHeap(), 0, blockArray);
	}

public:
	waveoutcpp(int sampleRate, int bitsPerSample, int channels)
	{
		waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
		waveFreeBlockCount = BLOCK_COUNT;
		waveCurrentBlock = 0;
		InitializeCriticalSection(&waveCriticalSection);


		/*
		* set up the WAVEFORMATEX structure.
		*/
		wfx.nSamplesPerSec = sampleRate; /* sample rate */
		wfx.wBitsPerSample = bitsPerSample; /* sample size */
		wfx.nChannels = channels; /* channels*/
		wfx.cbSize = 0; /* size of _extra_ info */
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
		wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;


		/*
		* try to open the default wave device. WAVE_MAPPER is
		* a constant defined in mmsystem.h, it always points to the
		* default wave device on the system (some people have 2 or
		* more sound cards).
		*/
		int retval = waveOutOpen(
			&hWaveOut,
			WAVE_MAPPER,
			&wfx,
			(DWORD_PTR)waveOutProcWrapper,
			(DWORD_PTR)this,
			CALLBACK_FUNCTION);
		
		if (retval != MMSYSERR_NOERROR) {
			std::string errortext;
			errortext.resize(255);
			waveOutGetErrorTextA(retval, (LPSTR)errortext.data(), errortext.capacity());

			std::cout << "unable to open wave mapper device: " << errortext << std::endl;

			throw std::runtime_error("unable to open wave mapper device");
		}
	};

	~waveoutcpp()
	{
		for (auto i = 0; i < waveFreeBlockCount; i++)
		if (waveBlocks[i].dwFlags & WHDR_PREPARED)
			waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof(WAVEHDR));
		DeleteCriticalSection(&waveCriticalSection);
		freeBlocks(waveBlocks);
		waveOutClose(hWaveOut);

	};

	void writeAudio(LPSTR data, int size)
	{
		WAVEHDR* current;
		int remain;
		current = &waveBlocks[waveCurrentBlock];
		while (size > 0) {
			/*
			* first make sure the header we're going to use is unprepared
			*/
			if (current->dwFlags & WHDR_PREPARED)
				waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
			if (size < (int)(BLOCK_SIZE - current->dwUser)) {
				memcpy(current->lpData + current->dwUser, data, size);
				current->dwUser += size;
				break;
			}
			remain = BLOCK_SIZE - current->dwUser;
			memcpy(current->lpData + current->dwUser, data, remain);
			size -= remain;
			data += remain;
			current->dwBufferLength = BLOCK_SIZE;
			waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
			waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
			EnterCriticalSection(&waveCriticalSection);
			waveFreeBlockCount--;
			LeaveCriticalSection(&waveCriticalSection);
			/*
			* wait for a block to become free
			*/
			while (!waveFreeBlockCount)
				Sleep(10);
			/*
			* point to the next block
			*/
			waveCurrentBlock++;
			waveCurrentBlock %= BLOCK_COUNT;
			current = &waveBlocks[waveCurrentBlock];
			current->dwUser = 0;
		}
	}

	//void writeAudioBlock(LPSTR block, DWORD size)
	//{
	//	WAVEHDR header;
	//	/*
	//	* initialise the block header with the size
	//	* and pointer.
	//	*/
	//	ZeroMemory(&header, sizeof(WAVEHDR));
	//	header.dwBufferLength = size;
	//	header.lpData = block;
	//	/*
	//	* prepare the block for playback
	//	*/
	//	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	//	/*
	//	* write the block to the device. waveOutWrite returns immediately
	//	* unless a synchronous driver is used (not often).
	//	*/
	//	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	//	/*
	//	* wait a while for the block to play then start trying
	//	* to unprepare the header. this will fail until the block has
	//	* played.
	//	*/
	//	//Sleep(500);
	//	while (waveOutUnprepareHeader(
	//		hWaveOut,
	//		&header,
	//		sizeof(WAVEHDR)
	//		) == WAVERR_STILLPLAYING)
	//		Sleep(1);
	//}

	//bool isPlaying(){
	//	return waveFreeBlockCount < BLOCK_COUNT;
	//}
};

#endif