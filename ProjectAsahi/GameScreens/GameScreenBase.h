#pragma once

#include "Entities\GameState.h"
#include "Common\Interpreter.h"


namespace ProjectAsahi
{
	namespace Screen
	{
		class GameScreenBase abstract
		{
		public:
			virtual void Release()
			{
				_interpreter = nullptr;
			}
			virtual void CreateWindowSizeDependentResources() final;
			virtual void Render()
			{
				_interpreter->Render();
			}
			virtual void Update(float timeTotal, float timeDelta) {}

			virtual void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e) {}
			virtual void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e) {}
			virtual void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e) {}
			virtual void OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e) {}

			GameScreenBase() {}
			GameScreenBase(const std::shared_ptr<DX::DeviceResources>& deviceResources, ProjectAsahi::Common::Interpreter^ interpreter)
			{
				m_deviceResources = deviceResources;
				_interpreter = interpreter;
			}
			~GameScreenBase()
			{
				Release();
			};
			ProjectAsahi::Entities::GameState ScreenType;
		protected:
			std::shared_ptr<DX::DeviceResources> m_deviceResources;

			float m_positionY;
			float m_positionX;
			float m_scale;
			ProjectAsahi::Common::Interpreter^ _interpreter;


		};
	}
}


