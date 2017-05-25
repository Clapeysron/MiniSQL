#pragma once
#include <string>
enum Type {
	Int, 
	Float,
	Chars
};

class TypeInfo {
public:
	TypeInfo(Type type, size_t size);
	~TypeInfo() = default;
	std::string name();

private:
	Type _type;
	size_t _size;

};

