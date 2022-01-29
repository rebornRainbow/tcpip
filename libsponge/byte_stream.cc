#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { 
    _room  = capacity;
    buffer =  new char[capacity + 1];
    wi = 0;
    ri = 0;
    w_n = 0;
    r_n = 0;
    _input_end = false;
}

void ByteStream::write_helper(char tem)
{
    buffer[(wi++)%_room] = tem;
    ++w_n;
}


size_t ByteStream::write(const string &data) {
    size_t w_num = min(data.length(),remaining_capacity()); 
    for(size_t i = 0;i < w_num; ++i)
    {
        write_helper(data[i]);
    }
    //一点点就有这么大的误差他妈的
    return w_num;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t r_num = min(len,buffer_size());
    string res = "";
    for(size_t i = 0;i < r_num; ++i)
    {   
        res += string(1,buffer[(ri+i)%_room]);
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t r_num = min(len,buffer_size());
    for(size_t i = 0;i < r_num; ++i)
    {
        ++ri; 
        //弹出也算读了，这我是没想到的
        ++r_n;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    size_t r_num = min(len,buffer_size());
    string res = "";
    for(size_t i = 0;i < r_num; ++i)
    {   
        res += string(1,buffer[(ri++)%_room]);
        ++r_n; 
    }
    return res;
}

void ByteStream::end_input() {

    _input_end = true;
}

bool ByteStream::input_ended() const { 
    return _input_end; 
}

size_t ByteStream::buffer_size() const { 
    return wi -  ri;
 }

bool ByteStream::buffer_empty() const { 
    return ri == wi;
 }

bool ByteStream::eof() const {
    return input_ended() && buffer_empty(); 
 }

size_t ByteStream::bytes_written() const { 
    return w_n;
 }

size_t ByteStream::bytes_read() const { 
    return r_n;
 }

size_t ByteStream::remaining_capacity() const { 
    return _room - buffer_size();
}
