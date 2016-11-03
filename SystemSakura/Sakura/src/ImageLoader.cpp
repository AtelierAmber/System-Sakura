#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "SakuraErrors.h"

#define COULD_NOT_FIND_FILE -5

namespace Sakura {

	Sakura::GLTexture ImageLoader::loadPNG(std::string filePath, TexParam param){
        //Create a GLTexture and initialize all its fields to 0
        GLTexture texture = {};

        //This is the input data to decodePNG, which we load from a file
        std::vector<unsigned char> in;
        //This is the output data from decodePNG, which is the pixel data for our texture
        std::vector<unsigned char> out;

        unsigned long width, height;

        //Read in the image file contents into a buffer
        if (IOManager::readFileToBuffer(filePath, in) == false) {
			SAKURA_PRINT_ERROR(std::string("Failed to load PNG file to buffer! Does it exist at " + filePath + "?\n").c_str());
        }

        //Decode the .png format into an array of pixels
        int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		SAKURA_FATAL_ASSERT((errorCode == 0), std::string("decodePNG failed with error: " + std::to_string(errorCode)).c_str());

        //Generate the openGL texture object
        glGenTextures(1, &(texture.id));

        //Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture.id);
        //Upload the pixels to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		/* Setup all the texture parameters */

		if (param & DEFAULT){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			// Generate the mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			// Wrap Formatting //
			if (param & REPEAT){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else if (param & EDGE_CLAMP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else if (param & TRANS_BORDER){
				float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
			}
			else{
				printf("WARNING : NO TEXTURE WRAP SPECEIFIED, REVERTING TO DEFAULT...\n");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			// Filter //
			if (param & PIXELATED && param & MIPMAP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}
			else if (param & PIXELATED){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}else if (param & LINEAR && param & MIPMAP){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			else if(param & LINEAR){
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else{
				printf("WARNING : NO TEXTURE FILTER SPECEIFIED, REVERTING TO DEFAULT...\n");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			// Mipmap //
			if (param & MIPMAP){
				//Generate the mipmaps
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else{
				printf("WARNING : NO MIPMAP GENERATED FOR TEXTURE ID %d\n", texture.id);
			}
		}

        //Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

		SAKURA_ASSERT_GL_ERROR(glGetError());

        texture.width = width;
        texture.height = height;

        //Return a copy of the texture data
        return texture;
    }

}