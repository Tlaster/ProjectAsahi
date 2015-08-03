﻿//
// App.xaml.h
// App 类的声明。
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"

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

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		DirectXPage^ m_directXPage;
	};
}
