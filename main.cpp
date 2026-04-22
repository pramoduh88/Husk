#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cstring>

int main() {
    char cwd[PATH_MAX +1];
    char* path = getcwd(cwd, sizeof(cwd));

    if (path == nullptr){
    	std::cerr << "Error getting current directory\n";
	return 1;
    }
    int len = (int) strlen(cwd);
    cwd[len] = '>';
    cwd[len+1] = '\0';

    std::string input;
    while(1){
         std::cout << cwd;
         std::getline(std::cin, input);
	 if (input.compare("exit") == 0){
	 	return 0;
	 }
	 std::cout << input << std::endl;
    }    

    return 0;
}
