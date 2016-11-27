#include "glslprogram.h"

#include <fstream>
using std::ifstream;
using std::ios;
#include <sstream>
using std::ostringstream;
#include <sys/stat.h>



GLSLProgram::GLSLProgram() : handle(0), linked(false)
{
}

bool GLSLProgram::compileShaderFromFile(const char* filename,
                                        GLSLShader::GLSLShaderType type)
{
    if(!fileExists(filename))
    {
        logString = "File not found.";
        return false;
    }
    if(handle<=0)
    {
        handle = glCreateProgram();
        if(handle==0)
        {
            logString = "Unable to create shader program.";
            return false;
        }
    }
    ifstream inFile(filename, ios::in);
    if(!inFile)
    {
        return false;
    }
    ostringstream code;
    while(inFile.good())
    {
        int c = inFile.get();
        if(!inFile.eof())
        {
            code << (char)c;
        }
    }
    inFile.close();
    return compileShaderFromString(code.str(), type);
}

bool GLSLProgram::compileShaderFromString(const string& source,
                                          GLSLShader::GLSLShaderType type)
{
    if(handle<=0)
    {
        handle = glCreateProgram();
        if(handle==0)
        {
            logString = "Unable to create shader program.";
            return false;
        }
    }
    GLuint shaderHandle = 0;
    switch (type)
    {
    case GLSLShader::VERTEX:
        shaderHandle = glCreateShader(GL_VERTEX_SHADER);
        break;
    case GLSLShader::FRAGMENT:
        shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    default:
        return false;
    }
    const char* c_code = source.c_str();
    glShaderSource(shaderHandle, 1, &c_code, NULL);
    glCompileShader(shaderHandle);

    int result;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
    if(GL_FALSE==result)
    {
        int length = 0;
        logString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
        if(length>0)
        {
            char* c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }
        return false;
    }
    else
    {
        glAttachShader(handle, shaderHandle);
        return true;
    }
}

bool GLSLProgram::link()
{
    if(linked)
    {
        return true;
    }
    if(handle<=0)
    {
        return false;
    }
    glLinkProgram(handle);

    int status = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    if(GL_FALSE==status)
    {
        int length = 0;
        logString = "";
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
        if(length>0)
        {
            char* c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }
        return false;
    }
    else
    {
        linked = true;
        return linked;
    }
}

void GLSLProgram::use()
{
    if(handle<=0||(!linked))
    {
        return;
    }
    glUseProgram(handle);
}

string GLSLProgram::log()
{
    return logString;
}

int GLSLProgram::getHandle()
{
    return handle;
}

bool GLSLProgram::isLinked()
{
    return linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char* name)
{
    glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char* name)
{
    glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::setUniform( const char *name, float x, float y, float z)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform3f(loc,x,y,z);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const vec3 & v)
{
    this->setUniform(name,v.x,v.y,v.z);
}

void GLSLProgram::setUniform( const char *name, const vec4 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform4f(loc,v.x,v.y,v.z,v.w);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const vec2 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform2f(loc,v.x,v.y);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const mat4 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const mat3 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, float val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1f(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, int val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, bool val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

int GLSLProgram::getUniformLocation(const char * name )
{
    return glGetUniformLocation(handle, name);
}

bool GLSLProgram::fileExists( const string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}
