#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _cur_index(0), buffer(), _eof_index(), _eof_seen(){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const std::string &data, const uint64_t index, const bool eof) {
    // 1. 记录 EOF 状态 (持久化)
    if (eof) {
        _eof_seen = true;
        _eof_index = index + data.size();
    }

    // 2. 严谨计算边界 (针对 #25 Capacity)
    uint64_t first_unread = _output.bytes_read();
    uint64_t first_unacceptable = first_unread + _capacity;

    // 3. 裁剪数据
    uint64_t start = std::max(index, _cur_index);
    uint64_t end = std::min(index + data.size(), first_unacceptable);

    // 4. 存入 buffer 并维护计数
    for (uint64_t i = start; i < end; i++) {
        // 使用 map 的 emplace 或 count 检查，防止重复计算 unassembled_bytes
        if (buffer.find(i) == buffer.end()) {
            buffer[i] = data[i - index];
            // _unassembled_cnt++; // 如果你增加了计数变量的话
        }
    }

    // 5. 连续写入
    std::string to_write = "";
    while (buffer.count(_cur_index)) {
        to_write += buffer[_cur_index];
        buffer.erase(_cur_index);
        _cur_index++;
    }
    if (!to_write.empty()) {
        _output.write(to_write);
    }

    // 6. 判定结束 (针对 #21 Holes)
    // 只要见过 EOF 标志，且当前重组进度追上了标志位置，就关门
    if (_eof_seen && _cur_index >= _eof_index) {
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