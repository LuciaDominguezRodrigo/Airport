#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

// Shaders
   Shaders shaders;

// Modelos
   Model sphere;
   Model plane;
   Model jet;

// Imagenes (texturas)
   Texture imgNoEmissive;
   Texture imgRuby;
   Texture imgGold;
   Texture imgEarth;
   Texture imgChess;
   Texture imgCubeDiffuse;
   Texture imgCubeSpecular;
   Texture imgWindow;
   Texture asphaltDiffuse;
   Texture asphaltSpecular;
   Texture asphaltNormal;
   Texture cammo;
   Texture trackDiffuse;
   Texture trackSpecular;
   Texture trackNormal;

// Luces y materiales
   #define   NLD 1
   #define   NLP 1
   #define   NLF 2
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  mluz;
   Material  ruby;
   Material  gold;
   Textures  texRuby;
   Textures  texGold;
   Textures  texEarth;
   Textures  texChess;
   Textures  texCube;
   Textures  texWindow;
   Textures  texAsphalt;
   Textures  texCammo;
   Textures  texTrack;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float rotX = 0.0;
   float rotY = 0.0;
   float desZ = 0.0;

// Movimiento de camara
   float fovy   = 60.0;
   float alphaX =  0.0;
   float alphaY =  0.0;

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Proyecto Final", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback      (window, funKey);
    glfwSetScrollCallback   (window, funScroll);
    glfwSetCursorPosCallback(window, funCursorPos);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    sphere.initModel("resources/models/sphere.obj");
    plane.initModel("resources/models/plane.obj");
    jet.initModel("resources/models/jet.obj");

 // Imagenes (texturas)
    imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
    imgRuby.initTexture("resources/textures/imgRuby.png");
    imgGold.initTexture("resources/textures/imgGold.png");
    imgEarth.initTexture("resources/textures/imgEarth.png");
    imgChess.initTexture("resources/textures/imgChess.png");
    imgCubeDiffuse.initTexture("resources/textures/imgCubeDiffuse.png");
    imgCubeSpecular.initTexture("resources/textures/imgCubeSpecular.png");
    imgWindow.initTexture("resources/textures/imgWindow.png");
    asphaltDiffuse.initTexture("resources/textures/asphaltDiffuse.jpg");
    asphaltSpecular.initTexture("resources/textures/asphaltSpecular.png");
    asphaltNormal.initTexture("resources/textures/asphaltNormal.png");
    cammo.initTexture("resources/textures/cammo.jpg");
    trackDiffuse.initTexture("resources/textures/trackDiffuse.jpg");
    trackSpecular.initTexture("resources/textures/trackSpecular.jpg");
    trackNormal.initTexture("resources/textures/trackNormal.jpg");

 // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

 // Luces direccionales
    lightD[0].direction = glm::vec3(0.0, 0.0, 1.0);  // Dirección hacia el centro
    lightD[0].ambient   = glm::vec3(0.1, 0.1, 0.1);
    lightD[0].diffuse   = glm::vec3(0.7, 0.7, 0.7);
    lightD[0].specular  = glm::vec3(0.7, 0.7, 0.7);

 // Luces posicionales
    lightP[0].position    = glm::vec3(-8.0, 5.0, 3.0);
    lightP[0].ambient     = glm::vec3(0.8, 0.8, 0.8);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Luces focales
    lightF[0].position    = glm::vec3(0.0, 2.0, 5.0);  // Posición encima del caza
    lightF[0].direction   = glm::normalize(glm::vec3(-8.0, 0.0, 0.0) - lightF[0].position);  // Apunta al caza
    lightF[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightF[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightF[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;
    lightF[1].position    = glm::vec3( 2.0,  2.0,  5.0);
    lightF[1].direction   = glm::vec3(-2.0, -2.0, -5.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 5.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

 // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8;

    gold.ambient   = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
    gold.diffuse   = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
    gold.specular  = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
    gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    gold.shininess = 51.2;

    texRuby.diffuse   = imgRuby.getTexture();
    texRuby.specular  = imgRuby.getTexture();
    texRuby.emissive  = imgNoEmissive.getTexture();
    texRuby.normal    = 0;
    texRuby.shininess = 76.8;

    texGold.diffuse   = imgGold.getTexture();
    texGold.specular  = imgGold.getTexture();
    texGold.emissive  = imgNoEmissive.getTexture();
    texGold.normal    = 0;
    texGold.shininess = 51.2;

    texEarth.diffuse   = imgEarth.getTexture();
    texEarth.specular  = imgEarth.getTexture();
    texEarth.emissive  = imgNoEmissive.getTexture();
    texEarth.normal    = 0;
    texEarth.shininess = 10.0;

    texChess.diffuse   = imgChess.getTexture();
    texChess.specular  = imgChess.getTexture();
    texChess.emissive  = imgNoEmissive.getTexture();
    texChess.normal    = 0;
    texChess.shininess = 10.0;

    texCube.diffuse    = imgCubeDiffuse.getTexture();
    texCube.specular   = imgCubeSpecular.getTexture();
    texCube.emissive   = imgNoEmissive.getTexture();
    texCube.normal     = 0;
    texCube.shininess  = 10.0;

    texWindow.diffuse   = imgWindow.getTexture();
    texWindow.specular  = imgWindow.getTexture();
    texWindow.emissive  = imgWindow.getTexture();
    texWindow.normal    = 0;
    texWindow.shininess = 10.0;

    texAsphalt.diffuse    = asphaltDiffuse.getTexture();
    texAsphalt.specular   = asphaltSpecular.getTexture();
    texAsphalt.emissive   = imgNoEmissive.getTexture();
    texAsphalt.normal     = asphaltNormal.getTexture();
    texAsphalt.shininess  = 51.2;

    texCammo.diffuse    = cammo.getTexture();
    texCammo.specular   = cammo.getTexture();
    texCammo.emissive   = imgNoEmissive.getTexture();
    texCammo.normal     = 0;
    texCammo.shininess  = 51.2;

    texTrack.diffuse    = trackDiffuse.getTexture();
    texTrack.specular   = trackSpecular.getTexture();
    texTrack.emissive   = imgNoEmissive.getTexture();
    texTrack.normal     = trackNormal.getTexture();
    texTrack.shininess  = 51.2;

}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    float x = (6.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX)));
    float y = 3.0f + (6.0f*glm::sin(glm::radians(alphaY)));
    float z = 6.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
    glm::mat4 Ry90 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,1,0));
    glm::mat4 SAsphalt = glm::scale    (I, glm::vec3(4.0, 1.0, 12.0));
    glm::mat4 TTrack = glm::translate(I, glm::vec3(-8.0, 0.0, 0.0));
    glm::mat4 TTerminal = glm::translate(I, glm::vec3(-16.0, 0.0, 4.0));
    glm::mat4 STerminal = glm::scale    (I, glm::vec3(4.0, 1.0, 8.0));
    glm::mat4 SAsphalt2 = glm::scale    (I, glm::vec3(4.0, 1.0, 4.0));
    glm::mat4 TAsphalt2 = glm::translate(I, glm::vec3(-16.0, 0.0, -8.0));
    drawObjectTex(plane, texAsphalt, P, V, SAsphalt);
    drawObjectTex(plane, texTrack, P, V, TTrack * SAsphalt * Ry90);
    drawObjectTex(plane, texChess, P, V, TTerminal * STerminal * Ry90);
    drawObjectTex(plane, texAsphalt, P, V, TAsphalt2*SAsphalt2);

    glm::mat4 SJet = glm::scale    (I, glm::vec3(0.02, 0.02, 0.02));
    glm::mat4 RJet = glm::rotate   (I, glm::radians(rotY + 90), glm::vec3(0,1,0));
    glm::mat4 Rx = glm::rotate   (I, glm::radians(rotX ), glm::vec3(1,0,0));
    glm::mat4 TJet = glm::translate(I, glm::vec3(-8.0, 0.0, desZ));
    drawObjectTex(jet, texCammo, P, V, TJet*  Rx * RJet * SJet);

    glm::mat4 Rv = glm::rotate   (I, glm::radians(90.0f), glm::vec3(1,0,0));
    glm::mat4 Tv = glm::translate(I, glm::vec3(0.0, 0.0, 3.0));
    glDepthMask(GL_FALSE);
        drawObjectTex(plane, texWindow, P, V, Tv * Rv);
    glDepthMask(GL_TRUE);

}

void setLights(glm::mat4 P, glm::mat4 V) {
    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectMat(sphere, mluz, P, V, M);
    }

}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_UP:    rotX -= 5.0f;   break;
        case GLFW_KEY_DOWN:  rotX += 5.0f;   break;
        case GLFW_KEY_LEFT:  rotY -= 5.0f;   break;
        case GLFW_KEY_RIGHT: rotY += 5.0f;   break;
        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;
        default:
            rotX = 0.0f;
            rotY = 0.0f;
            break;
    }

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

    float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}
