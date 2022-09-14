#include "shader.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>

using namespace Graphics::Render;

static std::string readFile(const std::string& filepath)
{
    std::string content;

    std::ifstream stream(filepath, std::ios::in);

    if (!stream.is_open())
    {
        Util::Log::Warn(std::format("Could not read file {}. File does not exist.", filepath));
        return "";
    }

    std::string line = "";
    while (!stream.eof())
    {
        std::getline(stream, line);
        content.append(line + "\n");
    }

    stream.close();
    return content;
}

uint32_t CompileProgram(const std::string& vert_src, const std::string& frag_src)
{
    uint32_t m_Program;

    uint32_t vert_shader = glCreateShader(GL_VERTEX_SHADER);
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* c_vert_src = vert_src.c_str();
    const char* c_frag_src = frag_src.c_str();

    int32_t result = GL_FALSE;
    int32_t log_length;

    GLchar* err_buff;

    glShaderSource(vert_shader, 1, &c_vert_src, 0);
    glCompileShader(vert_shader);

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        err_buff = new GLchar[log_length];
        glGetShaderInfoLog(vert_shader, log_length, 0, err_buff);
        Util::Log::Error(std::format("Vertex shader error buffer: {}.", err_buff));
        log_length = 0;
        delete[] err_buff;
    }

    glShaderSource(frag_shader, 1, &c_frag_src, 0);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        err_buff = new GLchar[log_length];
        glGetShaderInfoLog(frag_shader, log_length, 0, err_buff);
        Util::Log::Error(std::format("Fragment shader error buffer: {}.", err_buff));
        log_length = 0;
        delete[] err_buff;
    }

    m_Program = glCreateProgram();
    glAttachShader(m_Program, vert_shader);
    glAttachShader(m_Program, frag_shader);
    glLinkProgram(m_Program);

    glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
    glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        err_buff = new GLchar[log_length];
        glGetProgramInfoLog(m_Program, log_length, 0, err_buff);
        Util::Log::Error(std::format("Program link error buffer: {}.", err_buff));
        log_length = 0;
        delete[] err_buff;
        // exit(1);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    // Validate program.
    glValidateProgram(m_Program);
    glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &result);
    if (result != GL_TRUE)
        Util::Log::Error(std::format("Program failed to validate."));

    glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0)
    {
        err_buff = new GLchar[log_length];
        glGetProgramInfoLog(m_Program, log_length, 0, err_buff);
        Util::Log::Error(std::format("Program validate error buffer: {}.", err_buff));
        log_length = 0;
        delete[] err_buff;
        // exit(1);
    }  
    glUseProgram(m_Program);

    return m_Program;
}

Shader::Shader(const std::string& name, const std::string& path)
{
    std::filesystem::path vert_path(path);
    std::filesystem::path frag_path(path);
    vert_path /= name + ".vert";
    frag_path   /= name + ".frag";

    std::string vert_src = readFile(vert_path.string());
    std::string frag_src = readFile(frag_path.string());

    Util::Log::Info(std::format("Compiling {} shader program.", name));
    m_Program = CompileProgram(vert_src, frag_src);
}

Shader::Shader(const std::string& name, const std::string& vert_src, const std::string& frag_src)
:m_Name(name)
{
    Util::Log::Info(std::format("Compiling {} shader program.", name));
    m_Program = CompileProgram(vert_src, frag_src);
}

Shader::~Shader()
{
    
}

void Shader::bind() const
{
    glUseProgram(m_Program);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setInt(const std::string& name, int32_t value)
{
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setIntArray(const std::string& name, int32_t* values, uint32_t count)
{
    glUniform1iv(glGetUniformLocation(m_Program, name.c_str()), count, values);
}

void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, const glm::vec2& value)
{
    glUniform2f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y);
}

void Shader::setFloat3(const std::string& name, const glm::vec3& value)
{
    glUniform3f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y, value.z);
}

void Shader::setFloat4(const std::string& name, const glm::vec4& value)
{
    glUniform4f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y, value.y, value.w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value)
{
    GLint location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

const std::string& Shader::getName() const
{
    return m_Name;
}