//
// SavePage.xaml.cpp
// SavePage 类的实现
//

#include "pch.h"
#include "SavePage.xaml.h"
#include <ppltasks.h>
#include "Entities\GameState.h"
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
using namespace Windows::UI::Popups;
using namespace concurrency;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上提供

SavePage::SavePage()
{
	//SaveVM = ref new FileManager::ViewModel::SaveViewModel();
	SaveList = ref new Platform::Collections::Vector<FileManager::Model::SaveModel^>();
	LoadSaveList();
	InitializeComponent();
}

void SavePage::BackClick()
{
	App::CurrentGameState = Entities::GameState::GS_MAIN_MENU;
}

void ProjectAsahi::SavePage::NewSave()
{
	auto item = Common::CacheManager::SaveItemCache;
	create_task(FileManager::Manager::Save(item)).then([&]()
	{
		MessageDialog^ dialog = ref new MessageDialog(L"save successfully");
		create_task(dialog->ShowAsync());
		LoadSaveList();
	});
}

void ProjectAsahi::SavePage::Load()
{
	auto clickitem = dynamic_cast<FileManager::Model::SaveModel^>(SaveGridView->SelectedItem);
	Common::CacheManager::LoadItemCache = clickitem;
	App::CurrentGameState = Entities::GameState::GS_PLAYING;
}

void ProjectAsahi::SavePage::DeleteClick()
{
	auto clickitem = dynamic_cast<FileManager::Model::SaveModel^>(SaveGridView->SelectedItem);
	create_task(FileManager::Manager::Delete(clickitem)).then([&]()
	{
		MessageDialog^ dialog2 = ref new MessageDialog(L"delete successfully");
		create_task(dialog2->ShowAsync());
		LoadSaveList();
	});
}

void ProjectAsahi::SavePage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	_type = safe_cast<Entities::SavePageType>(e->Parameter);
}


void SavePage::SaveCommand(IUICommand^ command) 
{
	auto clickitem = dynamic_cast<FileManager::Model::SaveModel^>(SaveGridView->SelectedItem);
	auto item = Common::CacheManager::SaveItemCache;
	item->SaveIndex = clickitem->SaveIndex;
	create_task(FileManager::Manager::Save(item)).then([&]()
	{
		MessageDialog^ dialog2 = ref new MessageDialog(L"save successfully");
		create_task(dialog2->ShowAsync());
		LoadSaveList();
	});
}

void ProjectAsahi::SavePage::Save()
{
	MessageDialog^ dialog = ref new MessageDialog(L"Replace this save data?");
	auto command = ref new UICommand("Yes", ref new UICommandInvokedHandler(this, &SavePage::SaveCommand));
	auto command2 = ref new UICommand("No");
	dialog->Commands->Append(command);
	dialog->Commands->Append(command2);
	create_task(dialog->ShowAsync());

}

void ProjectAsahi::SavePage::LoadSaveList()
{
	create_task(FileManager::Manager::LoadSaveList()).then([&](Windows::Foundation::Collections::IVector<FileManager::Model::SaveModel^>^ collection)
	{
		if (collection != nullptr && collection->Size != 0)
		{
			SaveList->Clear();
			for (FileManager::Model::SaveModel^ item : collection)
			{
				SaveList->Append(item);
			}
		}
	});
}
