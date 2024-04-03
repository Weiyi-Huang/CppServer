#include "Buffer.h"
#include <cstring>
#include <iostream>

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::Append(const char *str, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (str[i] == '\0')
            break;
        m_buf.push_back(str[i]);
    }
}

ssize_t Buffer::Size()
{
    return m_buf.size();
}

const char *Buffer::C_str()
{
    return m_buf.c_str();
}

void Buffer::Clear()
{
    m_buf.clear();
}

void Buffer::Getline()
{
    m_buf.clear();
    std::getline(std::cin, m_buf);
}

void Buffer::SetBuf(const char *buf)
{
    m_buf.clear();
    m_buf.append(buf);
}
