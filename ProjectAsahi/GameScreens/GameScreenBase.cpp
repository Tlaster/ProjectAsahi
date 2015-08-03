#include "pch.h"
#include "GameScreenBase.h"


using namespace ProjectAsahi::Screen;


void GameScreenBase::CreateWindowSizeDependentResources()
{
	//auto renderTargetSize = m_deviceResources->GetOutputSize();
	//m_scale = min((renderTargetSize.Width / 1280.f), (renderTargetSize.Height / 720.f));
	//m_positionY = (renderTargetSize.Height - 720.f*m_scale) / 2.f;
	//m_positionX = (renderTargetSize.Width - 1280.f*m_scale) / 2.f;
	_interpreter->OnWindowSizeChanged();
}


