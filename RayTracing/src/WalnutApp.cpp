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
			Render();
		ImGui::End();

		ImGui::Begin("Viewport");
		viewportWidth = ImGui::GetContentRegionAvail().x;
		viewportHeight = ImGui::GetContentRegionAvail().y;
		if(m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), {(float)viewportWidth, (float)viewportHeight});
		ImGui::End();

	}

	void Render()
	{
		if (!m_Image)
		{
			m_Image = std::make_shared<Walnut::Image>(viewportWidth, viewportHeight, Walnut::ImageFormat::RGBA);
		}

		delete[] m_pImageData;
		m_pImageData = new uint32_t[viewportHeight * viewportWidth];
		for (uint32_t i = 0; i < viewportHeight * viewportWidth; i++)
		{
			m_pImageData[i] = 0xffff00ff;
		}

		std::cout << m_pImageData[0];
		m_Image->SetData(m_pImageData);
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;

	uint32_t viewportWidth;
	uint32_t viewportHeight;
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