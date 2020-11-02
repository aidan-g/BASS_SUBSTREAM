#ifdef _DEBUG
#include <stdio.h>
#endif

#include "bass_substream.h"

typedef struct {
	DWORD handle;
	QWORD offset;
	QWORD length;
	DWORD flags;
} SUBSTREAM;

extern const ADDON_FUNCTIONS addon_functions;

//We just privode the minimum functions, the NULL slots are optional.
const ADDON_FUNCTIONS addon_functions = {
	0,
	&BASS_SUBSTREAM_Free,
	&BASS_SUBSTREAM_GetLength,
	NULL,
	NULL,
	&BASS_SUBSTREAM_GetInfo,
	&BASS_SUBSTREAM_CanSetPosition,
	&BASS_SUBSTREAM_SetPosition,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

//I have no idea how to prevent linking against this routine in msvcrt.
//It doesn't exist on Windows XP.
//Hopefully it doesn't do anything important.
int _except_handler4_common() {
	return 0;
}

HSTREAM BASSSUBSTREAMDEF(BASS_SUBSTREAM_StreamCreate)(HSTREAM handle, QWORD offset, QWORD length, DWORD flags) {
	BASS_CHANNELINFO info;
	SUBSTREAM* substream;
	if (!BASS_ChannelGetInfo(handle, &info)) {
#if _DEBUG
		printf("Failed to get stream info.\n");
#endif
		return BASS_ERROR_UNKNOWN;
	}
	substream = malloc(sizeof(SUBSTREAM));
	substream->handle = handle;
	substream->offset = offset;
	substream->length = length;
	substream->flags = flags;
	handle = bassfunc->CreateStream(
		info.freq,
		info.chans,
		flags,
		&BASS_SUBSTREAM_StreamProc,
		substream,
		&addon_functions
	);
	if (handle == 0) {
		BASS_SUBSTREAM_Free(substream);
		return 0;
	}
	if (!BASS_ChannelSetPosition(substream->handle, offset, BASS_POS_BYTE)) {
		BASS_StreamFree(substream->handle);
		BASS_SUBSTREAM_Free(substream);
		return 0;
	}
	return handle;
}

DWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_StreamProc)(HSTREAM handle, void* buffer, DWORD length, void* user) {
	SUBSTREAM* stream = user;
	DWORD remaining = (DWORD)((stream->offset + stream->length) - BASS_ChannelGetPosition(stream->handle, BASS_POS_BYTE));
	if (length > remaining) {
		length = remaining;
	}
	if (length && BASS_ChannelIsActive(stream->handle) == BASS_ACTIVE_PLAYING) {
		length = BASS_ChannelGetData(stream->handle, buffer, length);
	}
	else {
		length = BASS_STREAMPROC_END;
	}
	return length;
}

QWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_GetLength)(void* inst, DWORD mode) {
	SUBSTREAM* substream = inst;
	if (mode == BASS_POS_BYTE) {
		return substream->length;
	}
	else {
		errorn(BASS_ERROR_NOTAVAIL);
		return 0;
	}
}

VOID BASSSUBSTREAMDEF(BASS_SUBSTREAM_GetInfo)(void* inst, BASS_CHANNELINFO* info) {
	SUBSTREAM* substream = inst;
	BASS_ChannelGetInfo(substream->handle, info);
}

BOOL BASSSUBSTREAMDEF(BASS_SUBSTREAM_CanSetPosition)(void* inst, QWORD position, DWORD mode) {
	SUBSTREAM* substream = inst;
	if (mode == BASS_POS_BYTE) {
		if (position < 0 || position > substream->length) {
			errorn(BASS_ERROR_POSITION);
			return FALSE;
		}
		return TRUE;
	}
	else {
		errorn(BASS_ERROR_NOTAVAIL);
		return FALSE;
	}
}

QWORD BASSSUBSTREAMDEF(BASS_SUBSTREAM_SetPosition)(void* inst, QWORD position, DWORD mode) {
	SUBSTREAM* substream = inst;
	if (mode == BASS_POS_BYTE) {
		BASS_ChannelSetPosition(substream->handle, substream->offset + position, mode);
	}
	else {
		errorn(BASS_ERROR_NOTAVAIL);
		return 0;
	}
}

VOID BASSSUBSTREAMDEF(BASS_SUBSTREAM_Free)(void* inst) {
	SUBSTREAM* substream = inst;
	if (substream) {
		if ((substream->flags & BASS_STREAM_AUTOFREE) == BASS_STREAM_AUTOFREE) {
			BASS_StreamFree(substream->handle);
		}
		free(substream);
	}
}