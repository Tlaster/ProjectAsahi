//
// LogoPage.xaml.cpp
// LogoPage 类的实现
//

#include "pch.h"
#include "LogoPage.xaml.h"

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

LogoPage::LogoPage()
{
	InitializeComponent();
}

void ProjectAsahi::LogoPage::MediaEnded()
{
	LogoElement->IsFullWindow = false;

	App::CurrentGameState = ProjectAsahi::Entities::GameState::GS_MAIN_MENU;
}

void ProjectAsahi::LogoPage::MediaDoubleTapped()
{
	LogoElement->Stop();
	LogoElement->IsFullWindow = false;
	App::CurrentGameState = ProjectAsahi::Entities::GameState::GS_MAIN_MENU;
}