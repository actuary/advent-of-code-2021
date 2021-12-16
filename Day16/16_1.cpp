#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <assert.h>

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

int bin2dec(std::string const& binary_data) {
  return std::stoi(binary_data, 0, 2);
}

void parseLiteralGroup(std::string const& packet, int& start_pos, int& value) {
  if (start_pos >= packet.length()) 
    assert(false); 

  char group_type = packet[start_pos];

  start_pos += 1;
  value += bin2dec(packet.substr(start_pos, 4));
  start_pos += 4;

  if (group_type == '0')
    return;

  parseLiteralGroup(packet, start_pos, value);

}

int parseLiteralPacket(std::string const& packet, int& start_pos) {
  if (start_pos >= packet.length()) 
    assert(false);

  int value = 0;
  parseLiteralGroup(packet, start_pos, value);

  return value;
}

int parsePacket(std::string const& packet, int& start_pos);

int parseSubPacketsLen(std::string const& packet, int& start_pos, int len) {
  if (len < 1)
    return 0;

  int end = start_pos + len;
  int version_number = 0;
  while (start_pos != end) {
    version_number += parsePacket(packet, start_pos);
  }

  return version_number;
}

int parseSubPacketsNum(std::string const& packet, int& start_pos, int num) {
  int version_number = 0;

  for (int i = 0; i < num; i++) {
    version_number += parsePacket(packet, start_pos);
  }

  return version_number;
}

int parseOperatorPacket(std::string const& packet, int& start_pos, char type) {
  start_pos += 1;
  switch (type) {
    case '0': 
      {
        int subpacket_len = bin2dec(packet.substr(start_pos, 15));
        start_pos += 15;
        return parseSubPacketsLen(packet, start_pos, subpacket_len);
      }
    case '1':
      {
        int subpacket_num = bin2dec(packet.substr(start_pos, 11));
        start_pos += 11;
        return parseSubPacketsNum(packet, start_pos, subpacket_num);
      }
  }
  assert(false);
}

int parsePacket(std::string const& packet, int& start_pos) {
  if (start_pos >= packet.length())
    return 0;
  
  int version_number = bin2dec(packet.substr(start_pos, 3));
  start_pos += 3;
  int type_id = bin2dec(packet.substr(start_pos, 3));
  start_pos += 3;

  if (type_id == 4) {
    parseLiteralPacket(packet, start_pos);
  } else {
    return version_number + parseOperatorPacket(packet, start_pos, packet[start_pos]);
  }

  return version_number;
}

void solve(std::string const& data) {
  int start_pos = 0;
  std::cout << parsePacket(data, start_pos) << "\n";
}

void solution() {
  auto data = getData();

  solve(data);
}

void test() {
  std::string data = transform("D2FE28");
  //solve(data); //6
  
  data = transform("EE00D40C823060"); //operator, length type id 1
  //solve(data); //14
  
  data = transform("8A004A801A8002F478"); 
  solve(data); //16

  data = transform("620080001611562C8802118E34"); 
  solve(data); //12
  
  data = transform("C0015000016115A2E0802F182340"); 
  solve(data); //23

  data = transform("A0016C880162017C3686B18A3D4780"); 
  solve(data); //31
}

int main() {
  test();
  solution();
  return 0;
}
