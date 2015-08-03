//
// DirectXPage.xaml.cpp
// DirectXPage 类的实现。
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace ProjectAsahi;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace ProjectAsahi::Entities;
using namespace ProjectAsahi::Common;

DirectXPage::DirectXPage():
	m_windowVisible(true),
	m_coreInput(nullptr)
{
	InitializeComponent();

	// 注册页面生命周期的事件处理程序。
	CoreWindow^ window = Window::Current->CoreWindow;

	//window->VisibilityChanged +=
	//	ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	//Error
	//if (Windows::Foundation::Metadata::ApiInformation::IsTypePresent(L"Windows::UI::ViewManagement::StatusBar"))
	//Correct
	if (Windows::Foundation::Metadata::ApiInformation::IsTypePresent(L"Windows.UI.ViewManagement.StatusBar"))
	{
		Windows::UI::ViewManagement::StatusBar::GetForCurrentView()->HideAsync();
	}
	Windows::System::Display::DisplayRequest^ req = ref new Windows::System::Display::DisplayRequest();
	req->RequestActive();
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged += 
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);
	swapChainPanel->KeyDown += ref new Windows::UI::Xaml::Input::KeyEventHandler(this, &ProjectAsahi::DirectXPage::OnKeyDown);
	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// 此时，我们具有访问设备的权限。
	// 我们可创建与设备相关的资源。
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

	// 注册我们的 SwapChainPanel 以获取独立的输入指针事件
	auto workItemHandler = ref new WorkItemHandler([this] (IAsyncAction ^)
	{
		// 对于指定的设备类型，无论它是在哪个线程上创建的，CoreIndependentInputSource 都将引发指针事件。
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);

		// 指针事件的寄存器，将在后台线程上引发。
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);

		// 一旦发送输入消息，即开始处理它们。
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// 在高优先级的专用后台线程上运行任务。
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<ProjectAsahiMain>(new ProjectAsahiMain(m_deviceResources));
	_prevGameState = GameState::GS_LOGO;
	m_timer = ref new Timer();
	CompositionTarget::Rendering += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &ProjectAsahi::DirectXPage::OnRendering);
}

DirectXPage::~DirectXPage()
{
	// 析构时停止渲染和处理事件。
	m_main.reset();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// 针对挂起和终止事件保存应用程序的当前状态。
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();
}

// 针对恢复事件加载应用程序的当前状态。
void DirectXPage::LoadInternalState(IPropertySet^ state)
{

}
void ProjectAsahi::DirectXPage::HideEveryThing()
{
	OpenGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	MenuGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void ProjectAsahi::DirectXPage::ShowGrid()
{
	switch (m_main->CurrentGameState)
	{
	case ProjectAsahi::Entities::GameState::GS_MAIN_MENU:
		MenuGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	case ProjectAsahi::Entities::GameState::GS_PLAYING:
		break;
	case ProjectAsahi::Entities::GameState::GS_PAUSED:
		break;
	case ProjectAsahi::Entities::GameState::GS_LOGO:
		OpenGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
}
void ProjectAsahi::DirectXPage::CheckScreen()
{
	if (_prevGameState != m_main->CurrentGameState)
	{
		HideEveryThing();
		ShowGrid();
		_prevGameState = m_main->CurrentGameState;
	}
}

// 窗口事件处理程序。
void ProjectAsahi::DirectXPage::OnRendering(Platform::Object ^ sender, Platform::Object ^ args)
{
	m_timer->Update();
	if (isLogoMediaEnded)
	{
		m_main->Update(m_timer->Total, m_timer->Delta);
		CheckScreen();
		m_main->Render();
		m_deviceResources->Present();
	}
}


// DisplayInformation 事件处理程序。

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}


void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}


void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	m_main->OnPointerPressed(sender, e);
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	m_main->OnPointerMoved(sender, e);
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	m_main->OnPointerReleased(sender, e);
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}


void ProjectAsahi::DirectXPage::StartButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_main->CurrentGameState = GameState::GS_PLAYING;
}


void ProjectAsahi::DirectXPage::ExitButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	App::Current->Exit();
}


void ProjectAsahi::DirectXPage::LogoElement_MediaEnded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	isLogoMediaEnded = true;
	LogoElement->IsFullWindow = false;
	m_main->CurrentGameState = GameState::GS_MAIN_MENU;
}


void ProjectAsahi::DirectXPage::LogoElement_DoubleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs^ e)
{
	LogoElement->Stop();
	isLogoMediaEnded = true;
	LogoElement->IsFullWindow = false;
	m_main->CurrentGameState = GameState::GS_MAIN_MENU;
}


void ProjectAsahi::DirectXPage::OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e)
{
	m_main->OnKeyDown(sender, e);
}
