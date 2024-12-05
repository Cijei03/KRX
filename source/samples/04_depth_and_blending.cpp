#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <krx.hpp>
#include <memory>
#include <string>
#include <glm/glm.hpp>

constexpr uint32_t wndWidth = 1280;
constexpr uint32_t wndHeight = 720;

std::unique_ptr<krxContext> Context;
std::unique_ptr<krxSwapchain> Swapchain;

std::unique_ptr<krxResourceView> SwapchainBackBufferView;
std::unique_ptr<krxBuffer> TriangleVertexBuffer;
std::unique_ptr<krxResourceView> TriangleVertexBufferView;

std::unique_ptr<krxBuffer> ColorUniformBuffer;
std::unique_ptr<krxResourceView> ColorUniformBufferView;

std::unique_ptr<krxPipelineLayout> PipelineLayout;
std::unique_ptr<krxShaderPipeline> ShaderPipeline;

std::unique_ptr<krxTexture2D> DepthBuffer;
std::unique_ptr<krxResourceView> DepthBufferView;

struct TriangleUniformData
{
	glm::f32vec4 Color;
	glm::f32vec3 Offset;
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* wnd = glfwCreateWindow(wndWidth, wndHeight, "krxSample", nullptr, nullptr);
	krxEnableValidationLayer();

	// Create swapchain and create render view for swapchain buffer.
	Swapchain = std::make_unique<krxSwapchain>(glfwGetX11Display(), glfwGetX11Window(wnd), glm::uvec2(wndWidth, wndHeight));
	SwapchainBackBufferView = std::make_unique<krxResourceView>
	(
		krxViewCreationInfo
		{
			.Resource = Swapchain->get_buffer(),
			.Target = krxTextureTarget::COLOR_RENDER_TARGET_OUTPUT
		}
	);
	// Create depth buffer.
	{
		DepthBuffer = std::make_unique<krxTexture2D>
		(
			krxTextureCreationInfo
			{
				.Format = krxFormat::UINT8_R,
				.Size
				{
					.Width = wndWidth,
					.Height = wndHeight
				}
			}
		);

		DepthBufferView = std::make_unique<krxResourceView>
		(
			krxViewCreationInfo
			{
				.Resource = DepthBuffer.get(),
				.Target = krxTextureTarget::DEPTH_RENDER_TARGET_OUTPUT
			}
		);
	}
	// Create uniform buffer.
	{
		ColorUniformBuffer = std::make_unique<krxBuffer>(sizeof(TriangleUniformData));

		ColorUniformBufferView = std::make_unique<krxResourceView>
		(
			krxViewCreationInfo
			{
				.Resource = ColorUniformBuffer.get(),
				.Target = krxBufferTarget::SHADER_UNIFORM_TARGET
			}
		);
	}
	// Create vertex buffer.
	{
		float Vertices[6] =
		{
			-0.5f, -0.5f,
			0.0f, 0.5f,
			0.5f, -0.5f
		};

		TriangleVertexBuffer = std::make_unique<krxBuffer>(sizeof(Vertices));
		TriangleVertexBuffer->update_buffer(Vertices);

		TriangleVertexBufferView = std::make_unique<krxResourceView>
		(
			krxViewCreationInfo
			{
				.Resource = TriangleVertexBuffer.get(),
				.Target = krxBufferTarget::ATTRIBUTE_TARGET
			}
		);
	}
	// Create pipeline layout.
	PipelineLayout = std::make_unique<krxPipelineLayout>
	(
		std::vector<krxViewInfo>
		{
			krxViewInfo
			{
				.Index = 0,
				.View = SwapchainBackBufferView.get()
			},
			krxViewInfo
			{
				.Index = 0,
				.View = TriangleVertexBufferView.get()
			},
			krxViewInfo
			{
				.Index = 0,
				.View = ColorUniformBufferView.get()
			},
			krxViewInfo
			{
				.Index = 0,
				.View = DepthBufferView.get()
			}
		}
	);
	// Create shaders pipeline.
	ShaderPipeline = std::make_unique<krxShaderPipeline>
	(
		std::vector<krxShaderCreationInfo>
		{
			krxShaderCreationInfo
			{
				.Path = "depth_and_blending_vert.krxsl"
			},
			krxShaderCreationInfo
			{
				.Path = "depth_and_blending_frag.krxsl"
			}
		},
		static_cast<krxShaderStageBitMask>(krxShaderStageBitMask::VERTEX_SHADER_BIT | krxShaderStageBitMask::FRAGMENT_SHADER_BIT)
	);

	Context = std::make_unique<krxContext>();
	Context->Rasterizer.set_primitive_type(krxPrimitiveType::TRIANGLES);
	Context->Rasterizer.set_viewport(glm::uvec2(0), glm::uvec2(wndWidth, wndHeight));
	Context->Rasterizer.enable_feature(krxRasterizerFeature::DEPTH_TESTING);
	Context->enable_blending();

	Context->bind_pipeline_layout(PipelineLayout.get());
	Context->bind_shader_pipeline(ShaderPipeline.get());

	uint32_t Frames = 0;

	while (!glfwWindowShouldClose(wnd))
	{
		if (glfwGetTime() > 1.0)
		{
			glfwSetTime(0.0);
			glfwSetWindowTitle(wnd, (std::string("krxSample 04 - ") + std::to_string(Frames) + std::string("FPS")).data());
			Frames = 0;
		}
		glfwPollEvents();

		Context->clear_depth_target();
		Context->fast_clear_color_targets();
		// First triangle
		{
			TriangleUniformData Data
			{
				.Color = glm::f32vec4(0.0f, 1.0f, 0.4f, 1.0f),
				.Offset = glm::f32vec3(-0.25f, 0.0f, 0.5f)
			};
			
			ColorUniformBuffer->update_buffer(&Data);
			Context->draw(0, 3);
		}
		// Second triangle
		{
			TriangleUniformData Data
			{
				.Color = glm::f32vec4(0.7f, 1.0f, 1.0f, 1.0f),
				.Offset = glm::f32vec3(0.25f, 0.0f, 0.5f)
			};
			
			ColorUniformBuffer->update_buffer(&Data);
			Context->draw(0, 3);
		}
		// Third triangle
		{
			TriangleUniformData Data
			{
				.Color = glm::f32vec4(1.0f, 0.3f, 0.0f, 0.8f),
				.Offset = glm::f32vec3(0.0f, 0.250f, 0.25f)
			};
			
			ColorUniformBuffer->update_buffer(&Data);
			Context->draw(0, 3);
		}

		Swapchain->present();
		Frames++;
	}

	// Deallocation, only swapchain needs this before closing windows. Other KRX resources will be destroyed automatically by RAII.
	SwapchainBackBufferView.reset();
	Swapchain.reset();

	glfwTerminate();
}