#ifndef BUFFER_H
#define BUFFER_H
#define leftMask 0x1
#define rightMask 0x80
#define BYTE 8

#include <cmath>
#include <iostream>
#include <vector>
namespace Buffer{
    template <typename T>
    class Buffer{
    private:
        unsigned char* buffer;
        int sz,rsv;
        int elemSize;
        unsigned char prefix;
    public:
        Buffer(unsigned char prefix=0x0){
            this->elemSize=sizeof(T);
            this->prefix=prefix;
            this->buffer=new unsigned char;
            this->sz=0;
            this->rsv=1;
        };
        explicit Buffer(int size,unsigned char prefix=0x0){
            this->elemSize=sizeof(T);
            this->prefix=prefix;
            this->buffer=new unsigned char[size];
            this->sz=size;
            this->rsv=size;
        }
        Buffer(T* t_buffer,int t_size,unsigned char prefix=0x0){
            this->elemSize=sizeof(T);
            this->prefix=prefix;
            int s=t_size*this->elemSize;
            this->buffer=new unsigned char[s];
            this->sz=s;
            this->rsv=s;
            for(int i=0;i<t_size;i++){
                this->push_back(t_buffer[i]);
            }
        }
        unsigned char& operator[](int index){
            if(index==0){ return this->prefix; } else {
                return this->buffer[index-1];
            }
        }
        unsigned char operator[](int index) const{
            if(index==0){ return this->prefix; } else {
                return this->buffer[index-1];
            }
        }
        unsigned char& at(int index){
            return this->buffer[index];
        }
        unsigned char at(int index) const{
            return this->buffer[index];
        }
        void push_back(T elem){
            if(this->sz+this->elemSize>this->rsv){
                this->reserve((this->sz+this->elemSize)*2);
            }
            char* bytes(reinterpret_cast<char*>(&elem));
            for(int i=0;i<this->elemSize;i++){
                this->buffer[this->sz++]=bytes[i];
            }
        }
        void reserve(int size){
            if(size>this->rsv){
                unsigned char* nP=new unsigned char[size];
                this->rsv=size;
                std::copy(this->buffer,this->buffer+this->sz,nP);
                delete[] this->buffer;
                this->buffer=nP;
            }
        }
        int size(){
            return this->sz;
        }
        bool isEmpty(){
            return this->sz==0;
        }
        void empty(){
            unsigned char* nP=new unsigned char;
            delete[] this->buffer;
            this->buffer=nP;
            this->sz=0;
            this->rsv=1;
        }
        unsigned char getPrefix(){
            return this->prefix;
        }
        ~Buffer(){
            delete[] this->buffer;
        }
    };

    template <typename T,typename B>
    std::vector<T> group(Buffer<B> b){
        std::vector<T> v(b.size()/sizeof(T));
        for(int i=0;i<v.size();i++){
            v[i]=*(reinterpret_cast<T*>(&b.at(sizeof(T)*i)));
        }
        return v;
    }
}

    

#endif