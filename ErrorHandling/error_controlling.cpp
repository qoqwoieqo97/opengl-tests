#include "error_controlling.h"

bool Err:: restart_gl_log()
{
    remove(GL_LOG_FILE);
    std::fstream file(GL_LOG_FILE, std::ios_base::out | std::ios_base::app);
    if (!file.is_open())
    {
        std::cout << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for writing" << std::endl;
        return false;
    }

    file.close();
    return true;
}

bool Err::gl_log(std::string message)
{
    std::fstream file(GL_LOG_FILE, std::ios_base::out | std::ios_base::app);
    if (!file.is_open())
    {
        std::cout << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for writing" << std::endl;
        return false;
    }
    file << message << std::endl;
    printf((message + "\n").c_str());

    return true;
}

void Err::glfw_error_callback(int error, const char* descp)
{
    gl_log("GLFW ERROR: code " + std::to_string(error) + "msg:" + descp);
}

void Err::log_gl_params() {
    GLenum params[] = {
      GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
      GL_MAX_CUBE_MAP_TEXTURE_SIZE,
      GL_MAX_DRAW_BUFFERS,
      GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
      GL_MAX_TEXTURE_IMAGE_UNITS,
      GL_MAX_TEXTURE_SIZE,
      GL_MAX_VARYING_FLOATS,
      GL_MAX_VERTEX_ATTRIBS,
      GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
      GL_MAX_VERTEX_UNIFORM_COMPONENTS,
      GL_MAX_VIEWPORT_DIMS,
      GL_STEREO,
    };
    std::string names[] = {
      "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
      "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
      "GL_MAX_DRAW_BUFFERS",
      "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
      "GL_MAX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_TEXTURE_SIZE",
      "GL_MAX_VARYING_FLOATS",
      "GL_MAX_VERTEX_ATTRIBS",
      "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
      "GL_MAX_VIEWPORT_DIMS",
      "GL_STEREO",
    };
    gl_log("GL Context Params:\n");
    char msg[256];
    // integers - only works if the order is 0-10 integer return types
    for (int i = 0; i < 10; i++) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        gl_log(names[i] + " " + std::to_string(v));
    }
    // others
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[10], v);
    gl_log(names[10] + " " + std::to_string(v[0]) + " " + std::to_string(v[1]));
    unsigned char s = 0;
    glGetBooleanv(params[11], &s);
    gl_log(names[11] + " " + std::to_string((unsigned int)s));
    gl_log("-----------------------------\n");
}

/* Shader's */

void Err::_print_shader_info_log(GLuint shader_index) {
    int max_length = 2048;
    int actual_length = 0;
    char shader_log[2048];
    glGetShaderInfoLog(shader_index, max_length, &actual_length, shader_log);
    printf("shader info log for GL index %u:\n%s\n", shader_index, shader_log);
}

bool Err::isCompileWorked(unsigned int shader_index)
{
    int params = -1;
    glGetShaderiv(shader_index, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shader_index);
        gl_log("ERROR: GL shader index " + std::to_string(shader_index) + " did not compile");
        _print_shader_info_log(shader_index);
        return false; // or exit or something
    }
    return true;
}

void Err::_print_programme_info_log(GLuint programme) {
    int max_length = 2048;
    int actual_length = 0;
    char program_log[2048];
    glGetProgramInfoLog(programme, max_length, &actual_length, program_log);
    gl_log("Program info log for GL index " + std::to_string(programme) + std::string(":\n") + program_log);
}

bool Err::isLinkingWorked(unsigned int program_index)
{
    int params = -1;
    glGetProgramiv(program_index, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr,
            "ERROR: could not link shader programme GL index %u\n",
            program_index);
        _print_programme_info_log(program_index);
        return false;
    }
}

const char * Err::GL_type_to_string(GLenum type) {
    switch (type) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
    }
    return "other";
}

void Err::print_all(GLuint programme) {
    gl_log("--------------------\nshader programme " + std::to_string(programme) + " info:");
    int params = -1;
    glGetProgramiv(programme, GL_LINK_STATUS, &params);
    gl_log("GL_LINK_STATUS = " + std::to_string(params));

    glGetProgramiv(programme, GL_ATTACHED_SHADERS, &params);
    gl_log("GL_ATTACHED_SHADERS = " + std::to_string(params));

    glGetProgramiv(programme, GL_ACTIVE_ATTRIBUTES, &params);
    gl_log("GL_ACTIVE_ATTRIBUTES = " + std::to_string(params));
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(
            programme,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char long_name[64];
                std::cout << long_name << name << "[" << j << "]" << std::endl;
                int location = glGetAttribLocation(programme, long_name);
                gl_log(std::to_string(i) + " type:" + GL_type_to_string(type) + " name:" + long_name + " location:" + std::to_string(location));
            }
        }
        else {
            int location = glGetAttribLocation(programme, name);
            gl_log(std::to_string(i) + " type:" + GL_type_to_string(type) + " name:" + name + " location:" + std::to_string(location));
        }
    }

    glGetProgramiv(programme, GL_ACTIVE_UNIFORMS, &params);
    printf("GL_ACTIVE_UNIFORMS = %i\n", params);
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(
            programme,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char long_name[64];
                std::cout << long_name << name << "[" << j << "]" << std::endl;
                int location = glGetUniformLocation(programme, long_name);
                gl_log(std::to_string(i) + " type:" + GL_type_to_string(type) + " name:" + long_name + " location:" + std::to_string(location));

            }
        }
        else {
            int location = glGetUniformLocation(programme, name);
            gl_log(std::to_string(i) + " type:" + GL_type_to_string(type) + " name:" + name + " location:" + std::to_string(location));
        }
    }

    _print_programme_info_log(programme);
}