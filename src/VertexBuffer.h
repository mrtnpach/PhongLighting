#pragma once

class VertexBuffer
{
public:

	VertexBuffer(const float* data, int size); // void pointers as to not limit the data type
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetRendererId() const;

	const bool IsBound() const;

private:

	unsigned int _rendererId;
	mutable bool _isBound;
};