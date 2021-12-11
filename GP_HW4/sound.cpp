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

	if (!soundManager->add(L"./sound/�������.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/�ٶ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/����.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/����.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/������.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/����.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/ǥ����.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/��������.wav", &id))
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