#include <fstream>  // std::ofstream / std::ifstream
#include <string>   // std::string
#include <cstdint>  // std::uint32_t
#include <iostream> // std::cout

struct Record
{
    std::uint32_t id;
    std::string   name;
};

/**
 * @brief Serialises a Record object to bytes on disk.
 * @param record The Record object to serialise.
 * @param pathToBin Path to the binary file the object's data is written to.
 */
void Save(const Record& record, const std::string& pathToBin)
{
    std::ofstream out(pathToBin, std::ios::binary);

    // Fixed-size field: dump its raw bytes directly.
    out.write(reinterpret_cast<const char*>(&record.id), sizeof(record.id));

    // Variable-length field: write the length FIRST, then the characters.
    std::uint32_t strLength = static_cast<std::uint32_t>(record.name.size());
    out.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));
    out.write(record.name.data(), strLength);
}

/**
 * @brief Deserialises bytes on disk into a Record object.
 * @param pathToBin Path to the binary file to read from.
 * @return A Record object holding the deserialised data.
 */
Record Load(const std::string& pathToBin)
{
    std::ifstream in(pathToBin, std::ios::binary);
    Record record;

    in.read(reinterpret_cast<char*>(&record.id), sizeof(record.id));

    std::uint32_t strLength = 0;
    in.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
    record.name.resize(strLength);
    in.read(&record.name[0], strLength);   // read the characters into the string's buffer

    return record;
}

int main()
{
    Save({42, "hello"}, "helloRecord.bin"); // should save helloRecord.bin in the working directory
    Record helloRecord = Load("helloRecord.bin");
    std::cout << helloRecord.id << " " << helloRecord.name << "\n";   // should print: 42 hello
}