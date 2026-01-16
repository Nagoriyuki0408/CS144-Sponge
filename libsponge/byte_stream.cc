#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

// 声明一个字节流，维护一个capacity大小的窗口
ByteStream::ByteStream(const size_t capacity)
    :   _capacity(capacity), 
        _byteslist(), 
        is_ended(false), 
        _bytes_written(0), 
        _bytes_read(0) {}

size_t ByteStream::write(const string &data) {
    size_t i = 0;
    // 条件 1: i 必须小于字符串长度 (防止越界)
    // 条件 2: 列表当前大小必须小于总容量 (防止溢出)
    while (i < data.size() && _byteslist.size() < _capacity) {
        _byteslist.push_back(data[i]);
        i++;
        _bytes_written++;  // 累计写入总数
    }

    // i 就是本次循环实际存入的字节数
    return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string res;
    for (size_t i = 0; i < len; i++) {
        if (_byteslist.size() > 0) {
            res += _byteslist[i];
        }
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    for (size_t i = 0; i < len; i++) {
        if (_byteslist.size() > 0) {
            _byteslist.pop_front();
            _bytes_read++;
        }
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res;
    for (size_t i = 0; i < len; i++) {
        if (_byteslist.size() > 0) {
            char item = _byteslist.front();
            _byteslist.pop_front();
            res += item;
            _bytes_read++;
        }
    }
    end_input();
    return res;
}

// 这是什么意思，输入端已经结束
void ByteStream::end_input() {
    is_ended = true;
}

// 这是看writer写入的内容是否完成
bool ByteStream::input_ended() const {
    return is_ended;
}

// 返回当前字节流窗口的大小
size_t ByteStream::buffer_size() const { 
    return _byteslist.size();
}

// 这是看字节流窗口是否为空
bool ByteStream::buffer_empty() const { 
    return _byteslist.empty();
}

// 查看字节流是否达到了容量
bool ByteStream::eof() const { 
    return input_ended() && buffer_empty(); 
}

// 返回写入的字节数
size_t ByteStream::bytes_written() const { 
    return _bytes_written;
}

// 读取的字节数
size_t ByteStream::bytes_read() const { return _bytes_read; }

// 返回字节流窗口的剩余容量
size_t ByteStream::remaining_capacity() const { return _capacity - _byteslist.size(); }
