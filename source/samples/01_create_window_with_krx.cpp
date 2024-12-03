#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <krx.hpp>
#include <memory>
#include <string>

constexpr uint32_t wndWidth = 1280;
constexpr uint32_t wndHeight = 720;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* wnd = glfwCreateWindow(wndWidth, wndHeight, "krxSample", nullptr, nullptr);

	std::unique_ptr<krxSwapchain> Swapchain = std::make_unique<krxSwapchain>(glfwGetX11Display(), glfwGetX11Window(wnd), glm::uvec2(wndWidth, wndHeight));

	uint32_t Frames = 0;

	while (!glfwWindowShouldClose(wnd))
	{
		if (glfwGetTime() > 1.0)
		{
			glfwSetTime(0.0);
			glfwSetWindowTitle(wnd, (std::string("krxSample 01 - ") + std::to_string(Frames) + std::string("FPS")).data());
			Frames = 0;
		}

		glfwPollEvents();
		Swapchain->present();
		Frames++;
	}

	glfwTerminate();
}