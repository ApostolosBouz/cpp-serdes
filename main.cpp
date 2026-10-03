#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>


struct Record
{
  std::uint32_t id;
  std::string name;
};

// Serialize
void save(const Record &r, const std::string &path)
{
  std::ofstream out(path, std::ios::binary);
  out.write(reinterpret_cast<const char *>(&r.id), sizeof(r.id));

  std::uint32_t len = static_cast<std::uint32_t>(r.name.size());
  out.write(reinterpret_cast<const char *>(&len), sizeof(len));
  out.write(r.name.data(), len);
}

// Deserialize
Record load(const std::string &path)
{
  std::ifstream in(path, std::ios::binary);
  
  Record r;
  in.read(reinterpret_cast<char *>(&r.id), sizeof(r.id));

  std::uint32_t len = 0;
  in.read(reinterpret_cast<char *>(&len), sizeof(len));

  r.name.resize(len);
  in.read(&r.name[0], len);

  return r;
}

int main()
{
  save({42, "hello"}, "record.bin");
  Record r = load("record.bin");
  std::cout << r.id << " " << r.name << "\n"; // prints: 42 hello

  return 0;
}