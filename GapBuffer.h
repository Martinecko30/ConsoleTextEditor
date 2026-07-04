#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

class GapBuffer {
public:
	GapBuffer();
	~GapBuffer();
	void left();
	void right();
	void insert(char);
	void remove();
	void remove_forward();
	size_t getCursorPos();

	std::string getText();

private:
	void grow();

	std::vector<char> data;
	size_t l = 0;
	size_t r = 0;
};