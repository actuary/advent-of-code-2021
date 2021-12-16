#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <assert.h>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <memory>


static int const SUBPACK_LEN_BITS = 15;
static int const SUBPACK_NUM_BITS = 11;
static int const ID_LEN = 3;
static int const GROUP_LEN = 4;

enum class PacketType {OPERATOR, LITERAL};
enum class TypeID {SUM, PRODUCT, MINIMUM, MAXIMUM, LITERAL, GREATER_THAN, LESS_THAN, EQUAL_TO};
enum class LengthType {LENGTH_OF_ALL_SUB_PACKETS, NUMBER_OF_SUB_PACKETS};

std::map<char, std::string> hex2bin = {
   {'0', "0000"},
   {'1', "0001"},
   {'2', "0010"},
   {'3', "0011"},
   {'4', "0100"},
   {'5', "0101"},
   {'6', "0110"},
   {'7', "0111"},
   {'8', "1000"},
   {'9', "1001"},
   {'A', "1010"},
   {'B', "1011"},
   {'C', "1100"},
   {'D', "1101"},
   {'E', "1110"},
   {'F', "1111"}
};

std::map<TypeID, std::string> TypeIDToString = {
   {TypeID::SUM, "SUM"},
   {TypeID::PRODUCT, "PRODUCT"},
   {TypeID::MINIMUM, "MIN"},
   {TypeID::MAXIMUM, "MAX"},
   {TypeID::LITERAL, "LITERAL"},
   {TypeID::GREATER_THAN, "GREATER THAN"},
   {TypeID::LESS_THAN, "LESS THAN"},
   {TypeID::EQUAL_TO, "EQUAL TO"}
};

std::map<LengthType, std::string> LengthTypeToString = {
   {LengthType::LENGTH_OF_ALL_SUB_PACKETS, "LEN"},
   {LengthType::NUMBER_OF_SUB_PACKETS, "NUM"}
};

class Packet {
public:
  Packet(int version_number, TypeID type_id) 
    : version_number_(version_number), type_id_(type_id) {
    assert(version_number >= 0 && version_number < 8);
  };

  virtual std::string serialise() const = 0;
  virtual long long getValue() const = 0;
  virtual void print(int offset = 0) const = 0;

  TypeID getTypeID() const { return type_id_; }
  int getVersionNumber() const { return version_number_; }

  long long getPacketLength() const { return serialise().length();}
protected:
  int version_number_;
  TypeID type_id_;
};

class LiteralPacket : public Packet {
public:
  LiteralPacket(int version_number, long long value) :
    Packet(version_number, TypeID::LITERAL), value_(value) {}

  long long getValue() const override { return value_; }


  std::string serialise() const override {
    std::string version_number = std::bitset<3>(version_number_).to_string();
    std::string type_id = std::bitset<3>((int) type_id_).to_string();
    std::string bin_value = std::bitset<64>(value_).to_string();

    std::string result = "";
    result.append(version_number);
    result.append(type_id);

    bin_value = bin_value.substr(bin_value.find('1'), bin_value.length() - bin_value.find('1'));
  
    int num_groups = (bin_value.length()-1) / 4 + 1;

    int pad = ((bin_value.length()-1)/ 4 + 1) * 4 - bin_value.length();
    bin_value = std::string(pad, '0').append(bin_value);
    
    for (int i = 0; i < num_groups; i++) {
      if (i == num_groups - 1)
        result.append("0");
      else 
        result.append("1");
      result.append(bin_value.substr(i*4, 4));
    }

    return result;
  }

  void print(int offset = 0) const override {
    std::cout << std::string(offset, '\t');
    std::cout << "LiteralPacket(version_number = " << version_number_ << ", " 
              <<  "value = " << value_ << ")\n";
  }


private:
  long long value_;
};

class OperatorPacket : public Packet {
public:
  OperatorPacket(int version_number, TypeID type_id, 
                 LengthType length_id, int length_value,
                 std::vector<std::shared_ptr<Packet>> const sub_packets)
    : Packet(version_number, type_id), length_id_(length_id), length_value_(length_value),
      sub_packets_(sub_packets) {}

  long long getValue() const override { 
    std::vector<long long> values;

    for (auto packet : sub_packets_) {
      values.push_back(packet->getValue());
    }

    switch (type_id_) {
      case TypeID::SUM: 
        return std::accumulate(values.begin(), values.end(), 0LL);
      case TypeID::PRODUCT: 
        return std::accumulate(values.begin(), values.end(), 1LL, std::multiplies<long long>());
      case TypeID::MINIMUM: 
        return *std::min_element(values.begin(), values.end());
      case TypeID::MAXIMUM: 
        return *std::max_element(values.begin(), values.end());
      case TypeID::GREATER_THAN: 
        return (values[0] > values[1])*1;
      case TypeID::LESS_THAN: 
        return (values[0] < values[1])*1;
      case TypeID::EQUAL_TO: 
        return (values[0] == values[1])*1;
      default:
        assert(false);
    }
  }

  std::string serialise() const override {
    std::string version_number = std::bitset<3>(version_number_).to_string();
    std::string type_id = std::bitset<3>((int) type_id_).to_string();

    std::string result = "";
    result.append(version_number);
    result.append(type_id);
    result.append(std::bitset<1>((int) length_id_).to_string());

    switch(length_id_) {
      case LengthType::LENGTH_OF_ALL_SUB_PACKETS:
        result.append(std::bitset<SUBPACK_LEN_BITS>(length_value_).to_string());
        break;
      case LengthType::NUMBER_OF_SUB_PACKETS:
        result.append(std::bitset<SUBPACK_NUM_BITS>(length_value_).to_string());
        break;
    }
    
    for (auto packet: sub_packets_) {
      result.append(packet->serialise());
    }

    return result;
  }

  void print(int offset = 0) const override {
    std::cout << std::string(offset, '\t');
    std::cout << "OperatorPacket(version_number = " << version_number_ << ", " 
              << "TypeID = " << TypeIDToString.at(type_id_) << ", "
              << "LengthValue = " << LengthTypeToString.at(length_id_)  << ", "
              << "Length Value = " << length_value_ << ")\n";
    for (auto packet : sub_packets_) { 
      packet->print(offset + 1);
    }
  }

private:
  LengthType length_id_;
  int length_value_;
  std::vector<std::shared_ptr<Packet>> const sub_packets_;
};

std::string getData(std::string const& filename = "input.data") {
  std::ifstream in(filename);

  if (!in)
    return "";

  std::string result = "";

  char ch;
  while ((in >> ch)) {
    result.append(hex2bin.at(ch));
  }

  return result;
}

std::string transform(std::string const& input) {
  std::string output = "";

  for (auto const& ch : input) {
    output.append(hex2bin.at(ch));
  }

  return output;
}

long long bin2dec(std::string const& binary_data) {
  return std::stoll(binary_data, 0, 2);
}

void parseLiteralGroup(std::string const& packet, int& start_pos, std::string& value_str) {
  if (start_pos >= packet.length()) 
    assert(false); 

  char group_type = packet[start_pos];

  start_pos += 1;

  value_str.append(packet.substr(start_pos, GROUP_LEN));
  start_pos += GROUP_LEN;

  if (group_type == '0')
    return;

  parseLiteralGroup(packet, start_pos, value_str);

}

long long parseLiteralPacket(std::string const& packet, int& start_pos) {
  assert(start_pos < packet.length());
  
  std::string value_str = "";
  parseLiteralGroup(packet, start_pos, value_str);
  long long value = bin2dec(value_str);

  return value;
}

long long parsePacket(std::string const& packet, int& start_pos);

std::vector<long long> parseSubPacketsLen(std::string const& packet, int& start_pos, int len) {
  assert(len > 0);

  int end = start_pos + len;

  std::vector<long long> values;
  while (start_pos != end) {
    values.push_back(parsePacket(packet, start_pos));
  }

  return values;
}

std::vector<long long> parseSubPacketsNum(std::string const& packet, int& start_pos, int num) {
  std::vector<long long> values;
  for (int i = 0; i < num; i++) {
    values.push_back(parsePacket(packet, start_pos));
  }

  return values;
}

std::vector<long long> parseOperatorPacket(std::string const& packet, int& start_pos, char type) {
  start_pos += 1;
  switch (type) {
    case '0': 
      {
        int subpacket_len = bin2dec(packet.substr(start_pos, SUBPACK_LEN_BITS));
        start_pos += SUBPACK_LEN_BITS;
        return parseSubPacketsLen(packet, start_pos, subpacket_len);
      }
    case '1':
      {
        int subpacket_num = bin2dec(packet.substr(start_pos, SUBPACK_NUM_BITS));
        start_pos += SUBPACK_NUM_BITS;
        return parseSubPacketsNum(packet, start_pos, subpacket_num);
      }
  }
  assert(false);
}



long long parsePacket(std::string const& packet, int& start_pos) {
  if (start_pos >= packet.length())
    return 0;
  
  int version_number = bin2dec(packet.substr(start_pos, ID_LEN));
  start_pos += ID_LEN;
  TypeID type_id = (TypeID) bin2dec(packet.substr(start_pos, ID_LEN));
  start_pos += ID_LEN;

  if (type_id == TypeID::LITERAL) {
    return parseLiteralPacket(packet, start_pos);
  } else {

    auto values = parseOperatorPacket(packet, start_pos, packet[start_pos]);

    switch (type_id) {
      case TypeID::SUM: 
        return std::accumulate(values.begin(), values.end(), 0LL);
      case TypeID::PRODUCT: 
        return std::accumulate(values.begin(), values.end(), 1LL, std::multiplies<long long>());
      case TypeID::MINIMUM: 
        return *std::min_element(values.begin(), values.end());
      case TypeID::MAXIMUM: 
        return *std::max_element(values.begin(), values.end());
      case TypeID::GREATER_THAN: 
        return (values[0] > values[1])*1;
      case TypeID::LESS_THAN: 
        return (values[0] < values[1])*1;
      case TypeID::EQUAL_TO: 
        return (values[0] == values[1])*1;
      default:
        assert(false);
    }
  }

  assert(false);
}

void deserialiseLiteralGroup(std::string const& packet, int& start_pos, std::string& value_str) {
  if (start_pos >= packet.length()) 
    assert(false); 

  char group_type = packet[start_pos];

  start_pos += 1;

  value_str.append(packet.substr(start_pos, GROUP_LEN));
  start_pos += GROUP_LEN;

  if (group_type == '0')
    return;

  deserialiseLiteralGroup(packet, start_pos, value_str);

}

std::shared_ptr<LiteralPacket> deserialiseLiteralPacket(std::string const& packet, int& start_pos, int version_number, TypeID type_id) {
  assert(start_pos < packet.length());
  
  std::string value_str = "";
  deserialiseLiteralGroup(packet, start_pos, value_str);
  long long value = bin2dec(value_str);

  return std::make_shared<LiteralPacket>(version_number, value);
}

std::shared_ptr<Packet> deserialisePacket(std::string const& packet, int& start_pos);

std::vector<std::shared_ptr<Packet>> deserialiseSubPacketsLen(std::string const& packet, int& start_pos, int len) {
  std::vector<std::shared_ptr<Packet>> packets;

  int end = start_pos + len;
  while (start_pos != end) {
    packets.push_back(deserialisePacket(packet, start_pos));
  }
  
  return packets;
}

std::vector<std::shared_ptr<Packet>> deserialiseSubPacketsNum(std::string const& packet, int& start_pos, int num) {
  std::vector<std::shared_ptr<Packet>> packets;

  for (int i = 0; i < num; i++) {
    packets.push_back(deserialisePacket(packet, start_pos));
  }

  return packets;
}

std::shared_ptr<Packet> deserialisePacket(std::string const& packet, int& start_pos) {
  if (start_pos >= packet.length())
    return nullptr;
  
  int version_number = bin2dec(packet.substr(start_pos, ID_LEN));
  start_pos += ID_LEN;
  TypeID type_id = (TypeID) bin2dec(packet.substr(start_pos, ID_LEN));
  start_pos += ID_LEN;

  if (type_id == TypeID::LITERAL) {
    return deserialiseLiteralPacket(packet, start_pos, version_number, type_id);
  } else {
    LengthType length_type = packet[start_pos] == '0'? LengthType::LENGTH_OF_ALL_SUB_PACKETS : LengthType::NUMBER_OF_SUB_PACKETS;
    int length_num = packet[start_pos] == '0'? SUBPACK_LEN_BITS : SUBPACK_NUM_BITS;
    start_pos += 1;

    int length_value = bin2dec(packet.substr(start_pos, length_num));
    start_pos += length_num;
 
    switch (length_type) {
      case LengthType::LENGTH_OF_ALL_SUB_PACKETS: 
        return std::make_shared<OperatorPacket>(version_number, type_id, length_type, length_value,
            deserialiseSubPacketsLen(packet, start_pos, length_value));
        break;
      case LengthType::NUMBER_OF_SUB_PACKETS:
        return std::make_shared<OperatorPacket>(version_number, type_id, length_type, length_value,
            deserialiseSubPacketsNum(packet, start_pos, length_value));
        break;
    }
  }

  return nullptr;
}

void solve(std::string const& data) {
  int start_pos = 0;
  std::cout << parsePacket(data, start_pos) << "\n";
}

void solution() {
  auto data = getData();
  
  solve(data);
  int start_pos = 0; 
  std::shared_ptr<Packet> packet = deserialisePacket(data, start_pos);
  packet->print(); //prints out what they used to make the values
  std::cout << packet->getValue() << "\n";

}




void test() {
  std::string data = transform("C200B40A82");
  solve(data); //3
  
  data = transform("04005AC33890"); //operator, length type id 1
  solve(data); //54
  
  data = transform("880086C3E88112"); 
  solve(data); //7

  data = transform("CE00C43D881120"); 
  solve(data); //9
  
  data = transform("D8005AC2A8F0"); 
  solve(data); //1

  data = transform("F600BC2D8F"); 
  solve(data); //0

  data = transform("9C005AC2F8F0"); 
  solve(data); //0

  data = transform("9C0141080250320F1802104A08"); 
  solve(data); //1

}

int main() {
  //test();
  solution();
}
