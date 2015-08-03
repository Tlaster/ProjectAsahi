#include "pch.h"
#include "Loader.h"
#include "Common\DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace ProjectAsahi::Common;


ProjectAsahi::Common::Loader::Loader(ID2D1DeviceContext1 * d2dContext, IDWriteFactory* dwFactory, IWICImagingFactory2 * wicFactory)
	:m_d2dContext(d2dContext),
	m_dwFactory(dwFactory),
	m_wicFactory(wicFactory)
{

}

void Loader::CreateD2DEffectFromFile(LPCWSTR wzFilename, ID2D1Effect ** effect)
{
	ComPtr<IWICBitmapDecoder> decoder;
	DX::ThrowIfFailed(m_wicFactory->CreateDecoderFromFilename(wzFilename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder));
	ComPtr<IWICBitmapFrameDecode> frame;
	DX::ThrowIfFailed(decoder->GetFrame(0, &frame));
	ComPtr<IWICFormatConverter> formatConvert;
	DX::ThrowIfFailed(m_wicFactory->CreateFormatConverter(&formatConvert));
	DX::ThrowIfFailed(formatConvert->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
		));
	DX::ThrowIfFailed(
		m_d2dContext->CreateEffect(CLSID_D2D1BitmapSource, &(*effect))
		);
	DX::ThrowIfFailed(
		(*effect)->SetValue(D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE, formatConvert.Get())
		);
	decoder.Reset();
	frame.Reset();
	formatConvert.Reset();
}

