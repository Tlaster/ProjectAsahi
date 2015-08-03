#pragma once

#include "Common\DeviceResources.h"
#include "Entities\GameState.h"
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

		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e);

		void Update(float timeTotal, float timeDelta);
		void Render();

		ProjectAsahi::Entities::GameState CurrentGameState;
	private:
		void CheckScreenType();
		ProjectAsahi::Screen::GameScreenBase* CurrentGameScreen;

		std::shared_ptr<DX::DeviceResources> m_deviceResources;


		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;
		ProjectAsahi::Common::Interpreter^ _interpreter;

	};
}