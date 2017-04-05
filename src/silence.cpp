
#include "stdafx.h"

#include "windows.h"
#include "audio.h"

enum Action {
	Switch,
	Mute,
	UnMute
};

int main(int argc, _TCHAR* argv[])
{
	Action a = Switch;
	if (argc > 1)
	{
		
		auto act = argv[1];
		if (!_tcscmp(act, _T("mute")))
			a = Mute;
		else if (!_tcscmp(act, _T("unmute")))
			a = UnMute;
		else
			printf("unknow action: %s\n", act);
	}

	CoreApi api;
	BOOL bMute = FALSE;
	HRESULT hr = api.GetMasterMute(&bMute);
	if (FAILED(hr))
	{
		printf("get master mute's state failed\n");
		return 1;
	}
	switch (a)
	{
	case Mute:
		if (!bMute)
			hr = api.SetMasterMute(TRUE, NULL);
		break;
	case UnMute:
		if (bMute)
			hr = api.SetMasterMute(FALSE, NULL);
		break;
	case Switch:
		hr = api.SetMasterMute(!bMute, NULL);
		break;
	}

	if (FAILED(hr))
	{
		printf("action failed\n");
		return 1;
	}

	return 0;
}
