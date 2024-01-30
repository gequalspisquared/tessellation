#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "shader.h"

enum ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM,
};

ShaderId create_shader(const char* path, ShaderType shader_type);
ShaderId create_shader_program(ShaderId vertex_shader, ShaderId fragment_shader);
void cleanup_shaders(ShaderId vertex_shader, ShaderId fragment_shader);
void check_compilation_errors(ShaderId shader_id, ShaderType shader_type);

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    ShaderId vertex_shader = create_shader(vertex_path, ShaderType::VERTEX);
    ShaderId fragment_shader = create_shader(fragment_path, ShaderType::FRAGMENT);

    m_id = create_shader_program(vertex_shader, fragment_shader);
    cleanup_shaders(vertex_shader, fragment_shader);
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int>(value));
}

void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

ShaderId create_shader(const char* path, ShaderType shader_type) {
    std::string code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        code = shader_stream.str();
    } catch (std::ifstream::failure& err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " 
                  << err.what() << std::endl;
    }

    const char* shader_code = code.c_str();
    ShaderId shader_id;
    
    if (shader_type == ShaderType::VERTEX) {
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    } else if (shader_type == ShaderType::FRAGMENT) {
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        std::cerr << "ERROR::SHADER::UNKOWN_SHADER_TYPE\n";
        shader_id = 0;
    }

    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);
    check_compilation_errors(shader_id, shader_type);

    return shader_id;
}

ShaderId create_shader_program(ShaderId vertex_shader, ShaderId fragment_shader) {
    ShaderId program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    check_compilation_errors(program_id, ShaderType::PROGRAM);

    return program_id;
}

void cleanup_shaders(ShaderId vertex_shader, ShaderId fragment_shader) {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void check_compilation_errors(ShaderId shader_id, ShaderType shader_type) {
    int success;
    char info_log[1024];

    if (shader_type != ShaderType::PROGRAM) {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader_id, 1024, NULL, info_log);
            std::cerr << "ERROR::SHADER_COMPILATION of type: ";
            if (shader_type == ShaderType::VERTEX) {
                std::cerr << "VERTEX" << "\n" << info_log << "\n\n";
            } else if (shader_type == ShaderType::FRAGMENT) {
                std::cerr << "FRAGMENT" << "\n" << info_log << "\n\n";
            } else {
                std::cerr << "UNKNOWN" << "\n" << info_log << "\n\n";
            }
            std::cerr << "-------------------------------------------------";
        }
    } else {
        glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_id, 1024, NULL, info_log);
            std::cerr << "ERROR::PROGRAM_LINKING\n" << info_log << "\n\n";
            std::cerr << "-------------------------------------------------";
        }
    }
}