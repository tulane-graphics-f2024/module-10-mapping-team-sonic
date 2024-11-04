//
//  CubeMap.cpp
//  phong
//
//  Created by Brian Summa on 7/26/22.
//

#include "common.h"
#include "SourcePath.h"


void CubeMap::loadImages(std::vector < string > files){
  
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

  unsigned int width, height, nrComponents;
  for (unsigned int i = 0; i < files.size(); i++)
  {
    std::vector<unsigned char> image;
    unsigned error = lodepng::decode(image, width, height, files[i].c_str());
    std::cout << width << " X " << height << " image loaded\n";
    
    if(!error)
      {
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
        image.clear();
      }
      else
      {
          std::cout << "Cubemap texture failed to load at path: " << files[i] << std::endl;
        image.clear();
      }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
}


void CubeMap::glInit(){
  
  float skyboxVertices[] = {
      // positions
      -1.0f,  1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, -1.0f, 1.0f,
       1.0f,  1.0f, -1.0f, 1.0f,
      -1.0f,  1.0f, -1.0f, 1.0f,

      -1.0f, -1.0f,  1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f,  1.0f, -1.0f, 1.0f,
      -1.0f,  1.0f, -1.0f, 1.0f,
      -1.0f,  1.0f,  1.0f, 1.0f,
      -1.0f, -1.0f,  1.0f, 1.0f,

       1.0f, -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
       1.0f,  1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, -1.0f, 1.0f,

      -1.0f, -1.0f,  1.0f, 1.0f,
      -1.0f,  1.0f,  1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 1.0f,
      -1.0f, -1.0f,  1.0f, 1.0f,

      -1.0f,  1.0f, -1.0f, 1.0f,
       1.0f,  1.0f, -1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
       1.0f,  1.0f,  1.0f, 1.0f,
      -1.0f,  1.0f,  1.0f, 1.0f,
      -1.0f,  1.0f, -1.0f, 1.0f,

      -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f,  1.0f, 1.0f,
       1.0f, -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f,  1.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 1.0f
  };
  
  std::string vshader = source_path + "/shaders/skybox_vshader.glsl";
  std::string fshader = source_path + "/shaders/skybox_fshader.glsl";

  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_shader_compilation(vshader, vertex_shader);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_shader_compilation(fshader, fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);
  check_program_link(program);
  glUseProgram(program);
  
  glBindFragDataLocation(program, 0, "fragColor");

  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  ModelView_loc = glGetUniformLocation( program, "ModelView" );
  Projection_loc = glGetUniformLocation( program, "Projection" );

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glBindVertexArray(0);
}


void CubeMap::draw(mat4 modelview, mat4 projection){
  
  // draw skybox as last
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
  glUseProgram(program);
  
  
  
  glUniformMatrix4fv( ModelView_loc, 1, GL_TRUE, mat4() );
  glUniformMatrix4fv( Projection_loc, 1, GL_TRUE, projection );

  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default

  glUseProgram(0);
}
