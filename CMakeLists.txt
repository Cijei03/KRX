cmake_minimum_required(VERSION 3.24)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(krx VERSION 1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

include_directories(
	${CMAKE_SOURCE_DIR}/dependencies
	${CMAKE_SOURCE_DIR}/source/krx/include
)

link_directories(
	${CMAKE_SOURCE_DIR}/dependencies
	${CMAKE_SOURCE_DIR}/bin
)

add_compile_options(-g)

target_sources(${PROJECT_NAME} PRIVATE
	"source/krx/source/ppipeline_triangles.cpp"
	"source/krx/source/context_rasterizer.cpp"
	"source/krx/source/context.cpp"
	"source/krx/source/swapchain.cpp"
	"source/krx/source/texture2d.cpp"
	"source/krx/source/buffer.cpp"
	"source/krx/source/validation_layer.cpp"
	"source/krx/source/pipeline_layout.cpp"
	"source/krx/source/view.cpp"
	"source/krx/source/shader_pipeline.cpp"
)
target_compile_options(${PROJECT_NAME} PRIVATE "-fPIE")

link_libraries(
	"glfw3"
	"krx"
	"X11"
)
set(KRX_SAMPLE0_NAME "01_create_window_with_krx")
add_executable(${KRX_SAMPLE0_NAME})
target_sources(${KRX_SAMPLE0_NAME} PRIVATE "source/samples/01_create_window_with_krx.cpp")
add_dependencies(${KRX_SAMPLE0_NAME} ${PROJECT_NAME})

set(KRX_SAMPLE1_NAME "02_enable_validation_layer")
add_executable(${KRX_SAMPLE1_NAME})
target_sources(${KRX_SAMPLE1_NAME} PRIVATE "source/samples/02_enable_validation_layer.cpp")
add_dependencies(${KRX_SAMPLE1_NAME} ${PROJECT_NAME})

set(KRX_SAMPLE2_NAME "03_hello_triangle")
add_executable(${KRX_SAMPLE2_NAME})
target_sources(${KRX_SAMPLE2_NAME} PRIVATE "source/samples/03_hello_triangle.cpp")
add_dependencies(${KRX_SAMPLE2_NAME} ${PROJECT_NAME})