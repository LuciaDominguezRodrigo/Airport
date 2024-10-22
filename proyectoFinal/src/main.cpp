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

void movingLightsLoad();
void staticLightsLoad();
void drawEntorno(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAvion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
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
   Model jetBody;
   Model jetWings;
   Model fence;
   Model torre1;
   Model cone;
   Model antena;
   Model cubeTerminal;
   Model sofa;
   Model chair;
   Model table;
   Model frame; //Marco del que se construye la estructura metalica de la terminal
   Model farola;
   Model helice;
   Model ruedas;
   Model soporteRuedas;
   Model soporteRuedaDelantera; //Mismo modelo que soporteRuedas, pero con la barra central de aluminio un poco mas alargada


// Imagenes (texturas)
   Texture imgNoEmissive;

   Texture imgGround; //Imagen de fondo de vegetacion
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

   Texture floorDiffuse; //Suelo de terminal
   Texture floorSpecular;
   Texture floorNormal;

   Texture grassDiffuse; //Suelo de cesped
   Texture grassSpecular;
   Texture grassNormal;

   Texture jetBodyDiffuse; //Wrap del fuselaje del avion
   Texture jetWingsDiffuse; //Wrap del modulo de alas y motores del avion

   Texture fenceDiffuse;
   Texture fenceNormal;

   Texture torreConcreteDiffuse; //Hormigon de la torre de control
   Texture torreConcreteSpecular;
   Texture torreConcreteNormal;

   Texture imgGlass;

   Texture metAntenaDiffuse; //Metal de los flaps
   Texture metAntenaNormal;
   Texture metAntenaSpecular;

   Texture sofaDiffuse;
   Texture sofaSpecular;
   Texture sofaNormal;

   Texture ruedasDiffuse;
   Texture ruedasNormal;


// Luces y materiales
   #define   NLD 1
   #define   NLP 4
   #define   NLF 2
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  mSol;
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
   Textures  texJetBody;
   Textures  texJetWings;
   Textures  texFence;
   Textures  texConcrete;
   Textures  texGlass;
   Textures  texMetalverde;
   Textures  texTelaSofa;
   Textures  texRuedas;


// Modo día/noche
   bool nightMode = false;

// Viewport
   int w = 500;
   int h = 500;

// Desplazamiento por el mundo
    float desX = 0.0;
    float desY = 0.0;
    float desZ = 0.0;

// Movimiento orbitalde camara
   float fovy   = 60.0;
   float alphaX =  0.0;
   float alphaY =  0.0;

//Variables auxiliares temporales para control de cámara
    float x   = 0.0;
    float y =  0.0;
    float z =  0.0;

//Angulo de rotacion de las ruedas del avion
    float wheelRotAngle = 0.0;
    float planeRotAngle = 90.0;
    glm::mat4 planeRotationMatrix;

//Movimiento del tren de aterrizaje
    int contador = 0;
    bool gearUp = false;
    float actualTime;

//Movimiento de flaps
    bool flapsUp = false;

//Activar o desactivar el movimiento orbital de la cámara centrado en el origen con el ratón
    bool mouseMovement = true;

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
    jetBody.initModel("resources/models/planes/fuselaje.obj");
    jetWings.initModel("resources/models/planes/alas.obj");
    fence.initModel("resources/models/sceneParts/fence.obj");
    torre1.initModel("resources/models/geometric/cylinder.obj");
    cone.initModel("resources/models/geometric/cone.obj");
    antena.initModel("resources/models/sceneParts/antena.obj");
    sofa.initModel("resources/models/sceneParts/sofa.obj");
    chair.initModel("resources/models/sceneParts/Chair.obj");
    table.initModel("resources/models/sceneParts/SmallTable.obj");
    frame.initModel("resources/models/geometric/frame.obj");
    farola.initModel("resources/models/sceneParts/farola.obj");
    helice.initModel("resources/models/planeParts/helice.obj");
    ruedas.initModel("resources/models/planeParts/ruedas.obj");
    soporteRuedas.initModel("resources/models/planeParts/soporteRuedas.obj");
    soporteRuedaDelantera.initModel("resources/models/planeParts/soporteRuedaDelantera.obj");


    // Imagenes (texturas)
    imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");

    imgGround.initTexture("resources/textures/fondoSinCielo.png");
    imgSkyDay.initTexture("resources/textures/14.png");
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

    jetBodyDiffuse.initTexture("resources/textures/jetBodyDiffuse.jpg");
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

    ruedasDiffuse.initTexture("resources/textures/tire01.png");
    ruedasNormal.initTexture("resources/textures/tireN01.png");

    staticLightsLoad();

    // Luces posicionales (farolas)
    lightP[0].position    = glm::vec3(19.85, 2.65, 20.0); //Farola 1 (esquina)
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.5, 0.5, 0.5);
    lightP[0].specular    = glm::vec3(0.5, 0.5, 0.5);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

    lightP[1].position    = glm::vec3(19.85, 2.65, -20.0); //Farola 2 (esquina)
    lightP[1].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[1].diffuse     = glm::vec3(0.5, 0.5, 0.5);
    lightP[1].specular    = glm::vec3(0.5, 0.5, 0.5);
    lightP[1].c0          = 1.00;
    lightP[1].c1          = 0.22;
    lightP[1].c2          = 0.20;

    lightP[2].position    = glm::vec3(-13.0, 2.65, -7.0); //Farola 3 (entre torre y terminal)
    lightP[2].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[2].diffuse     = glm::vec3(0.5, 0.5, 0.5);
    lightP[2].specular    = glm::vec3(0.5, 0.5, 0.5);
    lightP[2].c0          = 1.00;
    lightP[2].c1          = 0.22;
    lightP[2].c2          = 0.20;


    // Luces focales (dentro de la terminal)
    lightF[0].position    = glm::vec3(-18.0, 7.8, 10.1);
    lightF[0].direction   = glm::vec3(-18.0, 0.0, 10.1) - lightF[0].position;
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 1.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

    lightF[1].position    = glm::vec3(-18.0, 7.8, 15.1);
    lightF[1].direction   = glm::vec3(-18.0, 0.0, 15.1) -  lightF[1].position;
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 10.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

    movingLightsLoad();

 // Materiales y texturas

    mSol.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.emissive  = glm::vec4(0.5, 0.5, 0.5, 1.0);
    mSol.shininess = 1.0;

    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    mFrame.ambient   = glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f );
    mFrame.diffuse   = glm::vec4(0.50754f, 0.50754f, 0.50754f, 1.0f);
    mFrame.specular  = glm::vec4(0.508273f, 0.508273f, 0.508273f, 1.0f);
    mFrame.emissive  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mFrame.shininess = 51.2f;

    texAsphalt.diffuse    = asphaltDiffuse.getTexture();
    texAsphalt.specular   = asphaltSpecular.getTexture();
    texAsphalt.emissive   = imgNoEmissive.getTexture();
    texAsphalt.normal     = 0;
    texAsphalt.shininess  = 51.2;

    texCammo.diffuse    = cammo.getTexture();
    texCammo.specular   = cammo.getTexture();
    texCammo.emissive   = imgNoEmissive.getTexture();
    texCammo.normal     = 0;
    texCammo.shininess  = 51.2;

    texTrack.diffuse    = trackDiffuse.getTexture();
    texTrack.specular   = trackSpecular.getTexture();
    texTrack.emissive   = imgNoEmissive.getTexture();
    texTrack.normal     = 0;
    texTrack.shininess  = 51.2;

    texFloor.diffuse    = floorDiffuse.getTexture();
    texFloor.specular   = floorSpecular.getTexture();
    texFloor.emissive   = imgNoEmissive.getTexture();
    texFloor.normal     = 0;
    texFloor.shininess  = 51.2;

    texGrass.diffuse    = grassDiffuse.getTexture();
    texGrass.specular   = grassSpecular.getTexture();
    texGrass.emissive   = imgNoEmissive.getTexture();
    texGrass.normal     = 0;
    texGrass.shininess  = 51.2;

    texJetBody.diffuse    = jetBodyDiffuse.getTexture();
    texJetBody.specular = jetBodyDiffuse.getTexture();
    texJetBody.emissive   = imgNoEmissive.getTexture();
    texJetBody.normal     = 0;
    texJetBody.shininess  = 51.2;

    texJetWings.diffuse    = jetWingsDiffuse.getTexture();
    texJetWings.specular = jetWingsDiffuse.getTexture();
    texJetWings.emissive   = imgNoEmissive.getTexture();
    texJetWings.normal     = 0;
    texJetWings.shininess  = 51.2;

    texFence.diffuse    = fenceDiffuse.getTexture();
    texFence.specular   = fenceDiffuse.getTexture();
    texFence.emissive   = imgNoEmissive.getTexture();
    texFence.normal     = 0;
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
    texConcrete.normal     = 0;
    texConcrete.shininess  = 51.2;

    texGlass.diffuse   = imgGlass.getTexture();
    texGlass.specular  = imgGlass.getTexture();
    texGlass.emissive  = imgNoEmissive.getTexture();
    texGlass.normal    = 0;
    texGlass.shininess = 10.0;

    texMetalverde.diffuse = metAntenaDiffuse.getTexture();
    texMetalverde.specular= metAntenaSpecular.getTexture();
    texMetalverde.emissive   = imgNoEmissive.getTexture();
    texMetalverde.normal = 0;
    texMetalverde.shininess  = 51.2;

    texTelaSofa.diffuse = sofaDiffuse.getTexture();
    texTelaSofa.specular= sofaSpecular.getTexture();
    texTelaSofa.emissive   = imgNoEmissive.getTexture();
    texTelaSofa.normal = 0;
    texTelaSofa.shininess  = 51.2;

    texRuedas.diffuse = ruedasDiffuse.getTexture();
    texRuedas.specular = ruedasDiffuse.getTexture();
    texRuedas.emissive = imgNoEmissive.getTexture();
    texRuedas.normal = 0;
    texRuedas.shininess  = 51.2;

}

void renderScene() {

    staticLightsLoad();
    movingLightsLoad();

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
    if(mouseMovement){
        x = (6.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX)));
        y = 3.0f + (6.0f*glm::sin(glm::radians(alphaY)));
        z = 6.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    }
    glm::vec3 eye   (x, y, z);
    glm::vec3 center(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
    glm::mat4 M = I;

    glm::mat4 S = glm::scale(I, glm::vec3(0.4, 0.5, 0.4));
    glm::mat4 T = glm::translate(I, glm::vec3(-7.5, 0.00, -10.0));

    drawEntorno(P, V, M);
    drawAvion(P, V, M);
    drawTorreControl(P,V,M * T * S);
    drawTerminal(P,V,M);

}


void drawEntorno(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 SGrass = glm::scale    (I, glm::vec3(25.0, 1.0, 25.0));
    glm::mat4 TGrass1 = glm::translate(I, glm::vec3(0.0, -0.01, 0.0));
    glm::mat4 TGrass2 = glm::translate(I, glm::vec3(0.0, -0.02, 0.0));
    glm::mat4 Rx180 = glm::rotate   (I, glm::radians(180.0f), glm::vec3(1,0,0));

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

    glm::mat4 TFarola1 = glm::translate(I, glm::vec3(19.85, 0.0, 20.0));
    glm::mat4 TFarola2 = glm::translate(I, glm::vec3(19.85, 0.0, -20.0));
    glm::mat4 TFarola3 = glm::translate(I, glm::vec3(-13.0, 0.0, -7.0));
    glm::mat4 SFarola = glm::scale    (I, glm::vec3(0.15, 0.15, 0.15));

    drawCielo(P, V, M);

    drawObjectTex(plane, texGrass, P, V, M * TGrass1 * SGrass); //Suelo de césped
    drawObjectTex(plane, texGrass, P, V, M * TGrass2 * SGrass * Rx180); //Suelo de césped invertido

    drawObjectTex(plane, texTrack, P, V, M * TTrack1 * SAsphalt * Ry90); //Pistas de aterrizaje
    drawObjectTex(plane, texTrack, P, V, M * TTrack2 * SAsphalt * Ry90);

    drawObjectTex(plane, texAsphalt, P, V, M  * TAsphalt * Ry90 * SAsphalt2); //Pista transversal

    drawObjectTex(plane, texAsphalt, P, V, M * TAsphalt2 * SAsphalt3); //Pista de aparcamiento

    drawObjectTex(plane, texFloor, P, V, M * TTerminal * STerminal); //Suelo de la terminal planta 0

    drawObjectTex(farola, texConcrete, P, V, M * TFarola1 * SFarola); //Farola esquina 1
    drawObjectTex(farola, texConcrete, P, V, M * TFarola2 * SFarola); //Farola esquina 2
    drawObjectTex(farola, texConcrete, P, V, M * TFarola3 * SFarola); //Farola esquina 3

    drawVallas(P, V, M);
}


void drawAvion(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    planeRotationMatrix = glm::translate(I, glm::vec3(-6.0+desX, 0.0, 4.0+desZ)) * glm::rotate(I, glm::radians(planeRotAngle-90.0f), glm::vec3(0,1,0)) * glm::rotate(I, glm::radians(180.0f), glm::vec3(0,1,0));
    M *= planeRotationMatrix;

    glm::mat4 SJet = glm::scale    (I, glm::vec3(0.002, 0.002, 0.002));
    glm::mat4 RJet = glm::rotate   (I, glm::radians(90.0f), glm::vec3(1,0,0));
    glm::mat4 Ry180 = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
    glm::mat4 MDefault = glm::translate(I,glm::vec3(-8.0, 0.24, -1.25)) * glm::scale(I,glm::vec3(0.01));
    glm::mat4 TJet = glm::translate(I, glm::vec3(0.0, 0.0, 0.00));
    glm::mat4 SHelice = glm::scale    (I, glm::vec3(0.091, 0.091, 0.091));
    glm::mat4 THelice1 = glm::translate(I, glm::vec3(0.32, 0.385, 0.25));
    glm::mat4 THelice2 = glm::translate(I, glm::vec3(-0.32, 0.385, 0.25));

    glm::mat4 SRuedas = glm::scale    (I, glm::vec3(0.05, 0.05, 0.05));
    glm::mat4 TRuedas1 = glm::translate(I, glm::vec3(0.25, 0.05, 0.00));
    glm::mat4 TRuedas2 = glm::translate(I, glm::vec3(-0.25, 0.05, 0.00));
    glm::mat4 TRuedaDel = glm::translate(I, glm::vec3(0.0, 0.05, -1.00));

    glm::mat4 TAux1 = glm::translate(I, glm::vec3(0.0, -0.16, 0.0)); //Matrices auxiliares de traslación para poder rotar el tren de aterrizaje sobre su eje
    glm::mat4 TAux2 = glm::translate(I, glm::vec3(0.0, 0.16, 0.0));

    glm::mat4 RRuedas = glm::rotate   (I, glm::radians(wheelRotAngle), glm::vec3(1, 0, 0));

    //timer turbina
    float rotationSpeedMillis = 2.0f / 5.0f;  // 2 degrees every 5 milliseconds
    float rotationAngle = glm::radians(fmod(glfwGetTime() * rotationSpeedMillis * 1000.0f, 360.0f));
    glm::mat4 RHelice = glm::rotate   (I, rotationAngle, glm::vec3(0,0,1));

    drawObjectTex(jetBody, texJetBody, P, V, M * TJet * Ry180 * RJet * SJet);
    drawObjectTex(jetWings, texJetWings, P, V, M * TJet * Ry180 * RJet * SJet);
    drawObjectTex(helice, texCammo, P, V, M * THelice1 * RHelice * Ry180 * SHelice);
    drawObjectTex(helice, texCammo, P, V, M * THelice2 * RHelice * Ry180 * SHelice);

    glm::mat4 RTrenAterrizaje1;
    glm::mat4 RTrenAterrizaje2;
    glm::mat4 RTrenAterrizajeDel;
    if(gearUp){
        if(actualTime == 0.0){
            actualTime = glfwGetTime();
            contador = 0;
        }
        float rotationSpeedMillis = 2.0f / 25.0f;  // 2 degrees every 25 milliseconds
        float rotationAngle = glm::radians(fmod((glfwGetTime()-actualTime) * rotationSpeedMillis * 1000.0f, 105.0f));
        if(rotationAngle >= 1.8f){
            contador += 1;
        }
        if (contador == 0){
            RTrenAterrizaje1 = glm::rotate   (I, -rotationAngle, glm::vec3(0, 0, 1));
            RTrenAterrizaje2 = glm::rotate   (I, rotationAngle, glm::vec3(0, 0, 1));
            RTrenAterrizajeDel = glm::rotate   (I, -rotationAngle-0.02f, glm::vec3(1, 0, 0)); //Ajuste de 0.02 radianes (aproximadamente 1.2 grados) respecto al original debido a problema de oculatación de la rueda delantera
        }else{
            RTrenAterrizaje1 = glm::rotate   (I, glm::radians(-105.0f), glm::vec3(0, 0, 1));
            RTrenAterrizaje2 = glm::rotate   (I, glm::radians(105.0f), glm::vec3(0, 0, 1));
            RTrenAterrizajeDel = glm::rotate   (I, glm::radians(-105.0f)-0.02f, glm::vec3(1, 0, 0));
        }
    }
    else{
        if(actualTime == 0.0){
            actualTime = glfwGetTime();
            contador = 0;
        }
        float rotationSpeedMillis = 2.0f / 25.0f;  // 2 degrees every 25 milliseconds
        float rotationAngle = glm::radians(fmod((glfwGetTime()-actualTime) * rotationSpeedMillis * 1000.0f, 105.0f));
        if(rotationAngle <= 0.2f){
            contador += 1;
        }
        if (contador == 0){
            RTrenAterrizaje1 = glm::rotate   (I, -(1.83f - rotationAngle), glm::vec3(0, 0, 1));
            RTrenAterrizaje2 = glm::rotate   (I, 1.83f - rotationAngle, glm::vec3(0, 0, 1));
            RTrenAterrizajeDel = glm::rotate   (I, -(1.83f - rotationAngle), glm::vec3(1, 0, 0));
        }else{
            RTrenAterrizaje1 = glm::rotate   (I, glm::radians(-0.0f), glm::vec3(0, 0, 1));
            RTrenAterrizaje2 = glm::rotate   (I, glm::radians(0.0f), glm::vec3(0, 0, 1));
            RTrenAterrizajeDel = glm::rotate   (I, glm::radians(-0.0f), glm::vec3(1, 0, 0));
            actualTime = 0.0;
        }
    }

    drawObjectTex(soporteRuedas, texFence, P, V, M * TRuedas1 * TAux2 * RTrenAterrizaje1 * TAux1 * SRuedas);
    drawObjectTex(ruedas, texRuedas, P, V, M * TRuedas1 * TAux2 * RTrenAterrizaje1 * TAux1 * SRuedas * RRuedas);
    drawObjectTex(soporteRuedas, texFence, P, V, M * TRuedas2 * TAux2 * RTrenAterrizaje2 * TAux1 * SRuedas);
    drawObjectTex(ruedas, texRuedas, P, V, M * TRuedas2 * TAux2 * RTrenAterrizaje2 * TAux1 * SRuedas * RRuedas);

    drawObjectTex(soporteRuedaDelantera, texFence, P, V, M * TRuedaDel * TAux2 * RTrenAterrizajeDel * TAux1 * SRuedas);
    drawObjectTex(ruedas, texRuedas, P, V, M * TRuedaDel * TAux2 * RTrenAterrizajeDel * TAux1 * SRuedas * RRuedas);

    glm::mat4 RFlaps1 = glm::rotate   (I, glm::radians(10.0f) , glm::vec3(0, 1, 0));
    glm::mat4 RFlaps1Inv = glm::rotate   (I, glm::radians(-10.0f) , glm::vec3(0, 1, 0));
    glm::mat4 RFlaps2 = glm::rotate   (I, glm::radians(-4.0f) , glm::vec3(0, 0, 1));
    glm::mat4 RFlaps2Inv = glm::rotate   (I, glm::radians(4.0f) , glm::vec3(0, 0, 1));
    glm::mat4 SFlaps = glm::scale(I, glm::vec3(0.25, 0.5, 0.05));
    glm::mat4 TFlaps = glm::translate(I, glm::vec3(-1.0, 0.28, 0.35));
    glm::mat4 TFlapsInv = glm::translate(I, glm::vec3(1.0, 0.28, 0.35));
    glm::mat4 TAux1Flaps = glm::translate(I, glm::vec3(0.0, 0.0, 0.05));
    glm::mat4 TAux2Flaps = glm::translate(I, glm::vec3(0.0, 0.0, -0.05));
    glm::mat4 RAuxFlaps;

    if(flapsUp){
        RAuxFlaps = glm::rotate   (I, glm::radians(-45.0f) , glm::vec3(1,0,0));
    }else{
        RAuxFlaps = I;
    }

    drawObjectTex(plane, texMetalverde ,P, V, M * TFlaps * RFlaps2 * RFlaps1 * TAux2Flaps * RAuxFlaps * TAux1Flaps * SFlaps);
    drawObjectTex(plane, texMetalverde ,P, V, M * TFlapsInv * RFlaps2Inv * RFlaps1Inv * TAux2Flaps * RAuxFlaps * TAux1Flaps * SFlaps);
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
    //Timer nubes
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

    glm::mat4 MSol = glm::translate(I,glm::vec3(0.0, 17.0, 29.0)) * glm::scale(I,glm::vec3(0.3)); //No es ninguna luz, sino únicamente una esfera simulando el sol y la luna
    drawObjectMat(sphere, mSol, P, V, MSol);
}

void drawTorreControl (glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S1 = glm::scale    (I, glm::vec3(2.0, 7.0, 2.0));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-16.0, 7.0, -10.0));
    drawObjectTex(torre1, texConcrete, P, V, M * T1 * S1);

    glm::mat4 S3 = glm::scale    (I, glm::vec3(4.0, 2.0, 4.0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(-16.0, 20.25, -10.0));
    drawObjectTex(cone, texConcrete, P, V, M * T3 * S3);


    //Timer antena
    float rotationSpeedMillis = 2.0f / 100.0f;  // 2 degrees every 100 milliseconds
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
    drawObjectTex(torre1, texGlass, P, V, M * T2 * S2); //Vidrio de la torre de control
    glDepthMask(GL_TRUE);

}

void drawTerminal (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 SSofa = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 TSofa = glm::translate(I, glm::vec3(-22.0, -0.20, 17.90));
    glm::mat4 RSofa = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
    drawObjectTex(sofa, texTelaSofa, P, V, M * TSofa * SSofa * RSofa);

    glm::mat4 TSofa1 = glm::translate(I, glm::vec3(-22.0, -0.20, 2));
    drawObjectTex(sofa, texTelaSofa, P, V, M * TSofa1 * SSofa * RSofa);

    glm::mat4 SSilla = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 TSilla = glm::translate(I, glm::vec3(-20.0, 1, 10));
    glm::mat4 RSilla= glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
    drawObjectTex(chair, texTelaSofa, P, V, M * TSilla * SSilla * RSilla);

    glm::mat4 SSilla2 = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 TSilla2 = glm::translate(I, glm::vec3(-20.0, 1, 7));
    glm::mat4 RSilla2= glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
    drawObjectTex(chair, texTelaSofa, P, V, M * TSilla2 * SSilla2 * RSilla2);


    glm::mat4 SMesa = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 TMesa = glm::translate(I, glm::vec3(-20.0, 1, 8.5));
    drawObjectTex(table, texTelaSofa, P, V, M * TMesa * SMesa);


    glm::mat4 SFrame1 = glm::scale(I, glm::vec3(0.1, 0.181, 0.095));
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

    glm::mat4 SFrame2 = glm::scale(I, glm::vec3(0.114, 0.181, 0.095));
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
    drawObjectTex(cubeTerminal, texGlass, P, V, M * TTerminal2 * STerminal2); //Vidrio de la terminal
    glDepthMask(GL_TRUE);

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M;
        if(i == 3){
            M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.025)); //Luz situada en el morro del avión
        } else{
            M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1)); //Luces de las farolas, algo más grandes
        }
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025)); //Focos de la terminal
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
        //Controles del avion
        case GLFW_KEY_I: //Hacia delante
            wheelRotAngle += 45.0f;

            if(planeRotAngle == 90.0f){
                desZ += 1.0f;
            }
            else if(planeRotAngle == 180.0f){
                desX += 1.0f;
            }
            else if(planeRotAngle == 270.0f){
                desZ -= 1.0f;
            }
            else if(planeRotAngle == 0.0f){
                desX -= 1.0f;
            }
        break;

        case GLFW_KEY_K: //Hacia atras
            wheelRotAngle += 45.0f;
            if(planeRotAngle == 90.0f){
                desZ -= 1.0f;
            }
            else if(planeRotAngle == 180.0f){
                desX -= 1.0f;
            }
            else if(planeRotAngle == 270.0f){
                desZ += 1.0f;
            }
            else if(planeRotAngle == 0.0f){
                desX += 1.0f;
            }
        break;

        case GLFW_KEY_J: //Rotación de 90º a la izquierda (desde el ángulo actual)
            wheelRotAngle -= 45.0f;
            if(action == GLFW_PRESS) {
                planeRotAngle += 90.0f;
            }
            if(planeRotAngle == 360.0f){
                planeRotAngle = 0.0f;
            }
            break;

        case GLFW_KEY_L: //Rotación de 90º a la derecha (desde el ángulo actual)
            wheelRotAngle -= 45.0f;
            if(action == GLFW_PRESS) {
                planeRotAngle -= 90.0f;
            }
            if(planeRotAngle == -90.0f){
                planeRotAngle = 270.0f;
            }
            break;

        //Auxiliar: Movimiento libre por el mundo con centro en (0,0,0)
        case GLFW_KEY_W:  y += 1.0f;   break; //Aumentar y de la cámara
        case GLFW_KEY_S:  y -= 1.0f;   break; //Disminuir y de la cámara
        case GLFW_KEY_D:  x += 1.0f;   break; //Aumentar x de la cámara
        case GLFW_KEY_A:  x -= 1.0f;   break; //Disminuir x de la cámara
        case GLFW_KEY_E:  z += 1.0f;   break; //Aumentar z de la cámara
        case GLFW_KEY_Q:  z -= 1.0f;   break; //Disminuir z de la cámara

        //Modo noche
        case GLFW_KEY_N:
            if(action == GLFW_PRESS){
                nightMode = !nightMode;
            } else{
                nightMode = nightMode;
            }
            break;

        //Subir / Bajar el tren de aterrizaje
        case GLFW_KEY_G:
            if(action == GLFW_PRESS){
                gearUp = !gearUp;
            } else{
                gearUp = gearUp;
            }
            break;

        //Subir / Bajar los flaps
        case GLFW_KEY_F:
            if(action == GLFW_PRESS){
                flapsUp = !flapsUp;
            } else{
                flapsUp = flapsUp;
            }
            break;

        //Alternar entre movimiento orbital con el ratón o movimiento libre con las teclas
        case GLFW_KEY_M:
            if(action == GLFW_PRESS){
                mouseMovement = !mouseMovement;
            } else{
                mouseMovement = mouseMovement;
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

void staticLightsLoad(){
    if(nightMode){
        // Luz ambiental global
        lightG.ambient = glm::vec3(0.3, 0.3, 0.3);

        // Luces direccionales
        lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
        lightD[0].ambient   = glm::vec3( 0.1, 0.1, 0.1);
        lightD[0].diffuse   = glm::vec3( 0.1, 0.1, 0.1);
        lightD[0].specular  = glm::vec3( 0.1, 0.1, 0.1);
    }
    else{
        // Luz ambiental global
        lightG.ambient = glm::vec3(0.9, 0.9, 0.9);

        // Luces direccionales
        lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
        lightD[0].ambient   = glm::vec3( 0.1, 0.1, 0.1);
        lightD[0].diffuse   = glm::vec3( 0.7, 0.7, 0.7);
        lightD[0].specular  = glm::vec3( 0.7, 0.7, 0.7);
    }
}

void movingLightsLoad(){
    if(planeRotAngle == 90.0f){
        lightP[3].position    = glm::vec3(-6.0+desX, 0.24, 5.25+desZ); //Luz del morro del avión
    }
    else if (planeRotAngle == 180.0f){
        lightP[3].position    = glm::vec3(-4.7+desX, 0.24, 4.0+desZ);
    }
    else if (planeRotAngle == 270.0f){
        lightP[3].position    = glm::vec3(-6.0+desX, 0.24, 2.75+desZ);
    }
    else if (planeRotAngle == 0.0f){
        lightP[3].position    = glm::vec3(-7.2+desX, 0.24, 4.0+desZ);
    }
    lightP[3].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[3].diffuse     = glm::vec3(0.7, 0.7, 0.7);
    lightP[3].specular    = glm::vec3(0.7, 0.7, 0.7);
    lightP[3].c0          = 1.00;
    lightP[3].c1          = 0.22;
    lightP[3].c2          = 0.20;
}



