//
// SavePage.xaml.h
// SavePage 类的声明
//

#pragma once

#include "GamePages\SavePage.g.h"
#include "Entities\SavePageType.h"


namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SavePage sealed 
	{
	public:
		SavePage();
		property Windows::Foundation::Collections::IObservableVector<FileManager::Model::SaveModel^>^ SaveList;
		void BackClick();
		property Windows::UI::Xaml::Visibility SaveVisibility
		{
			Windows::UI::Xaml::Visibility get()
			{
				return _type == ProjectAsahi::Entities::SavePageType::SPT_SAVE ?
					Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
			}
		}
		property Windows::UI::Xaml::Visibility LoadVisibility
		{
			Windows::UI::Xaml::Visibility get()
			{
				return _type == ProjectAsahi::Entities::SavePageType::SPT_SAVE ?
					Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
			}
		}

		void NewSave();
		void Save();
		void Load();
		void DeleteClick();
	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		ProjectAsahi::Entities::SavePageType _type;
		void SaveCommand(Windows::UI::Popups::IUICommand ^ command);
		void LoadSaveList();
	};
}
