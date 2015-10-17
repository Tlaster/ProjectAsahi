//
// BackLogPage.xaml.cpp
// BackLogPage 类的实现
//

#include "pch.h"
#include "BackLogPage.xaml.h"
#include "Common\CacheManager.h"
#include <ppltasks.h>

using namespace ProjectAsahi;
using namespace concurrency;
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

BackLogPage::BackLogPage()
{
	InitializeComponent();
	BackLogList = Common::CacheManager::BackLogList;
	_voice = ref new Common::MediaEngine();
}

void ProjectAsahi::BackLogPage::Back()
{
	App::CurrentGameState = Entities::GameState::GS_PLAYING;
}

void ProjectAsahi::BackLogPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	create_task(this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([=]()
	{
		BackLogListView->ScrollIntoView(BackLogListView->Items->GetAt(BackLogListView->Items->Size - 1));
	})));
}

void ProjectAsahi::BackLogPage::ListItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	auto clickitem = dynamic_cast<Model::BackLogModel^>(e->ClickedItem);
 	if (clickitem->VoicePath != nullptr)
	{
		_voice->PlayMusic(clickitem->VoicePath);
	}
}