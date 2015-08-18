//
// GamePlayPage.xaml.h
// GamePlayPage 类的声明
//

#pragma once
#include "App.xaml.h"
#include "GamePages\GamePlayPage.g.h"
#include "Entities\GameState.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class GamePlayPage sealed
	{
	public:
		GamePlayPage();
		void Save();
		void Load();
	};
}
