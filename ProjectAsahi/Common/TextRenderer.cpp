#include "pch.h"
#include "TextRenderer.h"

using namespace ProjectAsahi::Common;


TextRenderer::TextRenderer(
	Microsoft::WRL::ComPtr<ID2D1Factory> D2DFactory,
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> D2DDeviceContext,
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> outlineBrush,
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fillBrush
	) :
	refCount(0),
	D2DFactory(D2DFactory),
	D2DDeviceContext(D2DDeviceContext),
	outlineBrush(outlineBrush),
	fillBrush(fillBrush)
{
}

IFACEMETHODIMP TextRenderer::DrawGlyphRun(
	_In_opt_ void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	_In_ DWRITE_GLYPH_RUN const* glyphRun,
	_In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<ID2D1PathGeometry> pathGeometry;
	hr = D2DFactory->CreatePathGeometry(&pathGeometry);

	Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
	if (SUCCEEDED(hr))
	{
		hr = pathGeometry->Open(&sink);
	}

	if (SUCCEEDED(hr))
	{
		hr = glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel % 2,
			sink.Get()
			);
	}

	if (SUCCEEDED(hr))
	{
		hr = sink.Get()->Close();
	}

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> transformedGeometry;
	if (SUCCEEDED(hr))
	{
		hr = D2DFactory.Get()->CreateTransformedGeometry(
			pathGeometry.Get(),
			&matrix,
			&transformedGeometry
			);
	}

	D2DDeviceContext->DrawGeometry(
		transformedGeometry.Get(),
		outlineBrush.Get()
		);

	D2DDeviceContext->FillGeometry(
		transformedGeometry.Get(),
		fillBrush.Get()
		);

	return hr;
}


IFACEMETHODIMP TextRenderer::DrawUnderline(
	_In_opt_ void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	_In_ DWRITE_UNDERLINE const* underline,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		underline->offset,
		underline->width,
		underline->offset + underline->thickness
		);

	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> rectangleGeometry;
	hr = D2DFactory.Get()->CreateRectangleGeometry(
		&rect,
		&rectangleGeometry
		);

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> transformedGeometry;
	if (SUCCEEDED(hr))
	{
		hr = D2DFactory.Get()->CreateTransformedGeometry(
			rectangleGeometry.Get(),
			&matrix,
			&transformedGeometry
			);
	}

	D2DDeviceContext.Get()->DrawGeometry(
		transformedGeometry.Get(),
		outlineBrush.Get()
		);

	D2DDeviceContext.Get()->FillGeometry(
		transformedGeometry.Get(),
		fillBrush.Get()
		);

	return S_OK;
}


IFACEMETHODIMP TextRenderer::DrawStrikethrough(
	_In_opt_ void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	_In_ DWRITE_STRIKETHROUGH const* strikethrough,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		strikethrough->offset,
		strikethrough->width,
		strikethrough->offset + strikethrough->thickness
		);

	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> rectangleGeometry;
	hr = D2DFactory.Get()->CreateRectangleGeometry(
		&rect,
		&rectangleGeometry
		);

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> transformedGeometry;
	if (SUCCEEDED(hr))
	{
		hr = D2DFactory.Get()->CreateTransformedGeometry(
			rectangleGeometry.Get(),
			&matrix,
			&transformedGeometry
			);
	}

	D2DDeviceContext.Get()->DrawGeometry(
		transformedGeometry.Get(),
		outlineBrush.Get()
		);

	D2DDeviceContext.Get()->FillGeometry(
		transformedGeometry.Get(),
		fillBrush.Get()
		);

	return S_OK;
}


IFACEMETHODIMP TextRenderer::DrawInlineObject(
	_In_opt_ void* clientDrawingContext,
	FLOAT originX,
	FLOAT originY,
	IDWriteInlineObject* inlineObject,
	BOOL isSideways,
	BOOL isRightToLeft,
	IUnknown* clientDrawingEffect
	)
{
	return E_NOTIMPL;
}


IFACEMETHODIMP_(unsigned long) TextRenderer::AddRef()
{
	return InterlockedIncrement(&refCount);
}


IFACEMETHODIMP_(unsigned long) TextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&refCount);
	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}


IFACEMETHODIMP TextRenderer::IsPixelSnappingDisabled(
	_In_opt_ void* clientDrawingContext,
	_Out_ BOOL* isDisabled
	)
{
	*isDisabled = FALSE;
	return S_OK;
}


IFACEMETHODIMP TextRenderer::GetCurrentTransform(
	_In_opt_ void* clientDrawingContext,
	_Out_ DWRITE_MATRIX* transform
	)
{
	D2DDeviceContext->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}


IFACEMETHODIMP TextRenderer::GetPixelsPerDip(
	_In_opt_ void* clientDrawingContext,
	_Out_ FLOAT* pixelsPerDip
	)
{
	float x, yUnused;

	D2DDeviceContext.Get()->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}


IFACEMETHODIMP TextRenderer::QueryInterface(
	IID const& riid,
	void** ppvObject
	)
{
	if (__uuidof(IDWriteTextRenderer) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IDWritePixelSnapping) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	}
	else
	{
		*ppvObject = nullptr;
		return E_FAIL;
	}

	this->AddRef();

	return S_OK;
}

