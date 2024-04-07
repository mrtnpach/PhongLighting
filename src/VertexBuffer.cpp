#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const float* data, int size)
{
	glGenBuffers(1, &_rendererId);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_rendererId);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	_isBound = true;
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_isBound = false;
}

unsigned int VertexBuffer::GetRendererId() const
{
	return _rendererId;
}

const bool VertexBuffer::IsBound() const
{
	return _isBound;
}
