#include "pch.h"
#include "GameScreens\MenuScreen.h"
#include "Common\DirectXHelper.h"

using namespace ProjectAsahi::Screen;
using namespace DirectX;
using namespace ProjectAsahi::Entities;
using namespace ProjectAsahi::Common;


MenuScreen::MenuScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, Interpreter^ interpreter)
{
	m_deviceResources = deviceResources;
	ScreenType = GameState::GS_MAIN_MENU;
	_interpreter = interpreter;
	CreateWindowSizeDependentResources();
}

void ProjectAsahi::Screen::MenuScreen::Update(float timeTotal, float timeDelta)
{
	_interpreter->Update(timeTotal, timeDelta);
}


