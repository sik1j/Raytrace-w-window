#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"

class Renderer
{
public:
	void OnResize(uint32_t width, uint32_t height);
	void Render();
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	uint32_t PerPixel(glm::vec2 coords);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_pImageData = nullptr;

	float m_LastRenderTime = 0.0f;
};
