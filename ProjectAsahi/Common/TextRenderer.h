#pragma once

namespace ProjectAsahi
{
	namespace Common
	{
		class TextRenderer : public IDWriteTextRenderer
		{
		public:
			TextRenderer(
				Microsoft::WRL::ComPtr<ID2D1Factory> D2DFactory,
				Microsoft::WRL::ComPtr<ID2D1DeviceContext> D2DDeviceContext,
				Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> outlineBrush,
				Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fillBrush
				);

			IFACEMETHOD(IsPixelSnappingDisabled)(
				_In_opt_ void* clientDrawingContext,
				_Out_ BOOL* isDisabled
				);

			IFACEMETHOD(GetCurrentTransform)(
				_In_opt_ void* clientDrawingContext,
				_Out_ DWRITE_MATRIX* transform
				);

			IFACEMETHOD(GetPixelsPerDip)(
				_In_opt_ void* clientDrawingContext,
				_Out_ FLOAT* pixelsPerDip
				);

			IFACEMETHOD(DrawGlyphRun)(
				_In_opt_ void* clientDrawingContext,
				FLOAT baselineOriginX,
				FLOAT baselineOriginY,
				DWRITE_MEASURING_MODE measuringMode,
				_In_ DWRITE_GLYPH_RUN const* glyphRun,
				_In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
				IUnknown* clientDrawingEffect
				);

			IFACEMETHOD(DrawUnderline)(
				_In_opt_ void* clientDrawingContext,
				FLOAT baselineOriginX,
				FLOAT baselineOriginY,
				_In_ DWRITE_UNDERLINE const* underline,
				IUnknown* clientDrawingEffect
				);

			IFACEMETHOD(DrawStrikethrough)(
				_In_opt_ void* clientDrawingContext,
				FLOAT baselineOriginX,
				FLOAT baselineOriginY,
				_In_ DWRITE_STRIKETHROUGH const* strikethrough,
				IUnknown* clientDrawingEffect
				);

			IFACEMETHOD(DrawInlineObject)(
				_In_opt_ void* clientDrawingContext,
				FLOAT originX,
				FLOAT originY,
				IDWriteInlineObject* inlineObject,
				BOOL isSideways,
				BOOL isRightToLeft,
				IUnknown* clientDrawingEffect
				);

		public:
			IFACEMETHOD_(unsigned long, AddRef) ();
			IFACEMETHOD_(unsigned long, Release) ();
			IFACEMETHOD(QueryInterface) (
				IID const& riid,
				void** ppvObject
				);

		private:
			unsigned long refCount;
			Microsoft::WRL::ComPtr<ID2D1Factory> D2DFactory;
			Microsoft::WRL::ComPtr<ID2D1DeviceContext> D2DDeviceContext;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> outlineBrush;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fillBrush;
		};
	}
}
