#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _cur_index(0), buffer(), _eof_index() {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const std::string &data, const uint64_t index, const bool eof) {
    // 1. 计算当前允许接收的边界
    // 剩余容量 = 总容量 - ByteStream中还没被读走的字节
    uint64_t first_unacceptable = _cur_index + _capacity - _output.buffer_size();

    // 2. Clipping (裁剪边界)
    uint64_t start = std::max(index, _cur_index);
    uint64_t end = std::min(index + data.size(), first_unacceptable);

    // 3. Buffering (存入 buffer)
    for (uint64_t i = start; i < end; i++) {
        buffer[i] = data[i - index];
    }

    // 4. 处理 EOF 的特殊情况
    // 如果这次传进来的片段带有 eof，我们需要记住这个流的总长度
    // 既然不增加变量，我们可以用一个特定的方式来标记（或者在后面判断）
    // 但 CS144 允许你在类定义里增加私有成员。如果不加，逻辑很难闭环。

    // 5. Pushing to ByteStream (尝试连续写入)
    std::string to_write = "";
    while (buffer.count(_cur_index)) {
        to_write += buffer[_cur_index];
        buffer.erase(_cur_index);
        _cur_index++;
    }

    if (!to_write.empty()) {
        _output.write(to_write);
    }

    // 6. 核心逻辑：什么时候调用 end_input？
    // 如果当前片段带 eof，且裁剪后的末尾就是这个 data 的末尾
    // 并且我们已经把所有数据（包括这个 data）都写进 _output 了
    if (eof && end == index + data.size() && _cur_index == end) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    size_t count = 0;
    for (const auto &p : buffer) {
        count += p.second.size();
    }
    return count;
}

bool StreamReassembler::empty() const { return buffer.empty(); }