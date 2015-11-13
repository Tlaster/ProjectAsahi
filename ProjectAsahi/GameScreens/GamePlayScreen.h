#pragma once
#include "GameScreenBase.h"
#include "Common\Interpreter.h"

namespace ProjectAsahi
{
	namespace Screen
	{
		ref class GamePlayScreen :
			public GameScreenBase
		{
		internal:
			GamePlayScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, ProjectAsahi::Common::Interpreter^ interpreter);
			void Update(float timeTotal, float timeDelta) override;
			void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) override;
			void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) override;

		private:
			//~GamePlayScreen()
			//{
			//	GameScreenBase::Release();
			//}

			bool isNext;
			bool isPressed;
		};
	}
}
