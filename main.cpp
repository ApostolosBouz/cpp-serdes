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
 * @brief Writes a 32-bit unsigned integer to a stream in little-endian byte order.
 * @param out The output stream to write the 4 bytes to
 * @param value The value to serialise
 */
void put_u32(std::ostream& out, std::uint32_t value)
{
    unsigned char intBytes[4];
    intBytes[0] = static_cast<unsigned char>(value & 0xFF);
    intBytes[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
    intBytes[2] = static_cast<unsigned char>((value >> 16) & 0xFF);
    intBytes[3] = static_cast<unsigned char>((value >> 24) & 0xFF);

    out.write(reinterpret_cast<const char*>(intBytes), 4);
}

/**
 * Reads a 32-bit unsigned integer written by put_u32 (little-endian) back into value.
 * @param in The input stream to read 4 bytes from
 * @return The reconstructed 32-bit value
 */
std::uint32_t get_u32(std::istream& in)
{
    unsigned char intBytes[4];
    in.read(reinterpret_cast<char*>(intBytes), 4);
    
    return static_cast<std::uint32_t>(intBytes[0])
            | (static_cast<std::uint32_t>(intBytes[1]) << 8)
            | (static_cast<std::uint32_t>(intBytes[2]) << 16)
            | (static_cast<std::uint32_t>(intBytes[3]) << 24);
}

/**
 * @brief Serialises a Record object to bytes on disk.
 * @param record The Record object to serialise.
 * @param pathToBin Path to the binary file the object's data is written to.
 */
void Save(const Record& record, const std::string& pathToBin)
{
    std::ofstream out(pathToBin, std::ios::binary);
    put_u32(out, record.id);

    put_u32(out, static_cast<std::uint32_t>(record.name.size()));
    out.write(record.name.data(), record.name.size());
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
    record.id = get_u32(in);

    std::uint32_t strLength = get_u32(in);
    record.name.resize(strLength);

    in.read(&record.name[0], strLength);

    return record;
}

int main()
{
    Save({42, "hello"}, "helloRecord.bin"); // should save helloRecord.bin in the working directory
    Record helloRecord = Load("helloRecord.bin");
    std::cout << helloRecord.id << ' ' << helloRecord.name << "\n=====\n";   // should print: 42 hello

    Save({291, "The quick brown fox jumps over the lazy dog."}, "Record1.bin");
    Record Record1 = Load("Record1.bin");
    std::cout << Record1.id << '\n' << Record1.name << "\n=====\n";

    Save({83, "I never write tests, but when I do llms write them."}, "Record2.bin");
    Record Record2 = Load("Record2.bin");
    std::cout << Record2.id << '\n' << Record2.name << "\n=====\n";

    Save({180, "- We 've achieved AGI.\n- For real or internally?\n- Internally."}, "Record3.bin");
    Record Record3 = Load("Record3.bin");
    std::cout << Record3.id << '\n' << Record3.name << "\n=====\n";
}
