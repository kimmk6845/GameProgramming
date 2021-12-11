#include "hw3.h"
#include "SoundManager.h"
#include <time.h>

using namespace D2D1;

BOOL DemoApp::addSound(HWND hWnd) {
	//SOUND
	soundManager = new CSoundManager;
	if (!soundManager->init(hWnd))
		return FALSE;

	int id;

	if (!soundManager->add(L"./sound/배경음악.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/바람.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/장전.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/권총.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/라이플.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/샷건.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/표적지.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/동물명중.wav", &id))
		return FALSE;
	return true;
}


BOOL DemoApp::SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG IPan) {
	if (lpDSBuffer->SetPan(IPan) != DS_OK)
		return FALSE;

	return TRUE;
}


BOOL DemoApp::SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume) {
	if (!lpDSBuffer) return FALSE;
	if (FAILED(lpDSBuffer->SetVolume(DSVOLUME_TO_DB(lVolume))))
		return FALSE;

	return TRUE;
}