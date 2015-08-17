#pragma once

#include "pch.h"
#include "GameScreens\GameScreenBase.h"
#include "Common\Interpreter.h"

namespace ProjectAsahi
{
	namespace Screen
	{
		ref class MenuScreen :
			public GameScreenBase
		{
		internal:
			MenuScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, ProjectAsahi::Common::Interpreter^ interpreter);
		private:
			//~MenuScreen()
			//{
			//	GameScreenBase::Release();
			//}

		};
	}
}


