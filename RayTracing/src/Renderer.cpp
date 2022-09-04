#include "Renderer.h"

#if WL_DEBUG
#include <iostream>
#define Log(x) std::cout << x << "\n"
#else
#define Log(x)
#endif

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
			// coord * 2 - 1 sets coord space from: [0,1) to [-1,1)
			// without this, a sphere at the point 0,0,0 appears at the bottom right of the screen 
			// instead of the center
			m_pImageData[y * m_FinalImage->GetWidth() + x] = PerPixel({ ((float)x / m_FinalImage->GetWidth()) * 2 - 1, ((float)y / m_FinalImage->GetHeight()) * 2 - 1 } );
		}
	}

	m_FinalImage->SetData(m_pImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coords)
{
	float z = 1.0f;
	/*
	Ray Fromula: at + b; where a,b are vectors; t is a scalar;
	a = ray dirction
	b = ray origin
	t = ray size (sorta)

	Equation of a sphere (origin 0,0,0): x^2 + y^2 + z^2 = r^2
	
	Plug in the x,y components of ray formula into x y of sphere:
	(a_x*t + b_x)^2 + (a_y*t + b_y)^2 + (a_z*t + a_z)= r^2
	Solve for t to get the solutions for t
	The solutions for t give the scaling of t required to hit the sphere

	dot(u,v) -> dot product of vectors u,v
	t = (-(dot(a,b))(+/-)sqrt((dot(a,b))^2-(dot(a,a))*(dot(b,b)-r^2)))/dot(a,a)

	descriminant = dot(a,b)^2-dot(a,a)*(dot(b,b)-r^2)
	descriminant determines if the ray hits the sphere or not
	descriminant >= 0 ? hit : no hit
	*/
	glm::vec3 a = {coords.x, coords.y, z};
	glm::vec3 b = { 0,0,-1.0f};
	float r = 0.2f;

	float descriminant = glm::dot(a, b) * glm::dot(a, b) - glm::dot(a, a) * (glm::dot(b, b) - r*r);

	if (descriminant >= 0)
		return 0xffff00ff;
	return 0xff000000;

	// return 0xff000000 + (uint32_t)(0xff * coords.y) * 0x100 + (uint32_t)(0xff * coords.x);
}

