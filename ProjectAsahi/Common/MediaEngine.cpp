#include "pch.h"
#include "MediaEngine.h"
#include <AudioSessionTypes.h>

using namespace ProjectAsahi::Common;
using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

class MediaEngineNotify : public IMFMediaEngineNotify
{
	long m_cRef;
	MediaEngineNotifyCallback^ m_pCB;

public:
	MediaEngineNotify() : m_cRef(1), m_pCB(nullptr)
	{
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		if (__uuidof(IMFMediaEngineNotify) == riid)
		{
			*ppv = static_cast<IMFMediaEngineNotify*>(this);
		}
		else
		{
			*ppv = nullptr;
			return E_NOINTERFACE;
		}

		AddRef();

		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;
	}

	// EventNotify is called when the Media Engine sends an event.
	STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD param2)
	{
		if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
		{
			SetEvent(reinterpret_cast<HANDLE>(param1));
		}
		else
		{
			m_pCB->OnMediaEngineEvent(meEvent);
		}

		return S_OK;
	}

	void MediaEngineNotifyCallback(MediaEngineNotifyCallback^ pCB)
	{
		m_pCB = pCB;
	}
};

MediaEngine::MediaEngine()
	:m_bstrURL(nullptr)
{
	Initialize();
}

MediaEngine::~MediaEngine()
{
	m_spMediaEngine->Shutdown();
	m_spEngineEx->Shutdown();
	if (nullptr != m_bstrURL)
	{
		::CoTaskMemFree(m_bstrURL);
	}
	MFShutdown();
	m_spMediaEngine.Reset();
	m_spEngineEx.Reset();
}

void MediaEngine::Initialize()
{
	ComPtr<IMFMediaEngineClassFactory> spFactory;
	ComPtr<IMFAttributes> spAttributes;
	ComPtr<MediaEngineNotify> spNotify;

	DX::ThrowIfFailed(MFStartup(MF_VERSION));
	MULTI_QI mqi = { 0 };
	mqi.hr = S_OK;
	mqi.pIID = &__uuidof(IMFMediaEngineClassFactory);
	HRESULT hr = CoCreateInstanceFromApp(CLSID_MFMediaEngineClassFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		nullptr,
		1,
		&mqi);
	if (FAILED(hr) || FAILED(mqi.hr))
		throw ref new Platform::COMException(hr);
	spFactory.Attach((IMFMediaEngineClassFactory*)mqi.pItf);
	//DX::ThrowIfFailed(
	//	CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spFactory))
	//	);
	DX::ThrowIfFailed(
		MFCreateAttributes(&spAttributes, 1)
		);
	spNotify = new MediaEngineNotify();

	spNotify->MediaEngineNotifyCallback(this);

	DX::ThrowIfFailed(
		spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*)spNotify.Get())
		);

	DX::ThrowIfFailed(
		spAttributes->SetUINT32(MF_MEDIA_ENGINE_AUDIO_CATEGORY, AudioCategory_GameMedia)
		);
	const DWORD flags = MF_MEDIA_ENGINE_AUDIOONLY | MF_MEDIA_ENGINE_REAL_TIME_MODE;

	//const DWORD flags = MF_MEDIA_ENGINE_WAITFORSTABLE_STATE;
	DX::ThrowIfFailed(
		spFactory->CreateInstance(flags, spAttributes.Get(), &m_spMediaEngine)
		);
	DX::ThrowIfFailed(
		m_spMediaEngine.Get()->QueryInterface(__uuidof(IMFMediaEngine), (void**)&m_spEngineEx)
		);

	SetURL(L"Why do I fucking need to pass an url when I play stream source?");
}

void MediaEngine::OnMediaEngineEvent(DWORD meEvent)
{
	switch (meEvent)
	{
	case MF_MEDIA_ENGINE_EVENT_CANPLAY:
		DX::ThrowIfFailed(m_spMediaEngine->Play());
		break;
	default:
		break;
	}

	return;
}

void MediaEngine::PlayMusic(Platform::String^ szURI, bool isLoop)
{
	create_task(FileManager::Manager::GetFileStreamAsync(szURI)).then([=](IBuffer^ buf)
	{
		InMemoryRandomAccessStream^ memstream = ref new InMemoryRandomAccessStream();
		create_task(memstream->WriteAsync(buf)).then([=](unsigned int progress)
		{
			SetBytestream(memstream);
		});
	});
	if (m_spMediaEngine)
	{
		m_spMediaEngine->SetLoop(isLoop);
	}
}

void MediaEngine::SetBytestream(IRandomAccessStream^ streamHandle)
{
	ComPtr<IMFByteStream> spMFByteStream = nullptr;
	DX::ThrowIfFailed(
		MFCreateMFByteStreamOnStreamEx((IUnknown*)streamHandle, &spMFByteStream)
		);
	DX::ThrowIfFailed(
		m_spEngineEx->SetSourceFromByteStream(spMFByteStream.Get(), m_bstrURL)
		);
	return;
}

void MediaEngine::SetURL(Platform::String^ szURL)
{
	if (nullptr != m_bstrURL)
	{
		::CoTaskMemFree(m_bstrURL);
		m_bstrURL = nullptr;
	}

	size_t cchAllocationSize = 1 + ::wcslen(szURL->Data());
	m_bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

	if (m_bstrURL == 0)
	{
		DX::ThrowIfFailed(E_OUTOFMEMORY);
	}

	StringCchCopyW(m_bstrURL, cchAllocationSize, szURL->Data());

	return;
}