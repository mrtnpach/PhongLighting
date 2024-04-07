#pragma once

class ElementBuffer
{
public:

	ElementBuffer(const unsigned int* data, int size, int count);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;

	const unsigned int GetRendererId() const;
	const unsigned int GetCount() const;

	const bool IsBound() const;

private:

	unsigned int _rendererId;
	unsigned int _count;
	mutable bool _isBound;
};