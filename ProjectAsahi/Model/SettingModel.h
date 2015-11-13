#pragma once
namespace ProjectAsahi
{
	namespace Model
	{
		template <class T>
		ref struct SettingModel
		{
		internal:
			property T Value
			{
				T get() { return _value; };
				void set(T value)
				{
					_value = value;
					_isSetted = true;
				}
			}
			property bool IsGlobal;
			property bool IsSetted { bool get() { return _isSetted; }; };
			void SetDefault(T value) { _value = value; };
		private:
			T _value;
			bool _isSetted;
		};
	}
}