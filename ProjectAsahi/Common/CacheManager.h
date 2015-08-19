#pragma once
#include "Model\BackLogModel.h"

namespace ProjectAsahi
{
	namespace Common
	{
		ref class CacheManager sealed
		{
		internal:
			property static FileManager::Model::SaveModel^ SaveItemCache
			{
				FileManager::Model::SaveModel^ get()
				{
					if (_saveItemCache != nullptr)
					{
						auto item = _saveItemCache;
						_saveItemCache = nullptr;
						return item;
					}
					else
					{
						return nullptr;
					}
				}
				void set(FileManager::Model::SaveModel^ item)
				{
					_saveItemCache = item;
				}
			}

			property static FileManager::Model::SaveModel^ LoadItemCache
			{
				FileManager::Model::SaveModel^ get()
				{
					if (_loadItemCache != nullptr)
					{
						auto item = _loadItemCache;
						_loadItemCache = nullptr;
						return item;
					}
					else
					{
						return nullptr;
					}
				}
				void set(FileManager::Model::SaveModel^ item)
				{
					_loadItemCache = item;
				}
			}

			property static Platform::Collections::Vector<Model::BackLogModel^>^ BackLogList
			{
				Platform::Collections::Vector<Model::BackLogModel^>^ get()
				{
					if (_backLogList != nullptr)
					{
						auto list = _backLogList;
						_backLogList = nullptr;
						return list;
					}
					else
					{
						return nullptr;
					}
				}
				void set(Platform::Collections::Vector<Model::BackLogModel^>^ list)
				{
					_backLogList = list;
				}
			}

		private:
			static FileManager::Model::SaveModel^ _saveItemCache;
			static FileManager::Model::SaveModel^ _loadItemCache;
			static Platform::Collections::Vector<Model::BackLogModel^>^ _backLogList;
		};
	}
}

