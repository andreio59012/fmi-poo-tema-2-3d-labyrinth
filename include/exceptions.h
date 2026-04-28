#pragma once
#include <stdexcept>
#include <string>

class LabyrinthException : public std::runtime_error {
public:
	explicit LabyrinthException(const std::string& msg) : std::runtime_error(msg) {};
};

class InvalidLevelException : public LabyrinthException {
public:
	explicit InvalidLevelException(const std::string& msg) : LabyrinthException("Invalid level: " + msg) {};
};

class InvalidSpriteException : public LabyrinthException {
public:
	explicit InvalidSpriteException(const std::string& msg) : LabyrinthException("Invalid sprite: " + msg) {};
};

class OutOfBoundsException : public LabyrinthException {
public:
	explicit OutOfBoundsException(const std::string& msg) : LabyrinthException("Out of bounds: " + msg) {};
};
