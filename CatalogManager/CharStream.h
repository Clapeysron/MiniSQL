#pragma once
#include <streambuf>
#include <string>
#include <cassert>
#include <iostream>
#include <cstring>

class CharInStream :std::streambuf {
public:
	CharInStream(char*begin, size_t num) {
		this->setg(begin, begin, begin + num);
	}

	CharInStream& operator>>(std::string& target) {
		target.clear();

		while (this->sgetc() != '\0') {
			assert(this->in_avail() >= 0);
			target.push_back(this->sbumpc());
		}
		this->snextc();
		return *this;
	}

	template<typename T>
	CharInStream& operator>>(T& target) {
		assert(this->in_avail() >= sizeof(T));
		memcpy(reinterpret_cast<char*>(&target), this->gptr(), sizeof(T));
		this->gbump(sizeof(T));

		return *this;
	}



	char* getbegin() {
		return this->eback();
	}

	size_t remain() {
		return this->egptr() - this->gptr();
	}

	void resize(size_t num) {
		assert(int(num) >= this->gptr() - this->eback());
		this->setg(this->eback(), this->gptr(), this->eback() + num);
	}

	void reset() {
		this->setg(this->eback(), this->eback(), this->egptr());
	}

};

class CharOutStream :std::streambuf {
public:
	CharOutStream(char* begin, size_t num) {
		this->setp(begin, begin + num);
	}


	CharOutStream& operator<<(const std::string& target) {
		//std::cout << "<< string" << std::endl;
		assert(this->remain() >= target.size());
		this->sputn(target.c_str(), target.size());
		this->sputc('\0');
		return *this;
	}

	template<typename T>
	CharOutStream& operator<<(const T& target) {
		//std::cout << "<< not string" << std::endl;
		assert(this->remain() >= sizeof(target));
		this->sputn(reinterpret_cast<const char*>(&target), sizeof(T));
		return *this;
	}




	char* getbegin() {
		return this->pbase();
	}

	size_t remain() {
		return this->epptr() - this->pptr();
	}

	void reset() {
		this->setp(this->pbase(), this->pbase(), this->epptr());
	}

};


