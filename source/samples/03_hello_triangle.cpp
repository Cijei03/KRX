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

std::unique_ptr<krxPipelineLayout> PipelineLayout;
std::unique_ptr<krxShaderPipeline> ShaderPipeline;

void FreeResources()
{
	SwapchainBackBufferView.reset();
	Swapchain.reset();
}

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
				.Path = "hello_triangle_vert.krxsl"
			},
			krxShaderCreationInfo
			{
				.Path = "hello_triangle_frag.krxsl"
			}
		},
		static_cast<krxShaderStageBitMask>(krxShaderStageBitMask::VERTEX_SHADER_BIT | krxShaderStageBitMask::FRAGMENT_SHADER_BIT)
	);

	Context = std::make_unique<krxContext>();
	Context->Rasterizer.set_primitive_type(krxPrimitiveType::TRIANGLES);
	Context->Rasterizer.set_viewport(glm::uvec2(0), glm::uvec2(wndWidth, wndHeight));
	Context->bind_pipeline_layout(PipelineLayout.get());
	Context->bind_shader_pipeline(ShaderPipeline.get());

	uint32_t Frames = 0;

	while (!glfwWindowShouldClose(wnd))
	{
		if (glfwGetTime() > 1.0)
		{
			glfwSetTime(0.0);
			glfwSetWindowTitle(wnd, (std::string("krxSample 03 - ") + std::to_string(Frames) + std::string("FPS")).data());
			Frames = 0;
		}
		glfwPollEvents();

		Context->fast_clear_color_targets();
		Context->draw(0, 3);

		Swapchain->present();
		Frames++;
	}

	FreeResources();

	glfwTerminate();
}