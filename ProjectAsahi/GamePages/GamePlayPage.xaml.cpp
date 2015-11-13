//
// GamePlayPage.xaml.cpp
// GamePlayPage 类的实现
//

#include "pch.h"
#include "GamePlayPage.xaml.h"

using namespace ProjectAsahi;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上提供

GamePlayPage::GamePlayPage()
{
	InitializeComponent();
}

void ProjectAsahi::GamePlayPage::Save()
{
	App::CurrentGameState = (Entities::GameState::GS_SAVE);
}

void ProjectAsahi::GamePlayPage::Load()
{
	App::CurrentGameState = (Entities::GameState::GS_LOAD);
}

void ProjectAsahi::GamePlayPage::Back()
{
	App::CurrentGameState = (Entities::GameState::GS_MAIN_MENU);
}

void ProjectAsahi::GamePlayPage::BackLog()
{
	App::CurrentGameState = (Entities::GameState::GS_BACKLOG);
}