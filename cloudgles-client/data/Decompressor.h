#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

class Decompressor {
   public:
    virtual int decompress(char *buf, int buf_start, int buf_end,
                           int data_len) = 0;
};

#endif  // DECOMPRESSOR_H
