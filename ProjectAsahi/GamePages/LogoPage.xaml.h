//
// LogoPage.xaml.h
// LogoPage 类的声明
//

#pragma once
#include "App.xaml.h"
#include "GamePages\LogoPage.g.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LogoPage sealed
	{
	public:
		LogoPage();
		void MediaEnded();

		void MediaDoubleTapped();
	};
}
