//
//  CubeMap.hpp
//  phong
//
//  Created by Brian Summa on 7/26/22.
//

#ifndef CubeMap_h
#define CubeMap_h

#include "common.h"

using namespace std;

class CubeMap{
public:
  
  GLuint cubemapTexture;

  CubeMap(){
    glGenTextures(1, &cubemapTexture);
  };
  
  ~CubeMap(){
    glDeleteTextures(1, &cubemapTexture);
  }
  
  void loadImages(std::vector < string > files);
  
  void glInit();
    
  void draw(mat4 modelview, mat4 projection);
  
private:
  GLuint program;
  GLuint skyboxVAO, skyboxVBO;
  
  GLuint ModelView_loc;
  GLuint Projection_loc;


  
};

#endif /* CubeMap_h */
