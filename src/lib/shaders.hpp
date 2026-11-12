#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include <sys/types.h>
  
class Shader {

    public:

    uint ID;
        
    Shader(const char* vertexPath, const char* fragmentPath) {

        std::vector<std::string> sources = readFiles(vertexPath, fragmentPath);

        const char* vertexCode = sources[0].c_str();
        const char* fragmentCode = sources[1].c_str();

        std::vector<uint> compiled = compShaders(vertexCode, fragmentCode);

        uint vertex = compiled[0];
        uint fragment = compiled[1];

        createProgram(vertex, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void del() {
        glDeleteProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const std::string &name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setMat4(const std::string &name, glm::mat4 value) const {
        int location = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    private:

    void createProgram (uint vertex, uint fragment) {
        ID = glCreateProgram();

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        glLinkProgram(ID);

        checkError(ID, NULL, "PROGRAM");
    }

    std::vector<uint> compShaders (const char* vertexSource, const char* fragmentSource) {
        uint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        checkError(vertexShader, vertexSource, "VERTEX");

        uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        checkError(fragmentShader, fragmentSource, "FRAGMENT");

        std::vector<uint> res = { vertexShader, fragmentShader };

        return res;
    }

    std::vector<std::string> readFiles (const char* vertexPath, const char* fragmentPath) {

        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vertexFile;
        std::ifstream fragmentFile;

        vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            std::stringstream vertexStream, fragmentStream;

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexFile.close();
            fragmentFile.close();

            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }
        catch(std::ifstream::failure e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        std::vector<std::string> res = { vertexCode, fragmentCode };

        return res;
    }

    void checkError(uint id, const char* source, std::string type) {

        int success;
        char infoLog[512];

        if (type != "PROGRAM") {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(id, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::TYPE::" << type << "\n" << infoLog << "\n";
                if (source) {
                    std::cerr << "source: \"" << source << "\"" << std::endl;
                }
            }
        }
        else {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(id, 512, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINK::TYPE::" << type << "\n" << infoLog << "\n";
                if (source) {
                    std::cerr << "source: \"" << source << "\"" << std::endl;
                }
            }
        }
    }
};
