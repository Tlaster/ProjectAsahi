//
// App.xaml.h
// App 类的声明。
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"
#include "Entities\GameState.h"

namespace ProjectAsahi
{
	/// <summary>
	/// 提供特定于应用程序的行为，以补充默认的应用程序类。
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

		property static Windows::UI::Xaml::Controls::Frame^ RootFrame;
		property static ProjectAsahi::Entities::GameState CurrentGameState
		{
			ProjectAsahi::Entities::GameState get() { return _gameStateStack->GetAt(_gameStateStack->Size - 1); }
			void set(ProjectAsahi::Entities::GameState value) { SetGameState(value); }
		}
		static void GoBack();
	private:
		static Platform::Collections::Vector<ProjectAsahi::Entities::GameState>^ _gameStateStack;
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		DirectXPage^ m_directXPage;
		void OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args);
		static void SetGameState(ProjectAsahi::Entities::GameState value);
	};
}