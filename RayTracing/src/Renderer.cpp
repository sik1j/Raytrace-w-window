#include "Renderer.h"

#if WL_DEBUG
#include <iostream>
#define Log(x) std::cout << x << "\n"
#else
#define Log(x)
#endif

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4 color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

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
	float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			// coord * 2 - 1 sets coord space from: [0,1) to [-1,1)
			// without this, a sphere at the point 0,0,0 appears at the bottom right of the screen 
			// instead of the center
			glm::vec2 coords = { (float)x / m_FinalImage->GetWidth() * aspectRatio, (float)y / m_FinalImage->GetHeight() };
			coords = coords * 2.0f - 1.0f;
			glm::vec4 color = PerPixel(coords);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_pImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
		}
	}

	m_FinalImage->SetData(m_pImageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 coords)
{
	float z = 1.0f;
	/*
	Ray Fromula: at + b; where a,b are vectors; t is a scalar;
	a = ray dirction
	b = ray origin
	t = ray size (sorta)

	Equation of a sphere (origin 0,0,0): x^2 + y^2 + z^2 = r^2
	
	Plug in the x,y components of ray formula into x y of sphere:
	(a_x*t + b_x)^2 + (a_y*t + b_y)^2 + (a_z*t + a_z)^2= r^2
	Solve for t to get the solutions for t
	The solutions for t give the scaling of t required to hit the sphere

	dot(u,v) -> dot product of vectors u,v
	solve using quadratic equation
	t = (-quad_b+/-sqrt(quad_b^2 - 4*quad_a*quad_c))/2quad_a | quad_ prefix used to differentaite from vectors a,b
	t = (-(dot(a,b))(+/-)sqrt((dot(a,b))^2-(dot(a,a))*(dot(b,b)-r^2)))/dot(a,a)

	discriminant = quad_b^2-4*quad_a*quad_c
	discriminant = dot(a,b)^2-dot(a,a)*(dot(b,b)-r^2)
	discriminant determines if the ray hits the sphere or not
	discriminant >= 0 ? hit : no hit
	*/


	float radius = 0.2f;

	glm::vec3 a = {coords.x, coords.y, z};
	glm::vec3 b = { 0.0f,0.0f,-1.0f};

	glm::vec3 lightOrigin = { 0.0f,1.0f,0.0f };

	float quad_a = glm::dot(a, a);
	float quad_b = 2.0f * glm::dot(a, b);
	float quad_c = glm::dot(b, b) - radius * radius;

	float discriminant = quad_b * quad_b - 4.0f * quad_a * quad_c;
	if (discriminant < 0.0f)
		return glm::vec4(0,0,0,1);

	float t0 = (-quad_b + glm::sqrt(quad_b * quad_b - 4.0f * quad_a * quad_c)) / 2.0f * quad_a;
	float t1 = (-quad_b - glm::sqrt(quad_b * quad_b - 4.0f * quad_a * quad_c)) / 2.0f * quad_a;

	return glm::vec4(1,0,1,1);
	// return 0xff000000 + (uint32_t)(0xff * coords.y) * 0x100 + (uint32_t)(0xff * coords.x);
}

