#pragma once

#include <mfmediaengine.h>
#include "Common\DirectXHelper.h"
#include <ppltasks.h>
#include <mfapi.h>
#include <strsafe.h>


namespace ProjectAsahi
{
	namespace Common
	{
		ref struct MediaEngineNotifyCallback abstract
		{
		internal:
			virtual void OnMediaEngineEvent(DWORD meEvent) = 0;
		};

		ref class MediaEngine : public MediaEngineNotifyCallback
		{
			Microsoft::WRL::ComPtr<IMFMediaEngine>              m_spMediaEngine;
			Microsoft::WRL::ComPtr<IMFMediaEngineEx>            m_spEngineEx;

			BSTR                                    m_bstrURL;
		internal:
			MediaEngine();
			void Initialize();
			void SetURL(Platform::String^ szURL);
			void SetBytestream(Windows::Storage::Streams::IRandomAccessStream^ streamHandle);
			void PlayMusic(Platform::String^ szURI, bool isLoop = false);
			virtual void OnMediaEngineEvent(DWORD meEvent) override;

			//MaxValue:1
			void SetVolume(double vol) { DX::ThrowIfFailed(m_spMediaEngine->SetVolume(vol)); }
			void Stop() { DX::ThrowIfFailed(m_spMediaEngine->Pause()); DX::ThrowIfFailed(m_spMediaEngine->SetCurrentTime(0)); }
			bool IsEnded()
			{
				//CurrentTime sometime can be lager than the Duration, I have no idea why
				return m_spMediaEngine->GetCurrentTime() >= m_spMediaEngine->GetDuration();
			}

		private:
			~MediaEngine();
		};
	}
}


