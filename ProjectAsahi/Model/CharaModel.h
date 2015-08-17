#pragma once

namespace ProjectAsahi
{
	namespace Model
	{
		ref class CharaModel sealed
		{
		internal:
			CharaModel(Platform::String^ filePath, Platform::String^ name, float pos_x, float pos_y,int deep)
			{
				FilePath = filePath;
				Name = name;
				Position_X = pos_x;
				Position_Y = pos_y;
				CharaItem = nullptr;
				Deep = deep;
			}
			float Position_X;
			float Position_Y;
			int Deep;
			ID2D1Effect * CharaItem;
			Platform::String^ Name;
			Platform::String^ FilePath;
		private:
			~CharaModel() { CharaItem->Release(); }
		};
	}
}

