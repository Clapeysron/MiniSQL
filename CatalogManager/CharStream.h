#include <streambuf>
#include <string>
#include <cassert>
#include <iostream>
#include <cstring>

class CharInStream:std::streambuf
{
public:
    CharInStream(char*begin, size_t num){
        this->setg(begin, begin, begin+num);
    }
    template<typename T>
    CharInStream& operator>>(T& target){
        assert(this->in_avail()>=sizeof(T));
        this->sgetn(reinterpret_cast<char*>(&target), sizeof(target));
        // memcpy(reinterpret_cast<char*>(&target),this->gptr(),sizeof(T));

        return *this;
    }

    CharInStream& operator>>(std::string& target){
        target.clear();

        while(this->sgetc()!='\0'){
            assert(this->in_avail()>=0);
            target.push_back(this->sbumpc());
        }
        this->snextc();
        return *this;
    }

    char* getbegin(){
        return this->eback();
    }

};

class CharOutStream:std::streambuf
{
public:
    CharOutStream(char* begin, size_t num)
    {
        
        this->setp(begin, begin+num);
        
    }

    template<typename T>
    CharOutStream& operator<<(T& target){
        // std::cout << this->remain() << std::endl;
        // std::cout << sizeof(T) << std::endl;
        assert(this->remain()>=sizeof(target));
        this->sputn(reinterpret_cast<const char*>(&target), sizeof(T));
        return *this;
    }

    CharOutStream& operator<<(std::string& target){
        
        assert(this->remain()>=target.size());
        this->sputn(target.c_str(), target.size());
        this->sputc('\0');
        return *this;
    }

    char* getbegin(){
        return this->pbase();
    }

    size_t remain(){
        return this->epptr()-this->pptr();
    }

};


