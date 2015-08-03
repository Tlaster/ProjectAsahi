#pragma once

namespace ProjectAsahi
{
	namespace Model
	{
		ref class CharaModel sealed
		{
		internal:
			CharaModel(Platform::String^ name, float pos_x, float pos_y)
			{
				Name = name;
				Position_X = pos_x;
				Position_Y = pos_y;
				CharaItem = nullptr;
			}
			float Position_X;
			float Position_Y;
			ID2D1Effect * CharaItem;
			Platform::String^ Name;
		private:
			~CharaModel() { CharaItem->Release(); }
		};
	}
}

