#pragma once
//This file holds some global error functions
#include <GL/glew.h>
#include <vector>
#include <string>

#define SAKURA_THROW_FATAL(s) fatalError(s);
#define SAKURA_FATAL_ASSERT(equate, errStr) if(!equate) fatalError(errStr);
#define SAKURA_ASSERT_FUNC(funcT, str) __try{ funcT }__except(...){ std::fprintf(stderr, "Exception occured: %s\n", str.c_str()); fflush(stderr);}
#define SAKURA_STATIC_ASSERT(equate, errStr) sakura_static_assert(equate, errStr, __FILE__, __LINE__);
#define SAKURA_ASSERT_GL_ERROR(err) if(err) sakura_gl_error(err, __FILE__, __LINE__);

#define SAKURA_PRINT_ERROR(err) std::fprintf(stderr, "%s\n", err); fflush(stderr);

namespace {
	std::vector<std::string> errRecord;

	bool isRecorded(std::string err){
		for (std::size_t i = 0; i < errRecord.size(); ++i){
			if (err == errRecord[i]){
				return true;
			}
		}
		errRecord.push_back(err);
		return false;
	}
	extern void sakura_gl_error(unsigned int err, const char *file, int line){
		while (err != GL_NO_ERROR) {
			std::string error;

			switch (err) {
			case GL_INVALID_OPERATION:              error = "INVALID_OPERATION";              break;
			case GL_INVALID_ENUM:                   error = "INVALID_ENUM";                   break;
			case GL_INVALID_VALUE:                  error = "INVALID_VALUE";                  break;
			case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY";                  break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
			}
			if (!isRecorded(error)){
				std::fprintf(stderr, "GL_%s - %s:%i\n", error.c_str(), file, line);
				fflush(stderr);
			}
			err = glGetError();
		}
	}

	extern int sakura_static_assert(bool equate, const char* errStr, const char *file, int line){
		if (!equate){
			if (!isRecorded(std::string(errStr))){
				std::fprintf(stderr, "Exception occured: %s - %s:%i\n", errStr, file, line);
				fflush(stderr);
			}
			return 0;
		}
		return 1;
	}

	extern void fatalError(const std::string& errorString){
		if (!isRecorded(errorString)){
			std::fprintf(stderr, "Fatal Exception occured: %s\n", errorString.c_str());
			fflush(stderr);
			throw(errorString.c_str());
		}
	}
}