#pragma once

namespace ProjectAsahi
{
	namespace Common
	{
		ref class Loader sealed
		{
		internal:
			Loader(_In_ ID2D1DeviceContext1* d2dContext, IDWriteFactory* dwFactory, _In_opt_ IWICImagingFactory2* wicFactory = nullptr);
			void CreateD2DEffectFromFile(Platform::String^ fileName, ID2D1Effect **effect, D2D1_VECTOR_2F &scale);

		private:
			Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;
			Microsoft::WRL::ComPtr<ID2D1DeviceContext1>	m_d2dContext;
			Microsoft::WRL::ComPtr<IDWriteFactory> m_dwFactory;
		};
	}
}
