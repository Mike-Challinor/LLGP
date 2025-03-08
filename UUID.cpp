#include "UUID.h"
#include <random>
#include <sstream>
#include <iomanip>

// Constructor generates a random 128-bit UUID (16 bytes)
UUID::UUID() 
{
    GenerateUUID();
}

// Converts the UUID to a string representation
std::string UUID::ToString() const 
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0')
        << std::setw(8) << m_UUID[0] << "-"
        << std::setw(4) << m_UUID[1] << "-"
        << std::setw(4) << m_UUID[2] << "-"
        << std::setw(4) << m_UUID[3] << "-"
        << std::setw(12) << m_UUID[4];
    return ss.str();
}

// Generates the 128-bit UUID by filling the m_UUID array with random values
void UUID::GenerateUUID() 
{
    std::random_device rd;
    std::uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFF);

    for (int i = 0; i < 5; ++i) 
    {
        m_UUID[i] = dist(rd);
    }
}
