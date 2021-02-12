#ifndef GL_ERROR_CONTROLLING
#define GL_ERROR_CONTROLLING


#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#define GL_LOG_FILE "gl.log"

class Err
{
public:
    bool static restart_gl_log();

    bool static gl_log(std::string message);

    void static glfw_error_callback(int error, const char* descp);

    void static log_gl_params();
    /* Shader's */

    void static _print_shader_info_log(GLuint shader_index);

    bool static isCompileWorked(unsigned int shader_index);

    void static _print_programme_info_log(GLuint programme);

    bool static isLinkingWorked(unsigned int program_index);

    const char static* GL_type_to_string(GLenum type);

    void static print_all(GLuint programme);
};

#endif