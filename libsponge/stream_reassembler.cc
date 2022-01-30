#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : buf(), _output(capacity), _capacity(capacity) {
    bi = 0;
    oi = 0;
}


void StreamReassembler::save_handle(UNIT& elem)
{
    int index = elem.index;
    if (bi != _capacity&&!(buf.count(index))) {
        buf[index] = elem;
        bi += 1;
    }
    return;
}

//不一定，我可以在这里吧每个字节的字符拆解然后发送统一处理
//他希望一次处理的就是所有可以的全部，没有的就丢掉


void StreamReassembler::_write(const string &data, const size_t index, const bool eof)
{
    for(size_t i = 0;i < data.length();++i)
    {
        UNIT tem{index+i,data[i],false};
        if(eof == true && i == data.length()-1)
        {
            tem.eof = true;
        }
        stream_write(tem);
    }
    stream_helper();
}

void StreamReassembler::_save(const string &data, const size_t index, const bool eof)
{
    for(size_t i = 0;i < data.length();++i)
    {
        UNIT tem{index+i,data[i],false};
        if(eof == true && i == data.length()-1)
        {
            tem.eof = true;
        }
        save_handle(tem);
    }
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //if it has useful part
    size_t  lasti = index -1 + data.length();
    if(data.length() == 0 || oi > lasti)
    {
        if(eof)
               _output.end_input();
        return;
    }
    else
    {
        if(oi < index)
        {
            _save(data,index,eof);
        }else
        {
            size_t len = _output.remaining_capacity();
            string use_part = data.substr(oi-index);
            if(len >= use_part.length())
            {
                _write(use_part,oi,eof);
            }else
            {
                _write(use_part.substr(0,len),oi,false);
            }

        }
    }
}

//存储了，但是还没有装配的字节数
size_t StreamReassembler::unassembled_bytes() const { return bi; }

bool StreamReassembler::empty() const { return bi == 0; }
