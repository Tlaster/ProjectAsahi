//
// SelectionPage.xaml.cpp
// SelectionPage 类的实现
//

#include "pch.h"
#include "SelectionPage.xaml.h"
#include "Common\CacheManager.h"

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

SelectionPage::SelectionPage()
{
	InitializeComponent();
}

void ProjectAsahi::SelectionPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^ e)
{
	SelectionList = dynamic_cast<Platform::Collections::Vector<Model::SelectionModel^>^>(e->Parameter);
}

void ProjectAsahi::SelectionPage::ListView_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	//TODO:Finish Selection
	Common::CacheManager::SelectedItem = dynamic_cast<Model::SelectionModel^>(e->ClickedItem);
	App::CurrentGameState = Entities::GameState::GS_PLAYING;
}