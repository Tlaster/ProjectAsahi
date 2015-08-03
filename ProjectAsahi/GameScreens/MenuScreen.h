#pragma once

#include "pch.h"
#include "GameScreens\GameScreenBase.h"
#include "Common\Interpreter.h"

namespace ProjectAsahi
{
	namespace Screen
	{
		class MenuScreen final :
			public GameScreenBase
		{
		public:
			MenuScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, ProjectAsahi::Common::Interpreter^ interpreter);
			~MenuScreen()
			{
				GameScreenBase::Release();
			}

		};
	}
}


