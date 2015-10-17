#pragma once

#include "Common\DeviceResources.h"
#include "GameScreens\GameScreenBase.h"
#include "Common\Interpreter.h"

// 在屏幕上呈现 Direct2D 和 3D 内容。
namespace ProjectAsahi
{
	class ProjectAsahiMain : public DX::IDeviceNotify
	{
	public:
		ProjectAsahiMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~ProjectAsahiMain();
		void CreateWindowSizeDependentResources();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e);

		void Update(float timeTotal, float timeDelta);
		void Render();

		Entities::GameState GetCurrentGameState();


	private:
		void CheckScreenType();
		void GoBack();
		ProjectAsahi::Screen::GameScreenBase^ CurrentGameScreen;

		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		Concurrency::critical_section m_criticalSection;
		ProjectAsahi::Common::Interpreter^ _interpreter;
		Platform::Collections::Vector<ProjectAsahi::Entities::GameState>^ _gameStateStack;

		bool _canHandlePointer;
	};
}