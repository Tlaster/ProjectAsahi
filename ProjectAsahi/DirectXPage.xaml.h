//
//DirectXPage.xaml.h
// DirectXPage 类的声明。
//

#pragma once

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "ProjectAsahiMain.h"
#include "Common\Timer.h"
#include "Entities\GameState.h"

namespace ProjectAsahi
{
	/// <summary>
	///包含 DirectX SwapChainPanel 的页。
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	private:

		bool isLogoMediaEnded;

		void CheckScreen();
		void HideEveryThing();
		void ShowGrid();
		// XAML 低级渲染事件处理程序。
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);
		
		// DisplayInformation 事件处理程序。
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// 其他事件处理程序。
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// 在后台工作线程上跟踪我们的独立输入。
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// 独立输入处理函数。
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		// 用于在 XAML 页面背景中呈现 DirectX 内容的资源。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<ProjectAsahiMain> m_main; 
		bool m_windowVisible;

		ProjectAsahi::Common::Timer^ m_timer;
		ProjectAsahi::Entities::GameState _prevGameState;
		void StartButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ExitButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LogoElement_MediaEnded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LogoElement_DoubleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs^ e);
		void OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e);
	};
}

