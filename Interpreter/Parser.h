#pragma once

#include "Block.h"

namespace Parser {
	bool isAssignment(int priority);

	std::unique_ptr<Block> parseFile(const std::string& filename);

	std::unique_ptr<Block> testLine(const std::string& line);
}