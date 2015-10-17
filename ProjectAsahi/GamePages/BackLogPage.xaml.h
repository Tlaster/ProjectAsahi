//
// BackLogPage.xaml.h
// BackLogPage 类的声明
//

#pragma once

#include "GamePages\BackLogPage.g.h"
#include "Model\BackLogModel.h"
#include "Common\MediaEngine.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BackLogPage sealed
	{
	public:
		BackLogPage();
		property Windows::Foundation::Collections::IObservableVector<Model::BackLogModel^>^ BackLogList;
		void Back();
		void ListItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		Common::MediaEngine^ _voice;
	};
}
