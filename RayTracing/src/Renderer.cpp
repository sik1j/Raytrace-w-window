#include "Renderer.h"
#include <iostream>

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if(m_FinalImage)
	{ 
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{

		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_pImageData;
	m_pImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			m_pImageData[y * m_FinalImage->GetWidth() + x] = PerPixel({ (float)x / m_FinalImage->GetWidth(), (float)y / m_FinalImage->GetHeight() });
		}
	}

	m_FinalImage->SetData(m_pImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coords)
{
	return 0xffff00ff;
}

