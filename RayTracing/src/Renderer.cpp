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

Renderer::Renderer(Sphere* sphere)
{
	pSphere = sphere;
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
	/*
	Ray Fromula: at + b; where a,b are vectors; t is a scalar;
	a = ray dirction
	b = ray origin
	t = ray size (sorta)

	Equation of a sphere (origin o_x,o_y,o_z): (x-o_x)^2 + (y-o_y)^2 + (z-o_z)^2 = r^2
	
	Plug in the x,y components of ray formula into x y of sphere:
	=>	(a_x*t + b_x - o_x)^2 + (a_y*t + b_y - o_y)^2 + (a_z*t + a_z - o_z)^2 
		= r^2
	=>	(a_x^2 + a_y^2 + a_z^2)*t^2 
		+ 2*(a_x * (b_x - o_x) + a_y * (b_y - o_y) + a_z * (b_z - o_z))*t 
		+ (b_x - o_x)^2 + (b_y - o_y)^2 + (b_z - o_z)^2 - r^2 
		= 0 
	=>	dot(a,a)*t^2 + 2*dot(a,(b - o)) + dot((b - o),(b - o)) - r^2
		= 0 
		^dot(u,v) is the dot product of two vectors u, v;

	Solve for t using the quadratic formula
	The solutions for t gives the scalar, (t) required to hit the sphere

	quad_a = dot(a,a);
	quad_b = 2*dot(a,(b - o));
	quad_c = dot((b-o),(b-o)) - r^2;
	t = (-quad_b +/- sqrt(quad_b^2 - 4 * quad_a * quad_c))/(2 * quad_a) 
	^<quad_> prefix used to differentaite variables in the quadratic formula from vectors a,b

	discriminant = quad_b^2 -4 * quad_a * quad_c
	discriminant determines if the ray hits the sphere or not
	discriminant >= 0 ? hit : no hit
	*/


	float radius = 0.5f;

	// neg z is forward; a computer graphics convention
	glm::vec3 rayDirection(coords.x, coords.y, -1.0f);
	glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
	glm::vec3 sphereOrigin(pSphere->x, pSphere->y, pSphere->z);

	// prefix quad_ represents parts of the quadratic equation: (-b +/- sqrt(b^2 - 4ac))/(2a)
	float quad_a = glm::dot(rayDirection, rayDirection);
	float quad_b = 2.0f * glm::dot(rayDirection, rayOrigin - sphereOrigin);
	float quad_c = glm::dot(rayOrigin - sphereOrigin, rayOrigin - sphereOrigin) - radius * radius;

	// b^2 - 4ac
	float discriminant = quad_b * quad_b - 4.0f * quad_a * quad_c;
	if (discriminant < 0.0f)
		return glm::vec4(0,0,0,1);

	// (-b +/- sqrt(discriminant))/2a
	float t0 = (-quad_b + glm::sqrt(discriminant)) / ( 2.0f * quad_a );
	// origin of sphere is at (0,0,0) | camera is at (0,0,2) | rays from camera travel in the neg z direction
	// a more neg z means the point is closer to the camera; The closest hit is the most neagtive z value;
	float closestT = (-quad_b - glm::sqrt(discriminant)) / ( 2.0f * quad_a ); 

	glm::vec3 hitPoint = rayDirection * closestT + rayOrigin;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(-1, -1, -1));

	// given two uint vectors: u,v and theta: the angle between them | cos(theta) == dot(u,v);
	float lightIntensity = glm::max(glm::dot(normal, -lightDirection), 0.0f); 

	glm::vec3 sphereColor(1, 0, 1);
	// sphereColor = normal / 2.0f + 0.5f;
	sphereColor *= lightIntensity;
	return glm::vec4(sphereColor, 1);
	// return 0xff000000 + (uint32_t)(0xff * coords.y) * 0x100 + (uint32_t)(0xff * coords.x);
}

