#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include <stdio.h>

using mat4x4_t = float[16];

constexpr void mat4x4_ortho(mat4x4_t out, float left, float right, float bottom, float top, float znear, float zfar) noexcept {
	#define T(a, b) (a * 4 + b)

	out[T(0,0)] = 2.0f / (right - left);
	out[T(0,1)] = 0.0f;
	out[T(0,2)] = 0.0f;
	out[T(0,3)] = 0.0f;

	out[T(1,1)] = 2.0f / (top - bottom);
	out[T(1,0)] = 0.0f;
	out[T(1,2)] = 0.0f;
	out[T(1,3)] = 0.0f;

	out[T(2,2)] = -2.0f / (zfar - znear);
	out[T(2,0)] = 0.0f;
	out[T(2,1)] = 0.0f;
	out[T(2,3)] = 0.0f;

	out[T(3,0)] = -(right + left) / (right - left);
	out[T(3,1)] = -(top + bottom) / (top - bottom);
	out[T(3,2)] = -(zfar + znear) / (zfar - znear);
	out[T(3,3)] = 1.0f;

	#undef T
}

constexpr std::string_view vertex_shader{ R"glsl(
#version 130

in vec2 i_position;
in vec4 i_color;
out vec4 v_color;

uniform mat4 u_projection_matrix;

void main() {
	v_color = i_color;
	gl_Position = u_projection_matrix * vec4(i_position, 0.0, 1.0);
}
)glsl"
};

constexpr std::string_view fragment_shader{ R"glsl(
#version 130

in vec4 v_color;
out vec4 o_color;

void main() {
	o_color = v_color;
}

)glsl"
};

constexpr auto attrib_position{ 0 };
constexpr auto attrib_color{ 1 };
constexpr int width{ 800 };
constexpr int height{ 600 };

int main() {
	using namespace gl;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	auto window{ SDL_CreateWindow("ogl-sandbox",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	) };
	auto context{ SDL_GL_CreateContext(window) };

	glbinding::initialize(
		reinterpret_cast<glbinding::ProcAddress(*)(const char *)>(SDL_GL_GetProcAddress)
	);

	auto vs{ glCreateShader(GL_VERTEX_SHADER) };
	{
		const GLchar *vertex_source_code{ vertex_shader.data() };
		const auto length{ static_cast<GLint>(vertex_shader.size()) };
		glShaderSource(vs, 1, &vertex_source_code, &length);
		glCompileShader(vs);

		GLint status;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if(status == static_cast<GLint>(GL_FALSE)) {
			fprintf(stderr, "vertex shader compilation failed\n");
			return 1;
		}
	}

	auto fs{ glCreateShader(GL_FRAGMENT_SHADER) };
	{
		const GLchar *fragment_source_code{ fragment_shader.data() };
		const auto length{ static_cast<GLint>(fragment_shader.size()) };
		glShaderSource(fs, 1, &fragment_source_code, &length);
		glCompileShader(fs);

		GLint status;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if(status == static_cast<GLint>(GL_FALSE)) {
			fprintf(stderr, "fragment shader compilation failed\n");
			return 1;
		}
	}

	auto program{ glCreateProgram() };
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glBindAttribLocation(program, attrib_position, "i_position");
	glBindAttribLocation(program, attrib_color, "i_color");
	glLinkProgram(program);

	glUseProgram(program);

	glDisable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.0, 0.0, 0.0);
	glViewport(0, 0, width, height);

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(attrib_position);
	glEnableVertexAttribArray(attrib_color);

	glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(4 * sizeof(float)));

	const GLfloat g_vertex_buffer_data[] = {
	/*  R, G, B, A, X, Y  */
		1, 0, 0, 1, 0, 0,
		0, 1, 0, 1, width, 0,
		0, 0, 1, 1, width, height,

		1, 0, 0, 1, 0, 0,
		0, 0, 1, 1, width, height,
		1, 1, 1, 1, 0, height
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	mat4x4_t projection_matrix;
	mat4x4_ortho(projection_matrix, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1, GL_FALSE, projection_matrix);

	bool running{ true };
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_ESCAPE) running = false;
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
