#ifndef FRAME_H
#define FRAME_H

#define uint unsigned int

struct Frame{
    uint h,w;
    char** frame_alloc;
    uint row_cont;
    Frame() : h(400), w(240){};
    Frame(uint h,uint w): h(h), w(w){};
    void add_row(char* row){
        if(++row_cont>h){
            return;
        }
        frame_alloc[row_cont-1]=row;
    }
    ~Frame(){
        while(h--){
            delete frame_alloc[h];
        }
        delete frame_alloc;
    }
};

#endif