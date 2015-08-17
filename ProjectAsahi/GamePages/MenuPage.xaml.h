//
// MenuPage.xaml.h
// MenuPage 类的声明
//

#pragma once
#include "App.xaml.h"
#include "GamePages\MenuPage.g.h"
#include "DirectXPage.xaml.h"
#include "Entities\GameState.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MenuPage sealed
	{
	public:
		MenuPage();
		void Start();
		void SaveList();
		void Exit();
	};
}
