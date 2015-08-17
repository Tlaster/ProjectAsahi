//
// App.xaml.cpp
// App 类的实现。
//

#include "pch.h"
#include "App.xaml.h"
#include "GamePages\MenuPage.xaml.h"
#include "GamePages\LogoPage.xaml.h"
#include "GamePages\GamePlayPage.xaml.h"
#include "GamePages\SavePage.xaml.h"

using namespace ProjectAsahi;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

Platform::Collections::Vector<ProjectAsahi::Entities::GameState>^ App::_gameStateStack;

/// <summary>
/// 初始化单一实例应用程序对象。这是执行的创作代码的第一行，
/// 已执行，逻辑上等同于 main() 或 WinMain()。
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	Resuming += ref new EventHandler<Object^>(this, &App::OnResuming);
}

/// <summary>
/// 在应用程序由最终用户正常启动时进行调用。
/// 当启动应用程序以打开特定的文件或显示时使用
/// 搜索结果等
/// </summary>
/// <param name="e">有关启动请求和过程的详细信息。</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
#if _DEBUG
	if (IsDebuggerPresent())
	{
		DebugSettings->EnableFrameRateCounter = true;
	}
#endif
	_gameStateStack = ref new Platform::Collections::Vector<ProjectAsahi::Entities::GameState>();
	_gameStateStack->Append(Entities::GameState::GS_PAUSED);

	if (m_directXPage == nullptr)
	{
		m_directXPage = ref new DirectXPage();
	}

	if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
	{
		m_directXPage->LoadInternalState(ApplicationData::Current->LocalSettings->Values);
	}

	// 将页面放在当前窗口中并确保其处于活动状态。
	Window::Current->Content = m_directXPage;
	Window::Current->Activate();

	Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &ProjectAsahi::App::OnBackRequested);
}

void ProjectAsahi::App::GoBack()
{
	RootFrame->GoBack();
	_gameStateStack->RemoveAtEnd();
}

/// <summary>
/// 在将要挂起应用程序执行时调用。  在不知道应用程序
/// 无需知道应用程序会被终止还是会恢复，
/// 并让内存内容保持不变。
/// </summary>
/// <param name="sender">挂起的请求的源。</param>
/// <param name="e">有关挂起请求的详细信息。</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	(void) sender;	// 未使用的参数
	(void) e;	// 未使用的参数

	m_directXPage->SaveInternalState(ApplicationData::Current->LocalSettings->Values);
}

/// <summary>
/// 在恢复应用程序执行时调用。
/// </summary>
/// <param name="sender">恢复请求的源。</param>
/// <param name="args">有关恢复请求的详细信息。</param>
void App::OnResuming(Object ^sender, Object ^args)
{
	(void) sender; // 未使用的参数
	(void) args; // 未使用的参数

	m_directXPage->LoadInternalState(ApplicationData::Current->LocalSettings->Values);
}


void ProjectAsahi::App::OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args)
{
	if (RootFrame->CanGoBack)
	{
		GoBack();
	}
	else
	{
		Current->Exit();
	}
}

void ProjectAsahi::App::SetGameState(ProjectAsahi::Entities::GameState value)
{
	if (CurrentGameState != ProjectAsahi::Entities::GameState::GS_SAVE || CurrentGameState != ProjectAsahi::Entities::GameState::GS_LOAD)
	{
		switch (value)
		{
		case ProjectAsahi::Entities::GameState::GS_LOGO:
			RootFrame->BackStack->Clear();
			_gameStateStack->Clear();
			RootFrame->Navigate(LogoPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_MAIN_MENU:
			RootFrame->BackStack->Clear();
			_gameStateStack->Clear();
			RootFrame->Navigate(MenuPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_PLAYING:
			RootFrame->Navigate(GamePlayPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_PAUSED:
			break;
		case ProjectAsahi::Entities::GameState::GS_SAVE:
			RootFrame->Navigate(SavePage::typeid, ProjectAsahi::Entities::SavePageType::SPT_SAVE);
			break;
		case ProjectAsahi::Entities::GameState::GS_LOAD:
			RootFrame->Navigate(SavePage::typeid, ProjectAsahi::Entities::SavePageType::SPT_LOAD);
			break;
		case ProjectAsahi::Entities::GameState::GS_BACKLOG:
			break;
		}
		_gameStateStack->Append(value);
	}
	else if(CurrentGameState == Entities::GameState::GS_SAVE)
	{
		GoBack();
	}
	else
	{
		switch (value)
		{
		case ProjectAsahi::Entities::GameState::GS_PLAYING:
			if (_gameStateStack->Size > 1)
			{
				for (size_t i = 0; i < _gameStateStack->Size - 2; i++)
				{
					_gameStateStack->RemoveAtEnd();
					RootFrame->BackStack->RemoveAtEnd();
				}
			}
			RootFrame->Navigate(MenuPage::typeid);
			_gameStateStack->Append(value);
			break;
		default:
			break;
		}
	}
}