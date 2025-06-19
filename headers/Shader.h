#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
private:
	unsigned int ID;
	void checkCompileErrors(unsigned int shader, const std::string &type);

public:
	Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
	~Shader();

	void use() const;
	int getID() const;

	void setUniform(const std::string &name, const glm::vec3 &value) const;
	void setUniform(const std::string &name, float value) const;
	void setUniform(const std::string &name, const glm::mat4 &value) const;
};
