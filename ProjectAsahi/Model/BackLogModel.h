#pragma once

namespace ProjectAsahi
{
	namespace Model
	{
		public ref class BackLogModel sealed
		{
		public:
			BackLogModel(Platform::String^ title, Platform::String^ content, Platform::String^ voicePath)
			{
				Title = title;
				Content = content;
				VoicePath = voicePath;
			}
			property Platform::String^ Title;
			property Platform::String^ Content;
			property Platform::String^ VoicePath;
			property Windows::UI::Xaml::Visibility VoiceVisibility
			{
				Windows::UI::Xaml::Visibility get()
				{
					return VoicePath == nullptr ?
						Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
				}
			}
		};
	}
}