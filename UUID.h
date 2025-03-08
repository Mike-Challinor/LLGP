#ifndef UUID_H
#define UUID_H

#include <string>

class UUID {
public:
    // Default constructor generates a random UUID
    UUID();

    // Convert the UUID to a string representation
    std::string ToString() const;

private:
    uint64_t m_UUID[5];  // 128-bit UUID (16 bytes)

    // Function to generate the random UUID
    void GenerateUUID();
};

#endif // UUID_H


