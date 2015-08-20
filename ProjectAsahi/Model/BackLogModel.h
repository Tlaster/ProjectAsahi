#pragma once

namespace ProjectAsahi
{
	namespace Model
	{
		public ref class BackLogModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
		{
		public:
			BackLogModel(Platform::String^ title, Platform::String^ content, Platform::String^ voicePath)
			{
				_isPropertyChangedObserved = false;
				Title = title;
				Content = content;
				VoicePath = voicePath;
				auto _scale = min((Windows::UI::Xaml::Window::Current->Bounds.Width / 1280.f), (Windows::UI::Xaml::Window::Current->Bounds.Height / 720.f));
				FontSize = 30.f*_scale;
				Windows::UI::Xaml::Window::Current->SizeChanged += ref new Windows::UI::Xaml::WindowSizeChangedEventHandler(this, &ProjectAsahi::Model::BackLogModel::OnSizeChanged);
				//Windows::UI::Xaml::Window::Current->SizeChanged += ref new Windows::UI::Xaml::WindowSizeChangedEventHandler([&](Platform::Object ^sender, Windows::UI::Core::WindowSizeChangedEventArgs ^e)
				//{
				//	//auto size = Windows::UI::Xaml::Window::Current->Bounds;
				//	auto _scale = min((e->Size.Width / 1280.f), (e->Size.Height / 720.f));
				//	FontSize == 0.f ? 30.f : 30.f*_scale;
				//});
			}
			property Platform::String^ Title;
			property Platform::String^ Content;
			property Platform::String^ VoicePath;
			property double FontSize;
			void OnSizeChanged(Platform::Object ^sender, Windows::UI::Core::WindowSizeChangedEventArgs ^e) 
			{
				auto _scale = min((e->Size.Width / 1280.f), (e->Size.Height / 720.f));
				FontSize = 30.f*_scale;
				BackLogModel::OnPropertyChanged(L"FontSize");
				//it will raise Platform::DisconnectedException when navigate to BackLogPage the second time
				//but it does not matter any more,application will not crash for this excepiton
				//for more:https://msdn.microsoft.com/en-us/library/windows/apps/jj160326.aspx about the HiloPage.cpp
			}

#pragma region INotifyPropertyChanged 
		private:
			bool _isPropertyChangedObserved;
			event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ _privatePropertyChanged;

		protected:
			/// <summary> 
			/// Notifies listeners that a property value has changed. 
			/// </summary> 
			/// <param name="propertyName">Name of the property used to notify listeners.</param> 
			void OnPropertyChanged(Platform::String^ propertyName)
			{
				if (_isPropertyChangedObserved)
				{
					PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
				}
			}

		public:
			virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged
			{
				virtual Windows::Foundation::EventRegistrationToken add(Windows::UI::Xaml::Data::PropertyChangedEventHandler^ e)
				{
					_isPropertyChangedObserved = true;
					return _privatePropertyChanged += e;
				}
				virtual void remove(Windows::Foundation::EventRegistrationToken t)
				{
					_privatePropertyChanged -= t;
				}

			protected:
				virtual void raise(Object^ sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e)
				{
					if (_isPropertyChangedObserved)
					{
						_privatePropertyChanged(sender, e);
					}
				}
			}
#pragma endregion 
		};
	}
}


