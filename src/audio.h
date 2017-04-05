#pragma once

#include <atlbase.h>
#include <Mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <Functiondiscoverykeys_devpkey.h>

#define EXIT_ON_ERROR(hr) if (FAILED(hr)) return hr
#define SAFE_RELEASE(p) if (p) p->Release()

class CoreApi
{
public:
	CoreApi(void) { CoInitializeEx(NULL, COINIT_MULTITHREADED); }

	IDeviceTopology* pDefaultDevTopo;

	// MMDevice ID 扬声器
	LPWSTR deviceId_out_master;//扬声器
	LPWSTR deviceId_out_spdif;//S/PDIF Pass-through Device

	//**************************************设备ID
	bool InitCoreApi(void);//调用一下四个InitXXX函数，完成api的初始化
	// 初始化DeviceId
	bool InitDeviceIds(void);//devId[7];

	HRESULT GetMasterVolId();//LPWSTR deviceId
	HRESULT GetMasterVol(float  *pfLevel);
	HRESULT SetMasterVol(float  fLevel, LPCGUID  EventContext);
	HRESULT GetMasterMute(BOOL  *pbMute);
	HRESULT SetMasterMute(BOOL  bMute, LPCGUID  EventContext);
	HRESULT GetMasterChannelVol(UINT nChannel, float  *pfLevel);
	HRESULT SetMasterChannelVol(UINT nChannel, float  fLevel, LPCGUID  EventContext);


	HRESULT GetMMDevice(LPWSTR deviceId, IMMDevice** ppDevice);


	//-------EndPoint^^^^^^^^^^^^^^^^^^^^^^^^^^

	HRESULT GetEndPtVolByDevId(LPWSTR deviceId, UINT nChannel, float* pfLevel);
	bool	SetEndPtVolByDevId(LPWSTR deviceId, UINT nChannel, float Level, LPCGUID  pguidEventContext = &GUID_NULL);

	HRESULT GetEndPtMuteByDevId(LPWSTR deviceId, BOOL *pbMute);
	bool	SetEndPtMuteByDevId(LPWSTR deviceId, BOOL bMute, LPCGUID pguidEventContext = &GUID_NULL);


	//-------EndPoint__________________________
};


bool CoreApi::InitDeviceIds(void)
{
	/*bool bFindDev_master = false;
	bool bFindDev_spdif = false;

	IMMDeviceCollection *pCollection = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	EXIT_ON_ERROR(hr);

	IMMDeviceEnumerator* pEnumerator;
	hr = pEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &pCollection);
	EXIT_ON_ERROR(hr);

	UINT deviceCount = 0;
	hr = pCollection->GetCount(&deviceCount);
	EXIT_ON_ERROR(hr);

	for (UINT dev = 0; dev<deviceCount; dev++)
	{
	IMMDevice *pDevice = NULL;
	hr = pCollection->Item(dev, &pDevice);

	if (hr == S_OK)
	{
	IPropertyStore *pProperties = NULL;
	hr = pDevice->OpenPropertyStore(STGM_READ, &pProperties);
	if (hr == S_OK)
	{
	PROPVARIANT varName;
	PropVariantInit(&varName);
	hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);

	if (hr == S_OK)
	{
	if (wcsstr(varName.pwszVal, L"hifier") >= 0)
	{

	}
	}
	PropVariantClear(&varName);
	}

	SAFE_RELEASE(pProperties);
	}

	}
	if (bFindDev_master&&bFindDev_spdif)
	{
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pCollection);
	return true;

	}

	Exit:
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pCollection);*/
	return false;
}

HRESULT CoreApi::GetMasterVol(float *pfLevel)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);
	// -------------------------
	//float currentVolume = 0;
	_hrStatus = spEndpointVolume->GetMasterVolumeLevel(pfLevel);
	EXIT_ON_ERROR(_hrStatus);
	return true;
}

HRESULT CoreApi::SetMasterVol(float fLevel, LPCGUID EventContext)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spEndpointVolume->SetMasterVolumeLevel(fLevel, EventContext);
	EXIT_ON_ERROR(_hrStatus);

	return true;
}

HRESULT CoreApi::GetMasterVolId()
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spDevice->GetId(&deviceId_out_master);
	EXIT_ON_ERROR(_hrStatus);

	return true;
}

HRESULT CoreApi::GetMasterChannelVol(UINT nChannel, float *pfLevel)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spEndpointVolume->GetChannelVolumeLevelScalar(nChannel, pfLevel);
	EXIT_ON_ERROR(_hrStatus);

	return true;
}

HRESULT CoreApi::SetMasterChannelVol(UINT nChannel, float  fLevel, LPCGUID  EventContext)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spEndpointVolume->SetChannelVolumeLevelScalar(nChannel, fLevel, EventContext);
	EXIT_ON_ERROR(_hrStatus);

	return true;
}

HRESULT CoreApi::GetMasterMute(BOOL  *pbMute)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spEndpointVolume->GetMute(pbMute);
	return _hrStatus;
}

HRESULT CoreApi::SetMasterMute(BOOL  bMute, LPCGUID  EventContext)
{
	CComPtr<IMMDeviceEnumerator> spEnumerator;
	HRESULT _hrStatus = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&spEnumerator);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IMMDevice> spDevice;
	_hrStatus = spEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	EXIT_ON_ERROR(_hrStatus);

	CComPtr<IAudioEndpointVolume> spEndpointVolume;
	_hrStatus = spDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&spEndpointVolume);
	EXIT_ON_ERROR(_hrStatus);

	_hrStatus = spEndpointVolume->SetMute(bMute, EventContext);
	EXIT_ON_ERROR(_hrStatus);
	return _hrStatus;
}

HRESULT CoreApi::GetEndPtVolByDevId(LPWSTR deviceId, UINT nChannel, float* pfLevel)
{
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioEndpointVolume *pVolumeAPI = NULL;


	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (hr != S_OK)
		goto Exit;
	hr = pEnumerator->GetDevice(deviceId, &pDevice);//通过Init()函数初始化了的MMDevice ID 来创建设备对象
	if (hr != S_OK)
		goto Exit;

	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)(&pVolumeAPI));
	if (hr != S_OK)
		goto Exit;


	hr = pVolumeAPI->GetChannelVolumeLevelScalar(nChannel, pfLevel);


Exit:
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pVolumeAPI);
	return hr;
}

bool CoreApi::SetEndPtVolByDevId(LPWSTR deviceId, UINT nChannel, float Level, LPCGUID  pguidEventContext)
{
	bool b = false;

	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;

	IAudioEndpointVolume *pVolumeAPI = NULL;

	//	CAudioEndpointVolumeCallback *EPVolEvents=new CAudioEndpointVolumeCallback;


	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (hr != S_OK)
		goto Exit;
	hr = pEnumerator->GetDevice(deviceId, &pDevice);//通过Init()函数初始化了的MMDevice ID 来创建设备对象
	if (hr != S_OK)
		goto Exit;
	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)(&pVolumeAPI));
	if (hr != S_OK)
		goto Exit;
	//	hr = pVolumeAPI->RegisterControlChangeNotify(
	//		(IAudioEndpointVolumeCallback*)EPVolEvents
	//		(IAudioEndpointVolumeCallback*)g_pVolumeMonitor
	//		);

	if (S_OK == pVolumeAPI->SetChannelVolumeLevelScalar(nChannel, Level, pguidEventContext))
		b = true;

Exit:

	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pVolumeAPI);
	return b;
}

HRESULT CoreApi::GetEndPtMuteByDevId(LPWSTR deviceId, BOOL *pbMute)
{
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioEndpointVolume *pVolumeAPI = NULL;


	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (hr != S_OK)
		goto Exit;
	hr = pEnumerator->GetDevice(deviceId, &pDevice);//通过Init()函数初始化了的MMDevice ID 来创建设备对象
	if (hr != S_OK)
		goto Exit;

	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)(&pVolumeAPI));
	if (hr != S_OK)
		goto Exit;


	hr = pVolumeAPI->GetMute(pbMute);


Exit:
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pVolumeAPI);
	return hr;
}

bool CoreApi::SetEndPtMuteByDevId(LPWSTR deviceId, BOOL bMute, LPCGUID pguidEventContext)
{
	bool b = false;

	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioEndpointVolume *pVolumeAPI = NULL;

	//	CAudioEndpointVolumeCallback *EPVolEvents=new CAudioEndpointVolumeCallback;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (hr != S_OK)
		goto Exit;
	hr = pEnumerator->GetDevice(deviceId, &pDevice);//通过Init()函数初始化了的MMDevice ID 来创建设备对象
	if (hr != S_OK)
		goto Exit;

	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)(&pVolumeAPI));
	if (hr != S_OK)
		goto Exit;

	//	hr = pVolumeAPI->RegisterControlChangeNotify(
	//		(IAudioEndpointVolumeCallback*)EPVolEvents);
	if (S_OK == pVolumeAPI->SetMute(bMute, pguidEventContext))
		b = true;

Exit:
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pVolumeAPI);
	return b;
}
