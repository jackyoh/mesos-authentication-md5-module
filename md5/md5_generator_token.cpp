#include <stdio.h>
#include <iostream>
#include <openssl/md5.h>
#include <string.h>

using namespace std;
using std::string;

class GeneratorMD5 {
public:
   GeneratorMD5(const string& word){
     _word = word;
   }

   ~GeneratorMD5(){}

   string render(){
      unsigned char digest[MD5_DIGEST_LENGTH];
      char wordArray[sizeof(_word)];
      strcpy(wordArray, _word.c_str());

      MD5((unsigned char*)&wordArray, strlen(wordArray), (unsigned char*)&digest);
     char mdString[33];
     int i;
     for(i = 0; i < 16; i++){
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
     }

      return mdString;

   }

private:

    string _word;

};


/*GeneratorMD5::GeneratorMD5(const string &word){
  _word = word;
}

GeneratorMD5::~GeneratorMD5(){

}

string GeneratorMD5::render(){
  unsigned char digest[MD5_DIGEST_LENGTH];
  char wordArray[sizeof(_word)];
  strcpy(wordArray, _word.c_str());

  MD5((unsigned char*)&wordArray, strlen(wordArray), (unsigned char*)&digest);
  char mdString[33];
  int i;
  for(i = 0; i < 16; i++){
     sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
  }

   return mdString;
}*/
