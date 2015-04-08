#include <iostream>

int main(
  int argc,
  const char* argv[]
)
{
  unsigned short s = 1;
  
  if(*(char*)&s == 1)
    std::cout << "LittleEndian";
  else
    std::cout << "BigEndian";
}