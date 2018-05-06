#ifndef buffer_H
#define buffer_H

#include "shader.hpp"

#include <glad/glad.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>


class Buffer {
  public:
    Buffer();
    Buffer(GLenum target);
    Buffer(Buffer const &b);
    ~Buffer();
    Buffer &operator=(Buffer const &b);
    Buffer &operator=(Buffer &&b);
    operator GLuint();

    template<typename T>
    Buffer(std::vector<T> const &data, GLenum target, GLenum usage) : Buffer() {
      loadData(data, target, usage);
    }

    template<typename T>
    Buffer(std::vector<T> const &data, GLenum target) : Buffer(data, target, GL_STATIC_DRAW) {
    }

    template<typename T>
    Buffer(std::vector<T> const &data) : Buffer(data, GL_ARRAY_BUFFER) {
    }

    template<typename T>
    void loadData(std::vector<T> const &data, GLenum target, GLenum usage) {
      if(this->id == 0) {
        glGenBuffers(1, &this->id);
        Buffer::refCount.insert(std::pair<GLint, unsigned>(this->id, 1));
      }
      glBindBuffer(target, this->id);
      glBufferData(target, data.size() * sizeof(T), &data[0], usage);
      this->target = target;
      this->size = data.size();
    }

    template<typename T>
    void loadData(std::vector<T> const &data, GLenum target) {
      loadData(data, target, GL_STATIC_DRAW);
    }

    template<typename T>
    void loadData(std::vector<T> const &data) {
      loadData(data, this->target);
    }

    void bind(GLenum target) const;
    void bind() const;
    void unbind(GLenum target) const;
    void unbind() const;

    void setAttrib(GLuint attribute, int divisor = 0, bool normalized = false, GLenum type = GL_FLOAT) const;
    void setAttrib(const ShaderProgram &s, const std::string& name, int divisor = 0, bool normalized = false, GLenum type = GL_FLOAT) const;
    void setAttrib(const ShaderProgram &s, const char *name, int divisor = 0, bool normalized = false, GLenum type = GL_FLOAT) const;

    void drawArrays(GLenum mode);
    void drawArrays();
    void drawElements(GLenum mode, GLenum type);
    void drawElements(GLenum mode);
    void drawElements();
    void drawInstanced(int count) const;
    void drawInstanced(GLenum mode, int count) const;
    void drawInstanced(GLenum mode, int count, GLenum type) const;
  protected:
    void del();

    GLuint id;
    unsigned int size;
    GLenum target;
    static std::map<GLuint, unsigned int> refCount;
};

class IndexBuffer : public Buffer {
  public:
    IndexBuffer();
    IndexBuffer(IndexBuffer const &b);
    IndexBuffer(std::vector<unsigned> const &data, GLenum usage);
    IndexBuffer(std::vector<unsigned> const &data);
};

#endif
