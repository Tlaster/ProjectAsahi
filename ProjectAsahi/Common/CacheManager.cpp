#include "pch.h"
#include "CacheManager.h"

using namespace ProjectAsahi::Common;


FileManager::Model::SaveModel^ CacheManager::_saveItemCache;
FileManager::Model::SaveModel^ CacheManager::_loadItemCache;
Platform::Collections::Vector<ProjectAsahi::Model::BackLogModel^>^ CacheManager::_backLogList;
ProjectAsahi::Model::SelectionModel^ CacheManager::_selectedItem;