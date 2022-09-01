#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <iostream>

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Render Time: %.3f", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
		{
			// ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_viewportWidth, (float)m_viewportHeight });
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight()});
		}
		ImGui::End();

		ImGui::PopStyleVar();

	}

	void Render()
	{
		Walnut::Timer timer;

		if (!m_Image || m_Image->GetHeight() != m_viewportHeight || m_Image->GetWidth() != m_viewportWidth)
		{
			m_Image = std::make_shared<Walnut::Image>(m_viewportWidth, m_viewportHeight, Walnut::ImageFormat::RGBA);
		delete[] m_pImageData;
		m_pImageData = new uint32_t[m_viewportHeight * m_viewportWidth];
		}

		for (uint32_t i = 0; i < m_viewportHeight * m_viewportWidth; i++)
		{
			m_pImageData[i] = Walnut::Random::UInt();
			m_pImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_pImageData);
		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	uint32_t* m_pImageData = nullptr;
	float m_LastRenderTime = 0.0f;

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