#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>

#include  <utility>
#include <map>

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
    /*
      我打算用map来做，
        与out_stream 匹配的是 oi 表示下一个进入流的序号初始是0
        用length()来记录容量
    */

  typedef struct unit{
    size_t index;
    char elem;
    bool eof;
  } UNIT;


    std::map<size_t,UNIT> buf;
    size_t bi = 0;
    size_t oi = 0;

    ByteStream _output;  //!< The reassembled in-order byte stream 按顺序装配好的字节流
    size_t _capacity;    //!< The maximum number of bytes最大的字节的数量

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    // 这个容量限制了已经装配和没有装配的字节的数量
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    void stream_helper()
    {
        while (buf.count(oi) ) {
            stream_write(buf[oi]);
            //写入之后把他删掉
        }
    }


    // ! \brief 写入流中的方法
    //  逐字节地将内容写入流中，返回鞋屋流中的数量,没有写入的会被默默地丢掉
    int stream_write(const UNIT&data)
    {
      int tem =  _output.write(std::string(1,data.elem));
      if(buf.count(data.index))
      {
         buf.erase(buf.find(data.index));
        --bi;
      }
      oi += tem;
      if(data.eof && tem != 0)
      {
        _output.end_input();
      }
      return tem;
      
    }

    void save_handle(UNIT& elem);

    void _save(const std::string &data, const uint64_t index, const bool eof);
    void _write(const std::string &data, const uint64_t index, const bool eof);
    //
    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}


    size_t remain_cap()
    {
        return _capacity - unassembled_bytes();
    }
    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
