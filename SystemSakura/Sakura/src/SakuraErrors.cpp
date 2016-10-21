#include "SakuraErrors.h"
#include <stdio.h>

#include <stdexcept>
namespace Sakura {

    //Prints out an error message and exits the game
    void fatalError(const std::string& errorString) {
		fprintf(stderr, "%s", errorString.c_str());
		fflush(stderr);
		throw std::logic_error(errorString.c_str());
    }

}