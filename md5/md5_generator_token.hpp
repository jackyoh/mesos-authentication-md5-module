#include "iostream"
#include "string"

#ifndef GeneratorMD5
#define GeneratorMD5

class GeneratorMD5 {
public:
//   GeneratorMD5(const std::string& word);

  virtual ~GeneratorMD5();

  virtual std::string render();

//private:

  //  std::string _word;

};

#endif
