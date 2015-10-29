#include "pch.h"
#include "Loader.h"
#include "Common\DirectXHelper.h"
#include <ppltasks.h>
#include <shcore.h>

using namespace Microsoft::WRL;
using namespace ProjectAsahi::Common;
using namespace Windows::Storage::Streams;
using namespace concurrency;


ProjectAsahi::Common::Loader::Loader(ID2D1DeviceContext1 * d2dContext, IDWriteFactory* dwFactory, IWICImagingFactory2 * wicFactory)
	:m_d2dContext(d2dContext),
	m_dwFactory(dwFactory),
	m_wicFactory(wicFactory)
{

}


void Loader::CreateD2DEffectFromFile(Platform::String^ fileName, ID2D1Effect ** effect, D2D1_VECTOR_2F &scale)
{
	create_task(FileManager::Manager::GetFileStreamAsync(fileName)).then([=](IBuffer^ buf)
	{
		InMemoryRandomAccessStream^ memstream = ref new InMemoryRandomAccessStream();
		create_task(memstream->WriteAsync(buf)).then([=](unsigned int progress)
		{
			ComPtr<IWICBitmapDecoder> decoder;
			ComPtr<IStream> stream;
			ComPtr<ID2D1Image> img;
			ComPtr<ID2D1Effect> ef;
			DX::ThrowIfFailed(CreateStreamOverRandomAccessStream(reinterpret_cast<IUnknown*>(memstream), IID_PPV_ARGS(&stream)));
			DX::ThrowIfFailed(m_wicFactory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand, &decoder));
			ComPtr<IWICBitmapFrameDecode> frame;
			DX::ThrowIfFailed(decoder->GetFrame(0, &frame));
			UINT a, b;
			frame->GetSize(&a, &b);
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
			DX::ThrowIfFailed((*effect)->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, scale));
			//ef->GetOutput(&img);
			////DX::ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1Composite, &(*effect)));
			////DX::ThrowIfFailed(
			////	(*effect)->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_IN)
			////	);
			//(*effect)->SetInput(0, img.Get());
			//ef.Reset();
			decoder.Reset();
			frame.Reset();
			formatConvert.Reset();
		});
	});
}

