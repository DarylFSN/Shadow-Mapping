/**************************************************
Light is a class for a camera object.
*****************************************************/
#include <glm/glm.hpp>
#include "DepthShader.h"
#ifndef __LIGHT_H__
#define __LIGHT_H__

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

class Light {
public:
    DepthShader* shader;

    GLuint shadowWidth = SHADOW_WIDTH;
    GLuint shadowHeight = SHADOW_HEIGHT;

    glm::vec4 position = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
   
    GLuint depthMap;
    GLuint depthMapFBO;

    Light() {

        shader = new DepthShader();
        
        glm::mat4 modelview = glm::lookAt(glm::vec3(position), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::mat4(1.0f);
        glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.5f, 12.0f);

        shader->read_source("shaders/depth.vert", "shaders/depth.frag");
        shader->compile();
        shader->initUniforms();
        shader->projection = projection;
        shader->modelview = modelview;
        
        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~Light() {
        delete shader;
    }
};

#endif 
