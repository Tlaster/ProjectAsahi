#include "pch.h"
#include "ProjectAsahiMain.h"
#include "Common\DirectXHelper.h"
#include "GameScreens\GamePlayScreen.h"
#include "GameScreens\MenuScreen.h"
#include "Entities\GameState.h"

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


void ProjectAsahiMain::CheckScreenType()
{
	switch (App::CurrentGameState)
	{
	case GameState::GS_MAIN_MENU:
		_canHandlePointer = true;
		if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_MAIN_MENU)
		{
			_interpreter->SetPathNLoad(L"Data/Script/MenuScript.jtt");
			CurrentGameScreen = ref new MenuScreen(m_deviceResources, _interpreter);
		}
		break;
	case GameState::GS_PLAYING:
	{
		_canHandlePointer = true;
		auto loaditem = FileManager::Manager::LoadItemCache;
		if (loaditem != nullptr)
		{
			_interpreter->LoadFromSaveModel(loaditem);
			CurrentGameScreen = ref new GamePlayScreen(m_deviceResources, _interpreter);
		}
		else if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_PLAYING)
		{
			_interpreter->SetPathNLoad(L"Data/Script/GameScript.jtt");
			CurrentGameScreen = ref new GamePlayScreen(m_deviceResources, _interpreter);
		}
	}
		break;
	case GameState::GS_SAVE:
		FileManager::Manager::SaveItemCache = _interpreter->GetSaveModel();
		_interpreter->isAuto = false;
		_canHandlePointer = false;
		break;
	case GameState::GS_LOAD:
		_interpreter->isAuto = false;
		_canHandlePointer = false;
		break;
	case GameState::GS_BACKLOG:
		_interpreter->isAuto = false;
		_canHandlePointer = false;
		break;
	case GameState::GS_PAUSED:
		break;
	default:
		break;
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
