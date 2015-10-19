#include "pch.h"
#include "ProjectAsahiMain.h"
#include "Common\DirectXHelper.h"
#include "GameScreens\GamePlayScreen.h"
#include "GameScreens\MenuScreen.h"
#include "Entities\GameState.h"
#include "Common\CacheManager.h"
#include "GamePages\MenuPage.xaml.h"
#include "GamePages\LogoPage.xaml.h"
#include "GamePages\GamePlayPage.xaml.h"
#include "GamePages\SavePage.xaml.h"
#include "GamePages\BackLogPage.xaml.h"
#include "GamePages\SelectionPage.xaml.h"

using namespace ProjectAsahi;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace ProjectAsahi::Common;
using namespace Platform;
using namespace ProjectAsahi::Entities;
using namespace ProjectAsahi::Screen;

ProjectAsahiMain::ProjectAsahiMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	m_deviceResources->RegisterDeviceNotify(this);
	_gameStateStack = ref new Platform::Collections::Vector<ProjectAsahi::Entities::GameState>();
	_gameStateStack->Append(Entities::GameState::GS_PAUSED);
	_interpreter = ref new Interpreter(deviceResources);
	_canHandlePointer = true;
}

ProjectAsahiMain::~ProjectAsahiMain()
{
	m_deviceResources->RegisterDeviceNotify(nullptr);
	delete CurrentGameScreen;
}

void ProjectAsahiMain::CreateWindowSizeDependentResources()
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->CreateWindowSizeDependentResources();
	}
	if (App::RootFrame != nullptr)
	{
		_interpreter->UpdateRootFrameMargin();
		//TODO: add scale for RootFrame
	}
}



void ProjectAsahiMain::Update(float timeTotal, float timeDelta)
{

	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->Update(timeTotal, timeDelta);
	}
	CheckScreenType();
}

void ProjectAsahiMain::Render()
{

	auto context = m_deviceResources->GetD3DDeviceContext();

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->Render();
	}

}

void ProjectAsahi::ProjectAsahiMain::GoBack()
{
	_canHandlePointer = true;
	App::RootFrame->GoBack();
	_gameStateStack->RemoveAtEnd();
	App::CurrentGameState = _gameStateStack->GetAt(_gameStateStack->Size - 1);
}


Entities::GameState ProjectAsahi::ProjectAsahiMain::GetCurrentGameState()
{
	return _gameStateStack->GetAt(_gameStateStack->Size - 1);
}


void ProjectAsahiMain::CheckScreenType()
{
	auto prevGameState = _gameStateStack->GetAt(_gameStateStack->Size - 1);
	if (prevGameState == App::CurrentGameState)
	{
		return;
	}
	if (prevGameState != Entities::GameState::GS_SAVE && prevGameState != Entities::GameState::GS_LOAD && prevGameState != Entities::GameState::GS_BACKLOG && prevGameState != GameState::GS_SELECTION)
	{
		switch (App::CurrentGameState)
		{
		case ProjectAsahi::Entities::GameState::GS_LOGO:
			App::RootFrame->BackStack->Clear();
			_gameStateStack->Clear();
			App::RootFrame->Navigate(LogoPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_MAIN_MENU:
			App::RootFrame->BackStack->Clear();
			_gameStateStack->Clear();
			_interpreter->SetPathNLoad(L"Data/Script/MenuScript.jtt");
			_canHandlePointer = true;
			if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_MAIN_MENU)
			{
				CurrentGameScreen = ref new MenuScreen(m_deviceResources, _interpreter);
			}
			App::RootFrame->Navigate(MenuPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_PLAYING:
			_interpreter->SetPathNLoad(L"Data/Script/GameScript.jtt");
			_canHandlePointer = true;
			if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_PLAYING)
			{
				CurrentGameScreen = ref new GamePlayScreen(m_deviceResources, _interpreter);
			}
			App::RootFrame->Navigate(GamePlayPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_PAUSED:
			break;
		case ProjectAsahi::Entities::GameState::GS_SAVE:
			_interpreter->isAuto = false;
			_canHandlePointer = false;
			if (Common::CacheManager::SaveItemCache == nullptr)
			{
				Common::CacheManager::SaveItemCache = _interpreter->GetSaveModel();
			}
			App::RootFrame->Navigate(SavePage::typeid, ProjectAsahi::Entities::SavePageType::SPT_SAVE);
			break;
		case ProjectAsahi::Entities::GameState::GS_LOAD:
			_interpreter->isAuto = false;
			_canHandlePointer = false;
			App::RootFrame->Navigate(SavePage::typeid, ProjectAsahi::Entities::SavePageType::SPT_LOAD);
			break;
		case ProjectAsahi::Entities::GameState::GS_BACKLOG:
			_interpreter->isAuto = false;
			_canHandlePointer = false;
			Common::CacheManager::BackLogList = _interpreter->GetBackLogList();
			App::RootFrame->Navigate(BackLogPage::typeid);
			break;
		case ProjectAsahi::Entities::GameState::GS_SELECTION:
			_interpreter->isAuto = false;
			_canHandlePointer = false;
			App::RootFrame->Navigate(SelectionPage::typeid, _interpreter->SelectionList);
			break;
		}
		_gameStateStack->Append(App::CurrentGameState);
	}
	else if (prevGameState == Entities::GameState::GS_LOAD)
	{
		switch (App::CurrentGameState)
		{
		case ProjectAsahi::Entities::GameState::GS_PLAYING:
		{
			auto item = CacheManager::LoadItemCache;
			if (item != nullptr)
			{

				if (_gameStateStack->Size > 1)
				{
					for (size_t i = 0; i < _gameStateStack->Size - 2; i++)
					{
						_gameStateStack->RemoveAtEnd();	
						App::RootFrame->BackStack->RemoveAtEnd();
					}
				}
				_interpreter->LoadFromSaveModel(item);
				CurrentGameScreen = ref new GamePlayScreen(m_deviceResources, _interpreter);
				App::RootFrame->Navigate(GamePlayPage::typeid);
				_gameStateStack->Append(App::CurrentGameState);
				_canHandlePointer = true;
			}
			else
			{
				GoBack();
			}
		}
		break;
		default:
			GoBack();
			break;
		}
	}
	else if (prevGameState == GameState::GS_SELECTION)
	{
		auto item = Common::CacheManager::SelectedItem;
		_interpreter->LoadSelection(item->NextFilePath);
		_canHandlePointer = true;
		GoBack();
	}
	else
	{
		GoBack();
	}
}

void ProjectAsahiMain::OnDeviceLost()
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->Release();
	}
}

void ProjectAsahiMain::OnDeviceRestored()
{
	CreateWindowSizeDependentResources();
}

void ProjectAsahiMain::OnPointerMoved(Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (CurrentGameScreen != nullptr && _canHandlePointer)
	{
		CurrentGameScreen->OnPointerMoved(sender, e);
	}
}

void ProjectAsahiMain::OnPointerPressed(Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (CurrentGameScreen != nullptr && _canHandlePointer)
	{
		CurrentGameScreen->OnPointerPressed(sender, e);
	}
}

void ProjectAsahiMain::OnPointerReleased(Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (CurrentGameScreen != nullptr && _canHandlePointer)
	{
		if (CurrentGameScreen->ScreenType == GameState::GS_PLAYING && _interpreter->isEnded)
		{
			_interpreter->ClearAll();
			App::CurrentGameState = GameState::GS_MAIN_MENU;
		}
		else
		{
			CurrentGameScreen->OnPointerReleased(sender, e);
		}
	}
}

void ProjectAsahi::ProjectAsahiMain::OnKeyDown(Platform::Object ^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^ e)
{
	CurrentGameScreen->OnKeyDown(sender, e);
}
