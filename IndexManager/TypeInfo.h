#pragma once
#include <string>
#include <../CatalogManager/CharStream.h>

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
	static TypeInfo deserialize(CharInStream& cis) {
		Type type;
		size_t size;
		cis >> type;
		cis >> size;
		return TypeInfo(type, size);
	}

	void serialize(CharOutStream& couts)const {
		couts << _type;
		couts << _size;
	}

private:
	Type _type;
	size_t _size;

};

// TODO make a compare function for every Type

