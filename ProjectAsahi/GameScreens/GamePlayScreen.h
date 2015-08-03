#pragma once
#include "GameScreenBase.h"
#include "Common\Interpreter.h"

namespace ProjectAsahi
{
	namespace Screen
	{
		class GamePlayScreen final :
			public GameScreenBase
		{
		public:
			GamePlayScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, ProjectAsahi::Common::Interpreter^ interpreter);
			void Update(float timeTotal, float timeDelta) override;
			void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e) override;
			void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e) override;
			~GamePlayScreen()
			{
				GameScreenBase::Release();
			}

		private:
			bool isNext;
			bool isPressed;
		};

	}
}
