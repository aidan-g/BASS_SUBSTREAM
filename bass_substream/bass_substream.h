#include "../bass/bass.h"
#include "../bass/bass_addon.h"

#ifndef BASSSUBSTREAMDEF
#define BASSSUBSTREAMDEF(f) WINAPI f
#endif

__declspec(dllexport)
HSTREAM BASSSUBSTREAMDEF(BASS_SUBSTREAM_StreamCreate)(HSTREAM handle, QWORD offset, QWORD length, DWORD flags);

__declspec(dllexport)
DWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_StreamProc)(HSTREAM handle, void* buffer, DWORD length, void* user);

__declspec(dllexport)
QWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_GetLength)(void* inst, DWORD mode);

__declspec(dllexport)
VOID BASSSUBSTREAMDEF(BASS_SUBSTREAM_GetInfo)(void* inst, BASS_CHANNELINFO* info);

__declspec(dllexport)
BOOL BASSSUBSTREAMDEF(BASS_SUBSTREAM_CanSetPosition)(void* inst, QWORD position, DWORD mode);

__declspec(dllexport)
QWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_SetPosition)(void* inst, QWORD position, DWORD mode);

__declspec(dllexport)
VOID BASSSUBSTREAMDEF(BASS_SUBSTREAM_Free)(void* inst);