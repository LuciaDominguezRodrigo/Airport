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

void texLoad();
void drawEntorno(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAvion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVentanas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVallas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCielo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTorreControl (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTerminal (glm::mat4 P, glm::mat4 V, glm::mat4 M);

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
   Model fence;
   Model cube;
   Model torre1;
   Model torre2;
   Model cone;
   Model antena;
   Model cubeTerminal;
   Model sofa;
   Model chair;
   Model table;
   Model frame;


// Imagenes (texturas)
   Texture imgNoEmissive;
   Texture imgMiddleEmissive;


   Texture imgWindow;

   Texture imgGround;
   Texture imgSkyDay;
   Texture imgSkyNight;
   Texture imgClouds;

   Texture asphaltDiffuse;
   Texture asphaltSpecular;
   Texture asphaltNormal;

   Texture cammo;

   Texture trackDiffuse;
   Texture trackSpecular;
   Texture trackNormal;

   Texture floorDiffuse;
   Texture floorSpecular;
   Texture floorNormal;

   Texture grassDiffuse;
   Texture grassSpecular;
   Texture grassNormal;

   Texture jetDiffuse;
   Texture jetWingsDiffuse;

   Texture fenceDiffuse;
   Texture fenceNormal;

   Texture torreConcreteDiffuse;
   Texture torreConcreteSpecular;
   Texture torreConcreteNormal;

   Texture imgGlass;

   Texture metAntenaDiffuse;
   Texture metAntenaNormal;
   Texture metAntenaSpecular;

   Texture sofaDiffuse;
   Texture sofaSpecular;
   Texture sofaNormal;


// Luces y materiales
   #define   NLD 1
   #define   NLP 1
   #define   NLF 1
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  mluz;
   Material  mFrame;
   Textures  texWindow;
   Textures  texGround;
   Textures  texSky;
   Textures  texClouds;
   Textures  texAsphalt;
   Textures  texCammo;
   Textures  texTrack;
   Textures  texFloor;
   Textures  texGrass;
   Textures  texJet;
   Textures  texFence;
   Textures  texConcrete;
   Textures  texGlass;
   Textures  texMetalverde;
   Textures  texTelaSofa;


// Modo día/noche
   bool nightMode = false;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float desZ = 0.0;

// Movimiento de camara
   float fovy   = 60.0;
   float alphaX =  0.0;
   float alphaY =  0.0;

//Variables auxiliares temporales para control de cámara
    float x   = 0.0;
    float y =  6.0;
    float z =  -6.0;

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
    cubeTerminal.initModel("resources/models/geometric/cube.obj");
    sphere.initModel("resources/models/geometric/sphere.obj");
    plane.initModel("resources/models/geometric/plane.obj");
    jet.initModel("resources/models/planes/jet.obj");
    fence.initModel("resources/models/sceneParts/fence.obj");
    torre1.initModel("resources/models/geometric/cylinder.obj");
    torre2.initModel("resources/models/geometric/cylinder.obj");
    cone.initModel("resources/models/geometric/cone.obj");
    antena.initModel("resources/models/sceneParts/antena.obj");
    sofa.initModel("resources/models/sceneParts/sofa.obj");
    chair.initModel("resources/models/sceneParts/Chair.obj");
    table.initModel("resources/models/sceneParts/SmallTable.obj");
    frame.initModel("resources/models/geometric/frame.obj");


    // Imagenes (texturas)
    imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
    imgMiddleEmissive.initTexture("resources/textures/imgMiddleEmissive.png");

    imgWindow.initTexture("resources/textures/imgWindow.png");

    imgGround.initTexture("resources/textures/fondoSinCielo.png");
    imgSkyDay.initTexture("resources/textures/14.png"); //Color del cielo en función propia (prueba)
    imgSkyNight.initTexture("resources/textures/4.png");
    imgClouds.initTexture("resources/textures/nubes.png");

    asphaltDiffuse.initTexture("resources/textures/asphaltDiffuse.jpg");
    asphaltSpecular.initTexture("resources/textures/asphaltSpecular.png");
    asphaltNormal.initTexture("resources/textures/asphaltNormal.png");

    cammo.initTexture("resources/textures/cammo.jpg");

    trackDiffuse.initTexture("resources/textures/trackDiffuse.jpg");
    trackSpecular.initTexture("resources/textures/trackSpecular.jpg");
    trackNormal.initTexture("resources/textures/trackNormal.jpg");

    floorDiffuse.initTexture("resources/textures/sueloTerminalDifusa.jpg");
    floorSpecular.initTexture("resources/textures/sueloTerminalEspecular.jpg");
    floorNormal.initTexture("resources/textures/sueloTerminalNormal.jpg");

    grassDiffuse.initTexture("resources/textures/cespedDifusa.jpg");
    grassSpecular.initTexture("resources/textures/cespedEspecular.jpg");
    grassNormal.initTexture("resources/textures/cespedNormal.jpg");

    jetDiffuse.initTexture("resources/textures/jetBodyDiffuse.jpg");
    jetWingsDiffuse.initTexture("resources/textures/jetWingsDiffuse.jpg");

    fenceDiffuse.initTexture("resources/textures/metal02.png");
    fenceNormal.initTexture("resources/textures/metalN02.png");

    torreConcreteDiffuse.initTexture("resources/textures/concreteDiffuse.jpg");
    torreConcreteSpecular.initTexture("resources/textures/concreteSpecular.jpg");
    torreConcreteNormal.initTexture("resources/textures/concreteNormal.jpg");

    imgGlass.initTexture("resources/textures/darkWindowGlass.png");

    metAntenaDiffuse.initTexture("resources/textures/metalverdeDiffuse.jpg");
    metAntenaSpecular.initTexture("resources/textures/metalverdeSpecular.jpg");
    metAntenaNormal.initTexture("resources/textures/metalverdeNormal.jpg");

    sofaSpecular.initTexture("resources/textures/sofaSpecular.jpg");
    sofaDiffuse.initTexture("resources/textures/sofaDiffuse.png");
    sofaNormal.initTexture("resources/textures/sofaNormal.jpg");


    // Luz ambiental global
    lightG.ambient = glm::vec3(0.0, 0.0, 0.0);

 // Luces direccionales
    lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);  // Dirección hacia el suelo
    lightD[0].ambient   = glm::vec3(0.1, 0.1, 0.1);
    lightD[0].diffuse   = glm::vec3(0.7, 0.7, 0.7);
    lightD[0].specular  = glm::vec3(0.7, 0.7, 0.7);

 // Luces posicionales
    lightP[0].position    = glm::vec3(0.0, 17.0, 29.0);
    lightP[0].ambient     = glm::vec3(0.6, 0.6, 0.6);
    lightP[0].diffuse     = glm::vec3(0.6, 0.6, 0.6);
    lightP[0].specular    = glm::vec3(0.6, 0.6, 0.6);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Luces focales
    lightF[0].position    = glm::vec3(-8.0, 0.24, desZ-1.25);  // Posición del casquillo de la luz del morro del avión
    lightF[0].direction   = glm::normalize(glm::vec3(-8.0, 0.24, desZ-3.0) - lightF[0].position);  // Apunta al caza
    lightF[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightF[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightF[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 0.5;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;



 // Materiales
    texLoad();

}

void renderScene() {

    texLoad();
 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 50.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    //Restauración de movimiento orbital original: quitar las variables globales xCenter, yCenter, zCenter
    //float x = (6.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX)));
    //float y = 3.0f + (6.0f*glm::sin(glm::radians(alphaY)));
    //float z = 6.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (x, y, z); //Cambiar por (x, y, z)
    glm::vec3 center(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
    glm::mat4 M = I; //Sustituir I por producto de matrices en caso de necesidad de movimiento global de la escena

    glm::mat4 S = glm::scale(I, glm::vec3(0.4, 0.5, 0.4));
    glm::mat4 T = glm::translate(I, glm::vec3(-7.5, 0.00, -10.0));

    drawEntorno(P, V, M);
    drawAvion(P, V, M);
    drawVentanas(P, V, M);
    drawTorreControl(P,V,M * T * S);
    drawTerminal(P,V,M);

}


void drawEntorno(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 SGrass = glm::scale    (I, glm::vec3(25.0, 1.0, 25.0));
    glm::mat4 TGrass = glm::translate(I, glm::vec3(0.0, -0.01, 0.0));

    glm::mat4 Ry90 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,1,0));
    glm::mat4 SAsphalt = glm::scale    (I, glm::vec3(4.0, 1.0, 20.0));
    glm::mat4 SAsphalt2 = glm::scale    (I, glm::vec3(4.0, 1.0, 7.0));

    glm::mat4 SAsphalt3 = glm::scale    (I, glm::vec3(5.0, 1.0, 18.0));

    glm::mat4 TAsphalt = glm::translate(I, glm::vec3(6.0, -0.005, 12.0));
    glm::mat4 TAsphalt2 = glm::translate(I, glm::vec3(-6.0, -0.0025, 0.0));

    glm::mat4 TTrack1 = glm::translate(I, glm::vec3(5.5, 0.0, 0.0));
    glm::mat4 TTrack2 = glm::translate(I, glm::vec3(14.5, 0.0, 0.0));

    glm::mat4 TTerminal = glm::translate(I, glm::vec3(-18.0, 0.0, 10.0));
    glm::mat4 STerminal = glm::scale    (I, glm::vec3(5.0, 1.0, 10.0));

    drawCielo(P, V, M);

    drawObjectTex(plane, texGrass, P, V, M * TGrass * SGrass); //Suelo de césped

    drawObjectTex(plane, texTrack, P, V, M * TTrack1 * SAsphalt * Ry90); //Pistas de aterrizaje
    drawObjectTex(plane, texTrack, P, V, M * TTrack2 * SAsphalt * Ry90);

    drawObjectTex(plane, texAsphalt, P, V, M  * TAsphalt * Ry90 * SAsphalt2); //Pista transversal

    drawObjectTex(plane, texAsphalt, P, V, M * TAsphalt2 * SAsphalt3); //Pista de aparcamiento

    drawObjectTex(plane, texFloor, P, V, M * TTerminal * STerminal); //Suelo de la terminal planta 0

    drawVallas(P, V, M);
}


void drawAvion(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 SJet = glm::scale    (I, glm::vec3(0.002, 0.002, 0.002));
    glm::mat4 RJet = glm::rotate   (I, glm::radians(90.0f), glm::vec3(1,0,0));
    glm::mat4 Ry90 = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
    glm::mat4 MDefault = glm::translate(I,glm::vec3(-8.0, 0.24, desZ-1.25)) * glm::scale(I,glm::vec3(0.01));
    glm::mat4 TJet = glm::translate(I, glm::vec3(-8.0, 0.0, desZ));
    drawObjectTex(jet, texJet, P, V, M * TJet * Ry90 * RJet * SJet);
    drawObjectMat(sphere, mluz, P, V, MDefault);
}


void drawVentanas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 Rv = glm::rotate   (I, glm::radians(90.0f), glm::vec3(1,0,0));
    glm::mat4 Tv = glm::translate(I, glm::vec3(0.0, 0.0, 3.0));
    glDepthMask(GL_FALSE);
    //drawObjectTex(plane, texWindow, P, V, M * Tv * Rv);
    glDepthMask(GL_TRUE);
}

void drawVallas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 S2 = glm::scale(I, glm::vec3(0.5, 0.5, 0.25));
    glm::mat4 S3 = glm::scale(I, glm::vec3(0.5, 0.5, 0.61));
    glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0,1,0));
    float desX1 = -15.9;
    float desZ1 = 22.25;
    float desX2 = 20.0;
    float desZ2 = -21.25;
    glm::mat4 T;

    for(int i=0; i<= 24; i++){
        T = glm::translate(I, glm::vec3(desX1, 0.8, 23.0));
        drawObjectTex(fence, texFence, P, V, M * T * S); //Valla alambrada
        desX1 += 1.5;
    }

    for(int i=0; i<= 29; i++){
        T = glm::translate(I, glm::vec3(20.85, 0.8, desZ1));
        drawObjectTex(fence, texFence, P, V, M * T * S * R); //Valla alambrada
        desZ1 -= 1.5;
    }

    for(int i=0; i<= 24; i++){
        T = glm::translate(I, glm::vec3(desX2, 0.8, -22.0));
        drawObjectTex(fence, texFence, P, V, M * T * S); //Valla alambrada
        desX2 -= 1.5;
    }

    for(int i=0; i<= 13; i++){
        T = glm::translate(I, glm::vec3(-16.75, 0.8, desZ2));
        drawObjectTex(fence, texFence, P, V, M * T * S * R); //Valla alambrada
        desZ2 += 1.5;
    }

    T = glm::translate(I, glm::vec3(-16.75, 0.8, -0.60));
    drawObjectTex(fence, texFence, P, V, M * T * S2 * R);
    T = glm::translate(I, glm::vec3(-16.75, 0.8, 20.75));
    drawObjectTex(fence, texFence, P, V, M * T * S2 * R);
    T = glm::translate(I, glm::vec3(-16.75, 0.8, 22.1125));
    drawObjectTex(fence, texFence, P, V, M * T * S3 * R);

}

void drawCielo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //timer nubes
    float rotationSpeedMillis = 1.0f / 1000.0f;  // 1 degree every 1000 milliseconds
    float rotationAngle = glm::radians(fmod(glfwGetTime() * rotationSpeedMillis * 1000.0f, 360.0f));
    glm::mat4 RNubes = glm::rotate(I, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 S1 = glm::scale    (I, glm::vec3(16.0, 16.0, 16.0));
    glm::mat4 S2 = glm::scale    (I, glm::vec3(16.1, 16.1, 16.1));
    glm::mat4 S3 = glm::scale    (I, glm::vec3(16.2, 16.2, 16.2));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 3.0, 0.0));
    drawObjectTex(sphere, texSky, P, V, M * T * S3);
    glDepthMask(GL_FALSE);
    drawObjectTex(sphere, texClouds, P, V, M * RNubes * T * S2); //Esfera que contiene al fondo de nubes
    glDepthMask(GL_TRUE);
    glDepthMask(GL_FALSE);
    drawObjectTex(sphere, texGround, P, V, M * T * S1); //Esfera que contiene al fondo de vegetación
    glDepthMask(GL_TRUE);
}

void drawTorreControl (glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S1 = glm::scale    (I, glm::vec3(2.0, 7.0, 2.0)); //para que no aparezca por debajo, el número de la y
    glm::mat4 T1 = glm::translate(I, glm::vec3(-16.0, 7.0, -10.0)); // tiene que coincidir en las dos matrices
    drawObjectTex(torre1, texConcrete, P, V, M * T1 * S1);

    glm::mat4 S3 = glm::scale    (I, glm::vec3(4.0, 2.0, 4.0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(-16.0, 20.25, -10.0));
    drawObjectTex(cone, texConcrete, P, V, M * T3 * S3);


    //timmer antena
    float rotationSpeedMillis = 2.0f / 100.0f;  // 2 degrees every 10 milliseconds
    float rotationAngle = glm::radians(fmod(glfwGetTime() * rotationSpeedMillis * 1000.0f, 360.0f));

    glm::mat4 SRotateAntena = glm::rotate(I, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 TAntena = glm::translate(I, glm::vec3(-16.0, 22.75, -10.0));
    glm::mat4 S4 = glm::scale(I, glm::vec3(0.7, 0.7, 0.7));
    glm::mat4 rotatedAntenaModel = M * TAntena * SRotateAntena;

    drawObjectTex(antena, texFence, P, V, rotatedAntenaModel * S4);


    glm::mat4 S5 = glm::scale    (I, glm::vec3(4.0, 0.25, 4.0));
    glm::mat4 T5 = glm::translate(I, glm::vec3(-16.0, 14.0, -10.0));
    drawObjectTex(torre1, texConcrete, P, V, M * T5 * S5); //Base de cemento que tiene por debajo el vidrio de la torre

    glm::mat4 S2 = glm::scale    (I, glm::vec3(4.0, 2.0, 4.0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(-16.0, 16.26, -10.0));
    glDepthMask(GL_FALSE);
    drawObjectTex(torre1, texGlass, P, V, M * T2 * S2);
    glDepthMask(GL_TRUE);

}

void drawTerminal (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 SSofa = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));  // Reducir el tamaño del sofá
    glm::mat4 TSofa = glm::translate(I, glm::vec3(-22.0, -0.20, 17.90));
    glm::mat4 RSofa = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  // Rotar 90 grados a la izquierda
    drawObjectTex(sofa, texTelaSofa, P, V, M * TSofa * SSofa * RSofa);

    glm::mat4 TSofa1 = glm::translate(I, glm::vec3(-22.0, -0.20, 2));
    drawObjectTex(sofa, texTelaSofa, P, V, M * TSofa1 * SSofa * RSofa);

    glm::mat4 SSilla = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));  // Reducir el tamaño del sofá
    glm::mat4 TSilla = glm::translate(I, glm::vec3(-20.0, 1, 10));
    glm::mat4 RSilla= glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  // Rotar 90 grados a la izquierda
    drawObjectTex(chair, texTelaSofa, P, V, M * TSilla * SSilla * RSilla);

    glm::mat4 SSilla2 = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));  // Reducir el tamaño del sofá
    glm::mat4 TSilla2 = glm::translate(I, glm::vec3(-20.0, 1, 7));
    glm::mat4 RSilla2= glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));  // Rotar 90 grados a la izquierda
    drawObjectTex(chair, texTelaSofa, P, V, M * TSilla2 * SSilla2 * RSilla2);


    glm::mat4 SMesa = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));  // Reducir el tamaño del sofá
    glm::mat4 TMesa = glm::translate(I, glm::vec3(-20.0, 1, 8.5));
    //glm::mat4 RMesa= glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));  // Rotar 90 grados a la izquierda
    drawObjectTex(table, texTelaSofa, P, V, M * TMesa * SMesa );


    glm::mat4 SFrame1 = glm::scale(I, glm::vec3(0.1, 0.181, 0.095));  // Reducir el tamaño del sofá
    glm::mat4 Tframe;
    float initialDesZ = 18.15;
    for(int i=1; i<=5; i++){
        Tframe = glm::translate(I, glm::vec3(-13.0, 4.201, initialDesZ));
        drawObjectMat(frame, mFrame, P, V, M * Tframe * SFrame1);
        initialDesZ -= 4.0;
    }
    initialDesZ = 18.15;
    for(int i=1; i<=5; i++){
        Tframe = glm::translate(I, glm::vec3(-23.1, 4.203, initialDesZ));
        drawObjectMat(frame, mFrame, P, V, M * Tframe * SFrame1);
        initialDesZ -= 4.0;
    }

    glm::mat4 SFrame2 = glm::scale(I, glm::vec3(0.114, 0.181, 0.095));  // Reducir el tamaño del sofá
    float initialDesX = -15.45;
    for(int i=1; i<=2; i++){
        Tframe = glm::translate(I, glm::vec3(initialDesX, 4.201, 0.0));
        glm::mat4 Ry90 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,1,0));
        drawObjectMat(frame, mFrame, P, V, M * Tframe * SFrame2 * Ry90);
        initialDesX -= 5.0;
    }

    initialDesX = -15.45;
    for(int i=1; i<=2; i++){
        Tframe = glm::translate(I, glm::vec3(initialDesX, 4.201, 20.1));
        glm::mat4 Ry90 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,1,0));
        drawObjectMat(frame, mFrame, P, V, M * Tframe * SFrame2 * Ry90);
        initialDesX -= 5.0;
    }

    glm::mat4 STerminal = glm::scale(I, glm::vec3(5.2, 0.3, 10.15));
    glm::mat4 TTerminal = glm::translate(I, glm::vec3(-18.0, 8.2, 10.1));
    drawObjectMat(cubeTerminal, mFrame, P, V, M * TTerminal * STerminal);

    glm::mat4 STerminal2 = glm::scale(I, glm::vec3(5.0, 4.0, 10.0));
    glm::mat4 TTerminal2 = glm::translate(I, glm::vec3(-18.0, 3.995, 10.0));
    glDepthMask(GL_FALSE);
    drawObjectTex(cubeTerminal, texGlass, P, V, M * TTerminal2 * STerminal2);
    glDepthMask(GL_TRUE);

}

void setLights(glm::mat4 P, glm::mat4 V) {
    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 MDefault = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        glm::mat4 MLight0 = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.3));
        if(i==0){
            drawObjectMat(sphere, mluz, P, V, MLight0);
        }else{
            drawObjectMat(sphere, mluz, P, V, MDefault);
        }


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
        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;

        //Auxiliar
        case GLFW_KEY_W:  y += 1.0f;   break;
        case GLFW_KEY_S:  y -= 1.0f;   break;
        case GLFW_KEY_A:  x -= 1.0f;   break;
        case GLFW_KEY_D:  x += 1.0f;   break;
        case GLFW_KEY_E:  z += 1.0f;   break;
        case GLFW_KEY_Q:  z -= 1.0f;   break;
        case GLFW_KEY_N:
            if(action == GLFW_PRESS){
                nightMode = !nightMode;
            } else{
                nightMode = nightMode;
            }
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

void texLoad(){
    if(nightMode){
        mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.emissive  = glm::vec4(0.3, 0.3, 0.3, 0.3);
        mluz.shininess = 1.0;

        mFrame.ambient   = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
        mFrame.diffuse   = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        mFrame.specular  = glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f);
        mFrame.emissive  = glm::vec4(0.3, 0.3, 0.3, 0.3);
        mFrame.shininess = 76.8f;

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

        texFloor.diffuse    = floorDiffuse.getTexture();
        texFloor.specular   = floorSpecular.getTexture();
        texFloor.emissive   = imgNoEmissive.getTexture();
        texFloor.normal     = floorNormal.getTexture();
        texFloor.shininess  = 51.2;

        texGrass.diffuse    = grassDiffuse.getTexture();
        texGrass.specular   = grassSpecular.getTexture();
        texGrass.emissive   = imgNoEmissive.getTexture();
        texGrass.normal     = grassNormal.getTexture();
        texGrass.shininess  = 51.2;

        texJet.diffuse    = jetDiffuse.getTexture();
        texJet.specular = jetDiffuse.getTexture();
        texJet.emissive   = imgNoEmissive.getTexture();
        texJet.normal     = 0;
        texJet.shininess  = 51.2;

        texFence.diffuse    = fenceDiffuse.getTexture();
        texFence.specular   = fenceDiffuse.getTexture();
        texFence.emissive   = imgNoEmissive.getTexture();
        texFence.normal     = fenceNormal.getTexture();
        texFence.shininess  = 11.2;

        texGround.diffuse    = imgGround.getTexture();
        texGround.specular = imgGround.getTexture();
        texGround.emissive   = imgNoEmissive.getTexture();
        texGround.normal     = 0;
        texGround.shininess  = 51.2;

        texSky.diffuse    = imgSkyDay.getTexture();
        texSky.specular = imgSkyDay.getTexture();
        texSky.emissive   = imgNoEmissive.getTexture();
        texSky.normal     = 0;
        texSky.shininess  = 51.2;

        texClouds.diffuse    = imgClouds.getTexture();
        texClouds.specular = imgClouds.getTexture();
        texClouds.emissive   = imgNoEmissive.getTexture();
        texClouds.normal     = 0;
        texClouds.shininess  = 51.2;

        texConcrete.diffuse    = torreConcreteDiffuse.getTexture();
        texConcrete.specular = torreConcreteSpecular.getTexture();
        texConcrete.emissive   = imgNoEmissive.getTexture();
        texConcrete.normal     = torreConcreteNormal.getTexture();
        texConcrete.shininess  = 51.2;

        texGlass.diffuse   = imgGlass.getTexture();
        texGlass.specular  = imgGlass.getTexture();
        texGlass.emissive  = imgNoEmissive.getTexture();
        texGlass.normal    = 0;
        texGlass.shininess = 10.0;

        texMetalverde.diffuse = metAntenaDiffuse.getTexture();
        texMetalverde.specular= metAntenaSpecular.getTexture();
        texMetalverde.emissive   = imgNoEmissive.getTexture();
        texMetalverde.normal = metAntenaNormal.getTexture();
        texMetalverde.shininess  = 51.2;

        texTelaSofa.diffuse = sofaDiffuse.getTexture();
        texTelaSofa.specular= sofaSpecular.getTexture();
        texTelaSofa.emissive   = imgNoEmissive.getTexture();
        texTelaSofa.normal = sofaNormal.getTexture();
        texTelaSofa.shininess  = 51.2;

    }
    else{

        mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
        mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
        mluz.shininess = 1.0;

        mFrame.ambient   = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
        mFrame.diffuse   = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        mFrame.specular  = glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f);
        mFrame.emissive  = glm::vec4(0.3, 0.3, 0.3, 0.3);
        mFrame.shininess = 76.8f;

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
        texCammo.emissive   = cammo.getTexture();
        texCammo.normal     = 0;
        texCammo.shininess  = 51.2;

        texTrack.diffuse    = trackDiffuse.getTexture();
        texTrack.specular   = trackSpecular.getTexture();
        texTrack.emissive   = imgNoEmissive.getTexture();
        texTrack.normal     = trackNormal.getTexture();
        texTrack.shininess  = 51.2;

        texFloor.diffuse    = floorDiffuse.getTexture();
        texFloor.specular   = floorSpecular.getTexture();
        texFloor.emissive   = imgNoEmissive.getTexture();
        texFloor.normal     = floorNormal.getTexture();
        texFloor.shininess  = 51.2;

        texGrass.diffuse    = grassDiffuse.getTexture();
        texGrass.specular   = grassSpecular.getTexture();
        texGrass.emissive   = imgNoEmissive.getTexture();
        texGrass.normal     = grassNormal.getTexture();
        texGrass.shininess  = 51.2;

        texJet.diffuse    = jetDiffuse.getTexture();
        texJet.specular = jetDiffuse.getTexture();
        texJet.emissive   = jetDiffuse.getTexture();
        texJet.normal     = 0;
        texJet.shininess  = 51.2;

        texFence.diffuse    = fenceDiffuse.getTexture();
        texFence.specular   = fenceDiffuse.getTexture();
        texFence.emissive   = imgMiddleEmissive.getTexture();
        texFence.normal     = fenceNormal.getTexture();
        texFence.shininess  = 11.2;

        texGround.diffuse    = imgGround.getTexture();
        texGround.specular = imgGround.getTexture();
        texGround.emissive   = imgGround.getTexture();
        texGround.normal     = 0;
        texGround.shininess  = 51.2;

        texSky.diffuse    = imgSkyDay.getTexture();
        texSky.specular = imgSkyDay.getTexture();
        texSky.emissive   = imgSkyDay.getTexture();
        texSky.normal     = 0;
        texSky.shininess  = 51.2;

        texClouds.diffuse    = imgClouds.getTexture();
        texClouds.specular = imgClouds.getTexture();
        texClouds.emissive   = imgClouds.getTexture();
        texClouds.normal     = 0;
        texClouds.shininess  = 51.2;

        texConcrete.diffuse    = torreConcreteDiffuse.getTexture();
        texConcrete.specular = torreConcreteSpecular.getTexture();
        texConcrete.emissive   = imgNoEmissive.getTexture();
        texConcrete.normal     = torreConcreteNormal.getTexture();
        texConcrete.shininess  = 51.2;

        texGlass.diffuse   = imgGlass.getTexture();
        texGlass.specular  = imgGlass.getTexture();
        texGlass.emissive  = imgGlass.getTexture();
        texGlass.normal    = 0;
        texGlass.shininess = 10.0;

        texMetalverde.diffuse = metAntenaDiffuse.getTexture();
        texMetalverde.specular= metAntenaSpecular.getTexture();
        texMetalverde.emissive   = metAntenaDiffuse.getTexture();
        texMetalverde.normal = metAntenaNormal.getTexture();
        texMetalverde.shininess  = 51.2;

        texTelaSofa.diffuse = sofaDiffuse.getTexture();
        texTelaSofa.specular= sofaSpecular.getTexture();
        texTelaSofa.emissive   = sofaDiffuse.getTexture();
        texTelaSofa.normal = sofaNormal.getTexture();
        texTelaSofa.shininess  = 51.2;

    }

}
