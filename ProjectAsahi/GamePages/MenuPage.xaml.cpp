//
// MenuPage.xaml.cpp
// MenuPage 类的实现
//

#include "pch.h"
#include "MenuPage.xaml.h"

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

MenuPage::MenuPage()
{
	InitializeComponent();
}

void ProjectAsahi::MenuPage::Start()
{
	App::CurrentGameState = ProjectAsahi::Entities::GameState::GS_PLAYING;
}

void ProjectAsahi::MenuPage::SaveList()
{
	App::CurrentGameState = Entities::GameState::GS_LOAD;
}

void ProjectAsahi::MenuPage::Exit()
{
	App::Current->Exit();
}
