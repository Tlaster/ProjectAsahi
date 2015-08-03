#include "pch.h"
#include "ProjectAsahiMain.h"
#include "Common\DirectXHelper.h"
#include "GameScreens\GamePlayScreen.h"
#include "GameScreens\MenuScreen.h"

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
	CurrentGameState = GameState::GS_LOGO;
	_interpreter = ref new Interpreter(deviceResources);
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

	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->Render();
	}

}


void ProjectAsahiMain::CheckScreenType()
{
	switch (CurrentGameState)
	{
	case ProjectAsahi::Entities::GameState::GS_MAIN_MENU:
		if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_MAIN_MENU)
		{
			_interpreter->SetPathNLoad(L"Data/Script/MenuScript.jtt");
			CurrentGameScreen = new MenuScreen(m_deviceResources, _interpreter);
		}
		break;
	case ProjectAsahi::Entities::GameState::GS_PLAYING:
		if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_PLAYING)
		{
			_interpreter->SetPathNLoad(L"Data/Script/GameScript.jtt");
			CurrentGameScreen = new GamePlayScreen(m_deviceResources, _interpreter);
		}
		break;
	case ProjectAsahi::Entities::GameState::GS_PAUSED:
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

void ProjectAsahiMain::OnPointerMoved(Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->OnPointerMoved(sender, e);
	}
}

void ProjectAsahiMain::OnPointerPressed(Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->OnPointerPressed(sender, e);
	}
}

void ProjectAsahiMain::OnPointerReleased(Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	if (CurrentGameScreen != nullptr)
	{
		if (CurrentGameScreen->ScreenType == GameState::GS_PLAYING && _interpreter->isEnded)
		{
			_interpreter->ClearAll();
			CurrentGameState = GameState::GS_MAIN_MENU;
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
