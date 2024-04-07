#include "ElementBuffer.h"
#include <glad/glad.h>

ElementBuffer::ElementBuffer(const unsigned int* data, int size, int count) : _count(count)
{
	glGenBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	_isBound = true;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &_rendererId);
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	_isBound = true;
}

void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_isBound = false;
}

const unsigned int ElementBuffer::GetRendererId() const
{
	return _rendererId;
}

const unsigned int ElementBuffer::GetCount() const
{
	return _count;
}

const bool ElementBuffer::IsBound() const
{
	return _isBound;
}
