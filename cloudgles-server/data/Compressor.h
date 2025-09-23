#ifndef COMPRESSOR_H
#define COMPRESSOR_H

class Compressor {
   public:
    /**
     * Compress the data in buf
     * @param buf the buffer to compress
     * @param buf_start the start position of the data to compress
     * @param buf_end the end position of the data to compress
     * @param data_len the length of the data to compress
     * @return the length of the compressed data
     */
    virtual int compress(char *buf, int buf_start, int buf_end,
                         int data_len) = 0;

    virtual ~Compressor() = default;
};

#endif
