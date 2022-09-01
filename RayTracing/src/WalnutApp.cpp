#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <iostream>

#include "Walnut/Image.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
		{
			// ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_viewportWidth, (float)m_viewportHeight });
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight()});
		}
		ImGui::End();

	}

	void Render()
	{
		std::cout << "rendered with W: " << m_viewportWidth << " and H: " << m_viewportHeight << "\n";
		if (!m_Image || m_Image->GetHeight() != m_viewportHeight || m_Image->GetWidth() != m_viewportWidth)
		{
			m_Image = std::make_shared<Walnut::Image>(m_viewportWidth, m_viewportHeight, Walnut::ImageFormat::RGBA);
		delete[] m_pImageData;
		m_pImageData = new uint32_t[m_viewportHeight * m_viewportWidth];
		}

		for (uint32_t i = 0; i < m_viewportHeight * m_viewportWidth; i++)
		{
			if(i < (m_viewportHeight * m_viewportWidth) / 4 )
			{ 
				m_pImageData[i] = 0xffff00ff;
			}
			else if (i < (2*(m_viewportHeight * m_viewportWidth)/4))
			{
			}
			else
			{ 
				m_pImageData[i] = 0xffff0000;
			}
		}

		m_Image->SetData(m_pImageData);
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	uint32_t* m_pImageData = nullptr;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}