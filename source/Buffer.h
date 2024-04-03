#pragma once
#include <string>

class Buffer
{
public:
    Buffer();
    ~Buffer();

    void Append(const char *str, int size);
    ssize_t Size();
    const char* C_str();
    void Clear();
    void Getline();
    void SetBuf(const char *buf);

private:
    std::string m_buf;
};