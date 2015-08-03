#include "pch.h"
#include "Interpreter.h"
#include <ppltasks.h>

using namespace ProjectAsahi::Common;
using namespace std;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace ProjectAsahi::Model;
using namespace Concurrency;
using namespace ScriptReader::Model;
using namespace ScriptReader;



void ProjectAsahi::Common::Interpreter::Push(ScriptReader::Model::Block ^ block)
{
	switch (block->BlockType)
	{
	case BlockTypes::TAB:
		for (auto element = block->ElementList; element != nullptr; element = element->Next)
		{
			PushTABElement(element);
		}
		break;
	case BlockTypes::SELECTION:
		break;
	case BlockTypes::SETTING:
		for (auto element = block->ElementList; element != nullptr; element = element->Next)
		{
			PushSettingElement(element);
		}
		break;
	default:
		break;
	}
}

void ProjectAsahi::Common::Interpreter::Render()
{
	auto d2dContext = m_deviceResources->GetD2DDeviceContext();
	d2dContext->BeginDraw();
	m_deviceResources->GetD2DDeviceContext()->SetTransform(m_deviceResources->GetOrientationTransform2D());
	d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	if (_isLoaded)
	{
		BackGroundRenderHandler();
		CharacterRenderHandler();
		ContentRenderHandler();
		FaceRenderHandler();
	}
	DX::ThrowIfFailed(d2dContext->EndDraw());
}

void ProjectAsahi::Common::Interpreter::Update(float timeTotal, float timeDelta)
{
	if (_prevTime + _updateTimeSpan <= timeTotal)
	{
		if (_contentPosition < _contentValue.length())
		{
			_contentShowed += _contentValue[_contentPosition++];
			DX::ThrowIfFailed(
				m_deviceResources->GetDWriteFactory()->CreateTextLayout(
					_contentShowed.c_str(),
					_contentShowed.length(),
					m_pTextFormat.Get(),
					_contentWidth,
					_contentHeight,
					&m_textLayout
					)
				);
		}
		_prevTime = timeTotal;
	}
	if (_isAuto)
	{
		if (_hasVoice)
		{
			if (_charaVoice->IsEnded())
			{
				_autoPlayPrevTime = timeTotal;
				ToNext();
			}
		}
		else if (_autoPlayPrevTime + _autoPlayTimeSpan <= timeTotal)
		{
			_autoPlayPrevTime = timeTotal;
			ToNext();
		}
	}
}

void ProjectAsahi::Common::Interpreter::Clear()
{
	_contentValue.clear();
	_contentTitle.clear();
	_contentShowed.clear();
	m_face.Reset();
	_charaVoice->Stop();
}

void ProjectAsahi::Common::Interpreter::ToNext()
{
	if (_contentPosition < _contentValue.length())
	{
		_contentPosition = _contentValue.length();
		_contentShowed = _contentTitle + _contentValue;
		DX::ThrowIfFailed(
			m_deviceResources->GetDWriteFactory()->CreateTextLayout(
				_contentShowed.c_str(),
				_contentShowed.length(),
				m_pTextFormat.Get(),
				_contentWidth,
				_contentHeight,
				&m_textLayout
				)
			);
	}
	else
	{
		if (_blockPosition < _block->Length)
		{
			Clear();
			Push(_block[_blockPosition++]);
			OnWindowSizeChanged();
		}
		isEnded = _blockPosition == _block->Length;
	}
}

void ProjectAsahi::Common::Interpreter::ClearAll()
{
	Clear();
	m_background = nullptr;
	m_face = nullptr;
	for (size_t i = 0; i < m_charaVector.size(); i++)
	{
		m_charaVector[i] = nullptr;
	}
	m_charaVector.clear();
	_contentShowed.clear();
	_contentTitle.clear();
	_contentValue.clear();
}

void ProjectAsahi::Common::Interpreter::OnWindowSizeChanged()
{
	CreateWindowSizeDependentResources();
	if (_isLoaded)
	{
		if (m_background != nullptr)
		{
			DX::ThrowIfFailed(m_background->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, D2D1::Vector2F(_imageScale, _imageScale)));
		}
		if (m_face != nullptr)
		{
			DX::ThrowIfFailed(m_face->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, D2D1::Vector2F(_imageScale, _imageScale)));
		}
		for (size_t i = 0; i < m_charaVector.size(); i++)
		{
			DX::ThrowIfFailed(m_charaVector[i]->CharaItem->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, D2D1::Vector2F(_imageScale, _imageScale)));
		}
		if (!_contentShowed.empty())
		{
			DX::ThrowIfFailed(m_textLayout->SetMaxHeight(_contentHeight));
			DX::ThrowIfFailed(m_textLayout->SetMaxWidth(_contentWidth));
			DWRITE_TEXT_RANGE textRange;
			textRange.length = _contentShowed.length();
			textRange.startPosition = 0;
			DX::ThrowIfFailed(m_textLayout->SetFontSize(_contentFontSize, textRange));
			DX::ThrowIfFailed(m_textLayoutBackground->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, D2D1::Vector2F(_imageScale, _imageScale)));
		}
	}
}

void ProjectAsahi::Common::Interpreter::LoadResource()
{
	CreateWindowSizeDependentResources();
	_loader = ref new Loader(m_deviceResources->GetD2DDeviceContext(), m_deviceResources->GetDWriteFactory(), m_deviceResources->GetWicImagingFactory());
	_backGroundMusic = ref new MediaEngine();
	_charaVoice = ref new MediaEngine();
	_backGroundMusic->SetVolume(0.2);
	DX::ThrowIfFailed(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
	DX::ThrowIfFailed(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));
	DX::ThrowIfFailed(m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textForeground));
	DX::ThrowIfFailed(m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_textBorderBrush));
	m_textRenderer = new (std::nothrow) TextRenderer(
		m_deviceResources->GetD2DFactory(),
		m_deviceResources->GetD2DDeviceContext(),
		m_textBorderBrush.Get(),
		m_textForeground.Get()
		);
	_updateTimeSpan = 0.01f;
	_isAuto = true;
	_autoPlaySpeed = 0.1f;
}

void ProjectAsahi::Common::Interpreter::LoadBlock()
{
	_reader = ref new Reader(_path);
	create_task(_reader->ReadFile()).then([=]()
	{
		_block = _reader->Block;
		_blockPosition = 0;
		_isLoaded = true;
		ToNext();
	});
}

void ProjectAsahi::Common::Interpreter::BackGroundRenderHandler()
{
	if (m_background != nullptr)
	{
		auto d2dContext = m_deviceResources->GetD2DDeviceContext();
		d2dContext->DrawImage(m_background.Get(), D2D1::Point2F(_positionX, _positionY));
	}
}

void ProjectAsahi::Common::Interpreter::CharacterRenderHandler()
{
	auto d2dContext = m_deviceResources->GetD2DDeviceContext();
	for (size_t i = 0; i < m_charaVector.size(); i++)
	{
		d2dContext->DrawImage
			(
				m_charaVector[i]->CharaItem,
				D2D1::Point2F
				(
					_positionX + m_charaVector[i]->Position_X*1280.f*_scale,
					_positionY + m_charaVector[i]->Position_Y*720.f*_scale
					)
				);
	}
}

void ProjectAsahi::Common::Interpreter::ContentRenderHandler()
{
	if (!_contentShowed.empty())
	{
		auto d2dContext = m_deviceResources->GetD2DDeviceContext();
		d2dContext->DrawImage(m_textLayoutBackground.Get(), D2D1::Point2F(_positionX, _positionY));
		m_textLayout->Draw(
			m_deviceResources->GetD2DDeviceContext(),
			m_textRenderer.Get(),
			_contentPosition_X,
			_contentPosition_Y
			);
	}
}

void ProjectAsahi::Common::Interpreter::FaceRenderHandler()
{
	if (m_face != nullptr)
	{
		auto d2dContext = m_deviceResources->GetD2DDeviceContext();
		d2dContext->DrawImage(m_face.Get(), D2D1::Point2F(_positionX + _textLayoutPadding*_scale, _contentPosition_Y));
	}
}


void ProjectAsahi::Common::Interpreter::CreateWindowSizeDependentResources()
{
	auto logicalSize = m_deviceResources->GetLogicalSize();
	auto outputSize = m_deviceResources->GetOutputSize();
	_textLayoutPadding = 50.f;
	_scale = min((logicalSize.Width / 1280.f), (logicalSize.Height / 720.f));
	_imageScale = min((outputSize.Width / 1280.f), (outputSize.Height / 720.f));
	_positionY = (logicalSize.Height - 720.f*_scale) / 2.f;
	_positionX = (logicalSize.Width - 1280.f*_scale) / 2.f;
	_contentHeight = 720.f*_scale / 3 * 1;
	_contentPosition_X = _positionX + _textLayoutPadding*_scale + _contentHeight;
	_contentPosition_Y = 720.f*_scale / 3 * 2 + _positionY + _textLayoutPadding*_scale / 2;
	_contentWidth = logicalSize.Width - _positionX * 2 - _textLayoutPadding*_scale * 2 - _contentHeight;
	_contentFontSize = _scale == 0.f ? 30.f : 30.f*_scale;
	DX::ThrowIfFailed(m_deviceResources->GetDWriteFactory()->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_contentFontSize,
		L"ja-JP",
		&m_pTextFormat
		));
}

void ProjectAsahi::Common::Interpreter::PushTABElement(Element ^ element)
{
	switch (element->ElementType)
	{
	case ElementTypes::BGM:
		BackgroundMusicHandler(element);
		break;
	case ElementTypes::Chara:
		CharacterVectorHandler(element);
		break;
	case ElementTypes::Background:
		BackgroundHandler(element);
		break;
	case ElementTypes::Face:
		FaceHandler(element);
		break;
	case ElementTypes::Content:
		ContentHandler(element);
		break;
	default:
		break;
	}
}

void ProjectAsahi::Common::Interpreter::PushSettingElement(ScriptReader::Model::Element ^ element)
{
	switch (element->ElementType)
	{
	case ElementTypes::NextFile:
		break;
	case ElementTypes::MultipleLanguage:

		break;
	default:
		break;
	}
}

void ProjectAsahi::Common::Interpreter::BackgroundMusicHandler(Element ^ element)
{
	String^ path;
	for (auto att = element->AttributeList; att != nullptr; att = att->Next)
	{
		switch (att->AttributeType)
		{
		case AttributeTypes::Path:
			path = att->AttributeValue;
			break;
		}
	}
	if (path == L"null")
	{
		_backGroundMusic->Stop();
	}
	else
	{
		_backGroundMusic->PlayMusic(path, true);
	}
}

void ProjectAsahi::Common::Interpreter::BackgroundHandler(Element ^ element)
{
	String^ path;
	for (auto att = element->AttributeList; att != nullptr; att = att->Next)
	{
		switch (att->AttributeType)
		{
		case AttributeTypes::Path:
			path = att->AttributeValue;
			break;
		}
	}
	_loader->CreateD2DEffectFromFile(path->Data(), &m_background);
}

void ProjectAsahi::Common::Interpreter::CharacterVectorHandler(Element ^ element)
{
	String^ path;
	float pos_x = 0.0;
	float pos_y = 0.0;
	String^ name;
	String^ method;
	for (auto att = element->AttributeList; att != nullptr; att = att->Next)
	{
		switch (att->AttributeType)
		{
		case ScriptReader::Model::AttributeTypes::Path:
			path = att->AttributeValue;
			break;
		case ScriptReader::Model::AttributeTypes::Position_X:
			pos_x = _wtof(att->AttributeValue->Data()) / 200.f;
			break;
		case ScriptReader::Model::AttributeTypes::Position_Y:
			pos_y = _wtof(att->AttributeValue->Data()) / 200.f;
			break;
		case ScriptReader::Model::AttributeTypes::Name:
			name = att->AttributeValue;
			break;
		case ScriptReader::Model::AttributeTypes::Method:
			method = att->AttributeValue;
			break;
		}
	}
	if (method == L"REMOVE")
	{
		for (size_t i = 0; i < m_charaVector.size(); i++)
		{
			if (m_charaVector[i]->Name == name)
			{
				m_charaVector.erase(m_charaVector.begin() + i);
				break;
			}
		}
	}
	else if (method == L"ADD")
	{
		CharaModel^ chara = ref new CharaModel(name, pos_x, pos_y);
		_loader->CreateD2DEffectFromFile(path->Data(), &chara->CharaItem);
		m_charaVector.push_back(chara);
	}
	else
	{
		for (size_t i = 0; i < m_charaVector.size(); i++)
		{
			if (m_charaVector[i]->Name == name)
			{
				_loader->CreateD2DEffectFromFile(path->Data(), &m_charaVector[i]->CharaItem);
				break;
			}
		}
	}
}

void ProjectAsahi::Common::Interpreter::ContentHandler(ScriptReader::Model::Element ^ element)
{
	String^ voicePath;
	float time = 0.f;
	bool hasVoice = false;
	for (auto att = element->AttributeList; att != nullptr; att = att->Next)
	{
		switch (att->AttributeType)
		{
		case AttributeTypes::Value:
			_contentValue = att->AttributeValue->Data();
			break;
		case AttributeTypes::Voice:
			voicePath = att->AttributeValue;
			hasVoice = true;
			break;
		case AttributeTypes::Time:
			time = _wtof(att->AttributeValue->Data());
			break;
		case AttributeTypes::Title:
			_contentTitle = (att->AttributeValue + L"\n")->Data();
			break;
		}
	}
	_hasVoice = hasVoice;
	if (hasVoice)
	{
		_charaVoice->PlayMusic(voicePath);
	}
	else
	{
		if (time == 0.f)
		{
			time = _contentValue.length() * _autoPlaySpeed;
		}
		_autoPlayTimeSpan = time;
	}
	if (_contentTitle.empty())
	{
		_contentPosition = 1;
		_contentShowed = _contentValue[0];
	}
	else
	{
		_contentPosition = 0;
		_contentShowed = _contentTitle;
	}
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextLayout(
			_contentShowed.c_str(),
			_contentShowed.length(),
			m_pTextFormat.Get(),
			_contentWidth,
			_contentHeight,
			&m_textLayout
			)
		);
	m_textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WHOLE_WORD);

	_loader->CreateD2DEffectFromFile(L"Data\\Image\\System\\TextLayoutBackground.png", &m_textLayoutBackground);
}

void ProjectAsahi::Common::Interpreter::FaceHandler(ScriptReader::Model::Element ^ element)
{
	String^ path;
	for (auto att = element->AttributeList; att != nullptr; att = att->Next)
	{
		switch (att->AttributeType)
		{
		case AttributeTypes::Path:
			path = att->AttributeValue;
			break;
		}
	}
	_loader->CreateD2DEffectFromFile(path->Data(), &m_face);
}

ProjectAsahi::Common::Interpreter::~Interpreter()
{
	m_background.Reset();
	delete _backGroundMusic;
	delete _charaVoice;
	delete _block;
	//for (auto pt : m_charaVector)
	//{
	//	if (nullptr != pt)
	//	{
	//		//delete pt;
	//	}
	//}
	m_textForeground.Reset();
	m_textBorderBrush.Reset();
	m_pTextFormat.Reset();
	m_textRenderer.Reset();
	m_textLayout.Reset();
	m_charaVector.clear();
}
