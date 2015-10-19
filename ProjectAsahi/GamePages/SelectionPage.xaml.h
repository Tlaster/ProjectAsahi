//
// SelectionPage.xaml.h
// SelectionPage 类的声明
//

#pragma once

#include "GamePages\SelectionPage.g.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SelectionPage sealed
	{
	public:
		SelectionPage();
		property Windows::Foundation::Collections::IObservableVector<Model::SelectionModel^>^ SelectionList;
	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void ListView_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
	};
}
