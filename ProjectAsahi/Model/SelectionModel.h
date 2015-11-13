#pragma once
namespace ProjectAsahi
{
	namespace Model
	{
		public ref class SelectionModel sealed
		{
		public:
			property Platform::String^ Content;
			property Platform::String^ NextFilePath;
		internal:
			SelectionModel(Platform::String^ content, Platform::String^ nextFilePath)
			{
				Content = content;
				NextFilePath = nextFilePath;
			}
		};
	}
}