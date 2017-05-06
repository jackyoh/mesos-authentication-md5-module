#include "stdio.h"
#include "../md5/md5_generator_token.cpp"
#include "iostream"

using namespace std;

int main(void){
   GeneratorMD5 md5("123456");
   cout<<md5.render()<<endl;
   return 0;
}
