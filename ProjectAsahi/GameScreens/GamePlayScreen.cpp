#include "pch.h"
#include "GamePlayScreen.h"
#include "Entities\GameState.h"

using namespace ProjectAsahi::Screen;
using namespace ProjectAsahi::Entities;
using namespace ProjectAsahi::Common;

GamePlayScreen::GamePlayScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, Interpreter^ interpreter)
{
	m_deviceResources = deviceResources;
	ScreenType = GameState::GS_PLAYING;
	_interpreter = interpreter;
	CreateWindowSizeDependentResources();
}

void ProjectAsahi::Screen::GamePlayScreen::Update(float timeTotal, float timeDelta)
{
	if (isNext)
	{
		_interpreter->ToNext();
		isNext = false;
	}
	_interpreter->Update(timeTotal, timeDelta);
}

void ProjectAsahi::Screen::GamePlayScreen::OnPointerPressed(Platform::Object ^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	isPressed = true;
}

void ProjectAsahi::Screen::GamePlayScreen::OnPointerReleased(Platform::Object ^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (isPressed)
	{
		isNext = true;
		isPressed = false;
	}
}