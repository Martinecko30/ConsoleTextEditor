#include "GapBuffer.h"

#include <algorithm>

GapBuffer::GapBuffer() {
	data.resize(32);
	l = 0;
	r = data.size();
}

GapBuffer::~GapBuffer() {

}

void GapBuffer::left() {
	if (l <= 0)
		return;

	data[--r] = data[--l];
}

void GapBuffer::right() {
	if (r >= data.size())
		return;

	data[l++] = data[r++];
}

void GapBuffer::insert(char x) {
	if (l == r)
		grow();

	data[l++] = x;
}

void GapBuffer::remove() {
	if (l == 0)
		return;

	--l;
}

void GapBuffer::remove_forward() {
	if (r == data.size())
		return;

	++r;
}

size_t GapBuffer::getCursorPos() {
	return l;
}

void GapBuffer::grow() {
	size_t old_size = data.size();
	size_t new_size = old_size << 1;
	data.resize(new_size);
	std::copy_n(data.begin() + r, old_size - r, data.begin() + new_size - (old_size - r));

	r = new_size - (old_size - r);
}

std::string GapBuffer::getText() {
	std::string text;
	text.append(data.begin(), data.begin() + l);
	text.append(data.begin() + r, data.end());
	return text;
}