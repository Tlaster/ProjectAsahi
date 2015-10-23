#pragma once
#include "Common\MediaEngine.h"
#include "Common\Loader.h"
#include "Model\CharaModel.h"
#include "Common\TextRenderer.h"
#include "Model\BackLogModel.h"
#include "Model\SettingModel.h"
#include "Model\SelectionModel.h"

namespace ProjectAsahi
{
	namespace Common
	{
		ref class Interpreter sealed
		{
		internal:
			Interpreter(const std::shared_ptr<DX::DeviceResources>& deviceResources, Platform::String^ path)
			{
				m_deviceResources = deviceResources;
				_currentFilePath = path;
				Init();
				SetDefault();
				LoadResource();
				LoadBlock();
			}
			Interpreter(const std::shared_ptr<DX::DeviceResources>& deviceResources)
			{
				m_deviceResources = deviceResources;
				//auto a = Windows::System::UserProfile::GlobalizationPreferences::Languages->GetAt(0);
				//auto b = Windows::System::UserProfile::GlobalizationPreferences::Languages->GetAt(1);
				//auto c = Windows::System::UserProfile::GlobalizationPreferences::Languages->GetAt(2);
				_currentFilePath = nullptr;
				Init();
				SetDefault();
				LoadResource();
			}
			void Push(ScriptReader::Model::Block^ block);
			void Render();
			void Update(float timeTotal, float timeDelta);
			void Clear();
			void ToNext();
			void ClearAll();
			void OnWindowSizeChanged();
			void SetPathNLoad(Platform::String^ path)
			{
				_isLoaded = false;
				ClearAll();
				_currentFilePath = path;
				LoadBlock();
			}
			void LoadSelection(Platform::String^ path)
			{
				_isLoaded = false;
				Clear();
				_nextFilePath = nullptr;
				_blockPosition = 0;
				_block = nullptr;
				_currentFilePath = path;
				LoadBlock();
			}
			FileManager::Model::SaveModel^ GetSaveModel();
			Platform::Collections::Vector<Model::BackLogModel^>^ GetBackLogList();
			property Platform::Collections::Vector<Model::SelectionModel^>^ SelectionList
			{ 
				Platform::Collections::Vector<Model::SelectionModel^>^ get()
				{ 
					auto list = _selectionList;
					_selectionList = ref new Platform::Collections::Vector<Model::SelectionModel^>();
					return list;
				} 
			};
			void LoadFromSaveModel(FileManager::Model::SaveModel^ item);
			void UpdateRootFrameMargin();

			bool isSelection;
			bool isEnded;
			bool isAuto;

		private:
			void Init();
			void SetDefault();
			void LoadResource();
			void LoadBlock();
			void BackGroundRenderHandler();
			void CharacterRenderHandler();
			void ContentRenderHandler();
			void FaceRenderHandler();
			void CreateWindowSizeDependentResources();

			void PushTABElement(ScriptReader::Model::Element^ element);
			void PushSettingAttribute(ScriptReader::Model::Attribute^ attribute);
			void PushSelectionBlock(ScriptReader::Model::Block ^ block);

			~Interpreter();

			void BackgroundMusicHandler(ScriptReader::Model::Element^ element);
			void BackgroundHandler(ScriptReader::Model::Element^ element);
			void CharacterVectorHandler(ScriptReader::Model::Element^ element);
			void ContentHandler(ScriptReader::Model::Element^ element);
			void ContentHandler(Platform::String^ value);
			void FaceHandler(ScriptReader::Model::Element^ element);
			void ToNextScript();

			std::shared_ptr<DX::DeviceResources> m_deviceResources;

			Loader^ _loader;
			ScriptReader::Reader^ _reader;


			Windows::Foundation::Collections::IVector<ScriptReader::Model::Block^>^ _block;
			Platform::Collections::Vector<Model::BackLogModel^>^ _backLogList;
			Platform::Collections::Vector<Model::SelectionModel^>^ _selectionList;
			int _blockPosition;

			std::wstring _contentValue;
			std::wstring _contentTitle;
			std::wstring _contentShowed;
			int _contentPosition;

			MediaEngine^ _backGroundMusic;
			MediaEngine^ _charaVoice;

			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textForeground;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBorderBrush;
			Microsoft::WRL::ComPtr<ID2D1Effect> m_background;
			Microsoft::WRL::ComPtr<ID2D1Effect> m_face;
			Microsoft::WRL::ComPtr<ID2D1Effect> m_textLayoutBackground;
			Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
			Microsoft::WRL::ComPtr<TextRenderer> m_textRenderer;
			Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;

			std::vector<ProjectAsahi::Model::CharaModel^> m_charaVector;

			Platform::String^ _nextFilePath;

			Platform::String^ _backgroundPath;
			Platform::String^ _bgmPath;
			Platform::String^ _currentFilePath;
			//Platform::String^ _textLayoutBackground;

			Model::SettingModel<Platform::String^>^ _textLayoutBackground;
			Model::SettingModel<bool>^ _isMultipleLanguage;
			Model::SettingModel<float>^ _imageHeight;
			Model::SettingModel<float>^ _imageWidth;
			Model::SettingModel<float>^ _fontSize;
			

			float _imageScale;
			float _contentFontSize;
			float _textLayoutPadding;
			float _positionY;
			float _positionX;
			float _scale;
			float _contentHeight;
			float _contentWidth;
			float _prevTime;
			float _updateTimeSpan;
			float _contentPosition_X;
			float _contentPosition_Y;
			float _autoPlayTimeSpan;
			float _autoPlayPrevTime;
			float _autoPlaySpeed;

			bool _hasVoice;
			bool _hasChara;
			bool _hasBackground;
			bool _hasFace;

			bool _isLoaded;
			bool _isResourceChanged;
			bool _isReadyToDraw;
			bool _isFadeAnimeComplete;

			property bool _isCharaReady
			{
				bool get()
				{
					if (m_charaVector.size() != 0)
					{
						for (auto item : m_charaVector)
						{
							if (item->CharaItem == nullptr)
							{
								return false;
							}
						}
						return true;
					}
					return false;
				}
			}
		};
	}
}


