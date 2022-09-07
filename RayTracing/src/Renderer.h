#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"

#include "Sphere.h"

class Renderer
{
public:
	Renderer(Sphere *sphere);

	void OnResize(uint32_t width, uint32_t height);
	void Render();
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }


	float m_rayOriginArr[3] = { 0.0f, 0.0f, 1.0f };
private:
	glm::vec4 PerPixel(glm::vec2 coords);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_pImageData = nullptr;
	Sphere* pSphere = nullptr;

	float m_LastRenderTime = 0.0f;

};
