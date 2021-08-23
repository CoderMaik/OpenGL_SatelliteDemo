#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0)

// Functions declarations
void funInit();
void funReshape(int w, int h);
void funDisplay();

void camMove(int x, int y);
void camZoom(int key, int status, int x, int y);

void sceneActions(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

void rotSolar(int value);
void rotPod(int value);

void setLights(glm::mat4 P, glm::mat4 V);

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures texture, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawFloor(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawThermalMod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAntennaMod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAntennaJoin(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAntenna(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCommsMod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSolarPanelsMod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSolarPanels(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSolarPanel(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHomeMod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHomePod(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSatellite(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAsteroids(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void updateLightD();

// Shaders
   Shaders shaders;

// Models
    Model plane;
    Model cone;
    Model sphere;
    Model cylinder;
    Model cube;
    Model semisphere;
    Model roundedcube;
    Model torus;
    Model cutcylinder;
    Model cutcylinder2;
    Model asteroid3D;

// Lights and materials
    #define   NLD 1
    #define   NLP 1
    #define   NLF 1

   Material polishedbronze;
   Material white;

   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];

// Textures
    Texture imgNoEmissive;      //img1
    Texture imgLightsDiffuse;   //img4
    Texture imgFullMetallic;

    Texture imgPanelDiffuse;
    Texture imgPanelMetallic;
    Texture imgPanelNormal;

    Texture imgSolarDiffuse;
    Texture imgSolarNormal;
    Texture imgSolarMetallic;

    Texture imgBlanketDiffuse;
    Texture imgBlanketNormal;

    Texture imgMetalDiffuse;
    Texture imgMetalSpecular;
    Texture imgMetalNormal;

    Texture imgAntennaDiffuse;
    Texture imgAntennaNormal;

    Texture imgThermalDiffuse;
    Texture imgThermalNormal;

    Texture imgAsteroidDiffuse;
    Texture imgAsteroidNormal;
    Texture imgAsteroidSpecular;

    Textures texLights;
    Textures panel;
    Textures solar;
    Textures blanket;
    Textures plates;
    Textures thermal;
    Textures antenna;
    Textures asteroid;

// Viewport
   int w = 600;
   int h = 600;

// Data for animations
    GLint speed = 5;
    float zoom = 65.0;
    float alphaX = -50.0;
    float alphaY = 25.0;

    float rotPanels = 0.0;

    float rotAntennaUpDown = 0.0;
    float rotAntennaLeftRight = 0.0;

    bool rotPodBool = true;
    float rotHome = 0.0;

    float movX = 0.0;
    float movY = 0.0;
    float movZ = 0.0;

    float lightDIntensity = 0.7;
    float rotLightD = 15;

    bool focalBool = true;
    bool texturesBool = true;

int main(int argc, char** argv) {

 // Init GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Spaceship IG");

 // Init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Init scene
    funInit();

 // CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutTimerFunc(speed, rotSolar, 0);
    glutTimerFunc(speed, rotPod, 0);
    glutMouseFunc(camZoom);
    glutKeyboardFunc(sceneActions);
    glutMotionFunc(camMove);
    glutSpecialFunc(specialKeys);

 // Main Loop
    glutMainLoop();

    return 0;
}

void funInit() {

 // Operate colors with alpha & enable transparencies
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Depth test
    glEnable(GL_DEPTH_TEST);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Models
    // Models made from primitives in blender
    semisphere.initModel("resources/models/antennacap.obj");
    roundedcube.initModel("resources/models/notipcube.obj");
    cutcylinder.initModel("resources/models/cutcylinder.obj");
    cutcylinder2.initModel("resources/models/cutcylinder2.obj");
    // Downloaded model
    asteroid3D.initModel("resources/models/asteroid.obj");
    // Primitive models from blender
    torus.initModel("resources/models/torus.obj");
    cube.initModel("resources/models/cube.obj");
    // Imported from previous exercises
    plane.initModel("resources/models/plane.obj");
    cone.initModel("resources/models/cone.obj");
    sphere.initModel("resources/models/sphere.obj");
    cylinder.initModel("resources/models/cylinder.obj");

 // Global ambient light
    lightG.ambient = glm::vec3(0.25,0.25,0.25);

 // Directional Lights
    lightD[0].direction   = glm::vec3(-1.0, 0.0, -0.9);
    lightD[0].ambient     = glm::vec3( 0.5, 0.5, 0.5);
    lightD[0].diffuse     = glm::vec3( 0.5, 0.5, 0.5);
    lightD[0].specular    = glm::vec3( 0.5, 0.5, 0.5);

 // Position Lights
    lightP[0].position    = glm::vec3(0.0, 0.0, 1.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Focal Lights
    lightF[0].position    = glm::vec3(-4.0,  1.0,  1.0);
    lightF[0].direction   = glm::vec3( 4.0, -1.0, -0.5);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 7.5;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 2;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

 // Materials
    //bronze for asteroids
    polishedbronze.ambient =  glm::vec4(0.2125f, 0.1275f, 0.054f, 1.0f );
    polishedbronze.diffuse =  glm::vec4(0.714f, 0.4284f, 0.18144f, 1.0f);
    polishedbronze.specular = glm::vec4(0.393548f, 0.271906f, 0.166721f, 1.0f );
    polishedbronze.emissive = glm::vec4(0.000000,0.000000,0.000000,1.00);
    polishedbronze.shininess =25.6f ;

    //White rubber (glass)
    white.ambient =glm::vec4(0.05f,0.05f,0.05f,0.5f);
    white.diffuse =glm::vec4( 0.5f,0.5f,0.5f,0.5f);
    white.specular =glm::vec4(0.7f,0.7f,0.7f,0.5f);
    white.emissive = glm::vec4(0.000000,0.000000,0.000000,1.00);
    white.shininess = 76.0f;


    // Textures
    imgNoEmissive.initTexture("resources/textures/noEmissive.png");
    imgFullMetallic.initTexture("resources/textures/fullMetallic.tif");

    imgLightsDiffuse.initTexture("resources/textures/img4.png");

    imgPanelDiffuse.initTexture("resources/textures/panelDiffuse.tif");
    imgPanelMetallic.initTexture("resources/textures/panelMetallic.tif");
    imgPanelNormal.initTexture("resources/textures/panelNormal.tif");

    imgSolarDiffuse.initTexture("resources/textures/solarDiffuse.jpg");
    imgSolarNormal.initTexture("resources/textures/solarNormal.jpg");
    imgSolarMetallic.initTexture("resources/textures/solarMetallic.jpg");

    imgBlanketDiffuse.initTexture("resources/textures/blanketDiffuse.tif");
    imgBlanketNormal.initTexture("resources/textures/blanketNormal.tif");

    imgThermalDiffuse.initTexture("resources/textures/thermalDiffuse.jpg");
    imgThermalNormal.initTexture("resources/textures/thermalNormal.jpg");

    imgMetalDiffuse.initTexture("resources/textures/metalDiffuse.jpg");
    imgMetalSpecular.initTexture("resources/textures/metalSpecular.jpg");
    imgMetalNormal.initTexture("resources/textures/metalNormal.jpg");

    imgAntennaDiffuse.initTexture("resources/textures/antennaDiffuse.jpg");
    imgAntennaNormal.initTexture("resources/textures/antennaNormal.jpg");

    imgAsteroidDiffuse.initTexture("resources/textures/asteroidDiffuse.jpg");
    imgAsteroidNormal.initTexture("resources/textures/asteroidNormal.jpg");
    imgAsteroidSpecular.initTexture("resources/textures/asteroidSpecular.jpg");

    texLights.diffuse = imgLightsDiffuse.getTexture();
    texLights.specular = imgLightsDiffuse.getTexture();
    texLights.emissive = imgNoEmissive.getTexture();
    texLights.normal = 0;
    texLights.shininess = 10.0;

    solar.diffuse = imgSolarDiffuse.getTexture();
    solar.specular = imgSolarMetallic.getTexture();
    solar.emissive = imgNoEmissive.getTexture();
    solar.normal = imgSolarNormal.getTexture();
    solar.shininess = 10.0;

    panel.diffuse = imgPanelDiffuse.getTexture();
    panel.specular = imgPanelMetallic.getTexture();
    panel.emissive = imgNoEmissive.getTexture();
    panel.normal = imgPanelNormal.getTexture();
    panel.shininess = 6.0;

    blanket.diffuse = imgBlanketDiffuse.getTexture();
    blanket.specular = imgFullMetallic.getTexture();
    blanket.normal = imgBlanketNormal.getTexture();
    blanket.emissive = imgNoEmissive.getTexture();
    blanket.shininess = 10;

    antenna.diffuse = imgAntennaDiffuse.getTexture();
    antenna.specular = imgFullMetallic.getTexture();
    antenna.normal = imgAntennaNormal.getTexture();
    antenna.emissive = imgNoEmissive.getTexture();
    antenna.shininess = 10;

    thermal.diffuse = imgThermalDiffuse.getTexture();
    thermal.specular = imgFullMetallic.getTexture();
    thermal.normal = imgThermalNormal.getTexture();
    thermal.emissive = imgNoEmissive.getTexture();
    thermal.shininess = 6;

    plates.diffuse = imgMetalDiffuse.getTexture();
    plates.normal = imgMetalNormal.getTexture();
    plates.specular = imgMetalSpecular.getTexture();
    plates.emissive = imgNoEmissive.getTexture();
    plates.shininess = 10;

    asteroid.diffuse = imgAsteroidDiffuse.getTexture();
    asteroid.specular = imgAsteroidSpecular.getTexture();
    asteroid.normal = imgAsteroidNormal.getTexture();
    asteroid.emissive = imgNoEmissive.getTexture();
    asteroid.shininess = 5;
}

void funReshape(int wnew, int hnew){
    glViewport(0,0,wnew,hnew);
    w = wnew;
    h = hnew;
}

void funDisplay() {

 // Deleting color buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Init shaders
    shaders.useShaders();

 // Matrix P
    float fovy   = zoom;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matrix V
    float x = 5.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 5.0f*glm::sin(glm::radians(alphaY));
    float z = 5.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos   (x,y,z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);

 // Lights
    setLights(P,V);

 // Disabling Culling for visible objects from both sides
    glDisable(GL_CULL_FACE);

// Opaque objets drawn with polygon offset to fix Z-Fighting
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f,1.0f);

 // Draw floor just for debugging
    //drawFloor(P,V,I);
 // Draw Satellite
    glm::mat4 T = glm::translate(I,glm::vec3(movX,movY,movZ));
    drawSatellite(P,V,I*T);
 // Draw space rocks
    drawAsteroids(P,V,I);

    glDisable(GL_POLYGON_OFFSET_FILL);

 // Swapping buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        lightP[i].position = glm::vec3(3*glm::cos(glm::radians(rotLightD)), 0.4, 3*glm::sin(glm::radians(rotLightD)));
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1/4));
        drawObjectTex(sphere,texLights, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.1/4));
        drawObjectTex(sphere,texLights, P, V, M);
    }

}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    shaders.setBool("uWithMaterials",true);
    shaders.setMat4("uN", glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM",M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);
}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    shaders.setBool("uWithMaterials",false);
    shaders.setMat4("uN", glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM",M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0)
        shaders.setBool("uWithNormals",true);
    else
        shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);
}

void drawFloor(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(4.0, 1.0, 4.0));
    drawObjectMat(plane,polishedbronze, P, V, M * S);
}
//Unused, tried to build a skybox but couldnt do it
/*void drawSpace(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(1.0, 35.0, 35.0));
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));
    glm::mat4 R90neg = glm::rotate(I,glm::radians(90.0f), glm::vec3(0,0,1));
    glm::mat4 R90x = glm::rotate(I,glm::radians(-90.0f), glm::vec3(1,0,0));

    glm::mat4 T1 = glm::translate(I,glm::vec3(0.0,15.0,0.0));

    drawObjectMat(plane, cyan, P, V, M * S * R90 * T1);
    drawObjectMat(plane, cyan, P, V, M * S * R90neg * T1);
}*/

void drawSatellite(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 SGeneral = glm::scale(I, glm::vec3(0.6, 0.6, 0.6));

    glm::mat4 T1 = glm::translate(I,glm::vec3(2.0,0.0,0.0));
    glm::mat4 T2 = glm::translate(I,glm::vec3(-2.0,0.0,0.0));
    glm::mat4 T3 = glm::translate(I,glm::vec3(-1.1,1.0,0.0));

    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));

    glm::mat4 S = glm::scale(I,glm::vec3(0.4,0.4,0.4));

    //Rotating communication module with solar panels
    drawCommsMod(P,V,M*SGeneral*T1);
    //Two antenna modules on tip and top
    drawAntennaMod(P,V,M*SGeneral*T2);
    drawAntennaMod(P,V,M*SGeneral*R90*T3*S);
    //Thermal module attached to the back
    drawThermalMod(P,V,M*SGeneral);
    //Transparent module
    drawHomeMod(P,V,M*SGeneral);
}

void drawHomeMod(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R = glm::rotate(I,glm::radians(rotHome), glm::vec3(1,0,0));
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));

    glm::mat4 S = glm::scale(I,glm::vec3(0.4,0.2,0.4));
    glm::mat4 S1 = glm::scale(I,glm::vec3(0.2,1.1,1.1));

    glm::mat4 T = glm::translate(I,glm::vec3(0.0,7.5,0.0));

    glm::mat4 T1 = glm::translate(I,glm::vec3(0.0,5.5,0.0));
    glm::mat4 T2 = glm::translate(I,glm::vec3(0.0,-5.5,0.0));

    glm::mat4 R90Inv = glm::rotate(I,glm::radians(90.0f), glm::vec3(0,0,1));

    drawHomePod(P,V,M*R);
    drawObjectTex(cone,blanket,P,V,M*R90Inv*S*T);
    drawObjectTex(cylinder,plates,P,V,M*S1*R90*T1);
    drawObjectTex(cylinder,plates,P,V,M*S1*R90*T2);
}

void drawHomePod(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));
    glm::mat4 R90y = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,1,0));
    glm::mat4 R45x = glm::rotate(I,glm::radians(-45.0f), glm::vec3(1,0,0));

    glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.0,1.5));

    glm::mat4 S = glm::scale(I,glm::vec3(0.02,0.02,0.5));

    drawObjectTex(cube,antenna,P,V,M*R45x*T*S);
    drawObjectTex(cube,antenna,P,V,M*R45x*R45x*R45x*R45x*T*S);

    drawObjectTex(cutcylinder,panel,P,V,M*R90);
    //Disabling depth mask to draw transparent windows
    glDepthMask(GL_FALSE);
    drawObjectMat(cutcylinder2,white,P,V,M*R90*R90y);
    glDepthMask(GL_TRUE);
}

void drawAntennaMod(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S1 = glm::scale(I,glm::vec3(0.15,0.15,0.15));

    glm::mat4 T1 = glm::translate(I,glm::vec3(-1.1,0.0,0.0));

    glm::mat4 RAntennaLeftRight = glm::rotate(I,glm::radians(rotAntennaLeftRight), glm::vec3(0,1,0));
    glm::mat4 RAntennaUpDown = glm::rotate(I,glm::radians(rotAntennaUpDown), glm::vec3(0,0,1));

    drawObjectTex(roundedcube,antenna,P,V,M*S1);
    //Draw articulation
    drawAntennaJoin(P,V,M*RAntennaUpDown*RAntennaLeftRight*T1);
}

void drawAntennaJoin(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));
    glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.9,0.0));
    glm::mat4 S = glm::scale(I,glm::vec3(0.05,0.2,0.05));

    glm::mat4 S1 = glm::scale(I,glm::vec3(0.05,0.05,0.05));
    glm::mat4 T1 = glm::translate(I,glm::vec3(0.7,0.0,0.0));

    drawObjectTex(cylinder,panel,P,V,M*R90*T*S);
    drawObjectTex(sphere,blanket,P,V,M*T1*S1);
    drawAntenna(P,V,M);
}

void drawAntenna(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));
    glm::mat4 S = glm::scale(I,glm::vec3(0.05,0.6,0.05));

    glm::mat4 S2 = glm::scale(I,glm::vec3(0.04,0.04,0.04));
    glm::mat4 T2 = glm::translate(I,glm::vec3(-0.65,0.0,0.0));
    drawObjectTex(sphere,antenna,P,V,M*T2*S2);
    drawObjectTex(cylinder,antenna,P,V,M*R90*S);
    drawObjectTex(semisphere,antenna,P,V,M*R90);
}

void drawCommsMod(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));

    drawObjectTex(cylinder,panel,P,V,M*R90);
    glm::mat4 R = glm::rotate(I,glm::radians(rotPanels), glm::vec3(1,0,0));
    drawSolarPanelsMod(P,V,M*R);
}

void drawSolarPanelsMod(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R90 = glm::rotate(I,glm::radians(-90.0f), glm::vec3(0,0,1));
    drawObjectTex(torus,blanket,P,V,M*R90);

    drawSolarPanels(P,V,M*R90);
}

void drawSolarPanels(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R = glm::rotate(I,glm::radians(-180.0f), glm::vec3(1,0,0));
    glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.0,1.0));
    drawSolarPanel(P,V,M*T);
    drawSolarPanel(P,V,M*R*T);
}

void drawSolarPanel(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I,glm::vec3(0.04,0.04,1));
    glm::mat4 T = glm::translate(I,glm::vec3(0.0,0.0,1.0));
    drawObjectTex(roundedcube,panel,P,V,M*S*T);

    glm::mat4 S1 = glm::scale(I,glm::vec3(1,0.01,0.2));
    glm::mat4 S2 = glm::scale(I,glm::vec3(-1,0.01,0.2));

    glm::mat4 T2 = glm::translate(I,glm::vec3(1.0,0.0,3.5));
    glm::mat4 T3 = glm::translate(I,glm::vec3(1.0,0.0,6));
    glm::mat4 T4 = glm::translate(I,glm::vec3(1.0,0.0,8.5));

    drawObjectTex(cube,solar,P,V,M*S1*T2);
    drawObjectTex(cube,solar,P,V,M*S1*T3);
    drawObjectTex(cube,solar,P,V,M*S1*T4);

    drawObjectTex(cube,solar,P,V,M*S2*T2);
    drawObjectTex(cube,solar,P,V,M*S2*T3);
    drawObjectTex(cube,solar,P,V,M*S2*T4);
}

void drawThermalMod(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I,glm::vec3(0.1,0.65,0.65));
    glm::mat4 T = glm::translate(I,glm::vec3(30.0,0.0,0.0));

    drawObjectTex(sphere,thermal,P,V,M*S*T);
}

void drawAsteroids(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 R = glm::rotate(I,glm::radians(-rotPanels), glm::vec3(1,0,1));
    glm::mat4 R1 = glm::rotate(I,glm::radians(rotPanels), glm::vec3(1,1,0));
    glm::mat4 R2 = glm::rotate(I,glm::radians(rotPanels), glm::vec3(1,1,0));
    glm::mat4 T = glm::translate(I,glm::vec3(0.0,55.0,0.0));
    glm::mat4 S = glm::scale(I,glm::vec3(0.08,0.08,0.08));
    if (!texturesBool) {
        drawObjectMat(asteroid3D, polishedbronze, P, V, M * S * R2 * T * R1);
        drawObjectMat(asteroid3D, polishedbronze, P, V, M * S * R * T * R1);
    } else {
        drawObjectTex(asteroid3D, asteroid, P, V, M * S * R2 * T * R1);
        drawObjectTex(asteroid3D, asteroid, P, V, M * S * R * T * R1);
    }
}

// -----------------------------------Animaciones-----------------------------------

void rotSolar(int value){ //Solar panels rotation
    rotPanels -= 0.5;
    glutPostRedisplay();
    glutTimerFunc(speed, rotSolar, 0);
}

void rotPod(int value){ //HomeMod rotation
    if(rotPodBool) {
        rotHome += 0.5;
        glutPostRedisplay();
        glutTimerFunc(speed, rotPod, 0);
    }
}

void camZoom(int key, int status, int x, int y){
    if (key == 4) {
        if (zoom < 90)
            zoom += 5;
    }else if (key == 3)
        if (zoom > 10)
            zoom -= 5;
    glutPostRedisplay();
}

void camMove(int x, int y) {
    int xi = (x - w/2)*180 / w;
    int yi = (y - h/2)*90 / (h/2);

    if(xi<180 && xi>-180)
        alphaX = xi;
    if(yi<90 && yi>-90)
        alphaY = -yi;
    glutPostRedisplay();
}

void sceneActions(unsigned char key, int x, int y){
    switch(key) {
        case 'L': // Directional light intensity up
            if (lightDIntensity < 1.0) {
                lightDIntensity += 0.1;
                updateLightD();
            }
            break;
        case 'l': // Directional light intensity down
            if (lightDIntensity > 0.0) {
                lightDIntensity -= 0.1;
                updateLightD();
            }
            break;
        case 'P': // Rotation of pLight
            rotLightD += 2.5;
            break;
        case 'p':
            rotLightD -= 2.5;
            break;
        case 't':  // Toggle textures on/off asteroids
            if (texturesBool)
                texturesBool = false;
            else
                texturesBool = true;
            break;
        //Model moves
        case 'd': // -Y
            if(movY>-1.2)
                movY -= 0.05;
            break;
        case 'u': // +Y
            if(movY<1.2)
                movY += 0.05;
            break;
        case 'Z': // rotAntennaUpDown Up
            if(rotAntennaUpDown<45 && rotAntennaUpDown>=-45)
                rotAntennaUpDown+=3;
            break;
        case 'z': // rotAntennaUpDown Down
            if(rotAntennaUpDown<=45 && rotAntennaUpDown>-45)
                rotAntennaUpDown-=3;
            break;
        case 'X': // rotAntennaLeftRight Left
            if(rotAntennaLeftRight<45 && rotAntennaLeftRight>=-45)
                rotAntennaLeftRight+=3;
            break;
        case 'x': // rotAntennaLeftRight Right
            if(rotAntennaLeftRight<=45 && rotAntennaLeftRight>-45)
                rotAntennaLeftRight-=3;
            break;
        case 's': // Toggle rotation of HomeMod
            if(rotPodBool)
                rotPodBool = false;
            else{
                rotPodBool = true;
                glutTimerFunc(speed, rotPod, 0);
            }break;
        default:
            break;
    }glutPostRedisplay();
}

void updateLightD(){
    lightD[0].ambient = glm::vec3(lightDIntensity,lightDIntensity,lightDIntensity);
    lightD[0].diffuse = glm::vec3(lightDIntensity,lightDIntensity,lightDIntensity);
    lightD[0].specular = glm::vec3(lightDIntensity,lightDIntensity,lightDIntensity);
}
void specialKeys(int key, int x, int y){
    switch(key){
        //Movements on XZ axis
        case GLUT_KEY_UP:
            if(movZ > -1.2)
                movZ -= 0.05;
            break;
        case GLUT_KEY_DOWN:
            if(movZ < 1.2)
                movZ += 0.05;
            break;
        case GLUT_KEY_LEFT:
            if(movX>-1.2)
                movX -= 0.05;
            break;
        case GLUT_KEY_RIGHT:
            if(movX < 1.2)
                movX += 0.05;
            break;
        // On/Off Lights
        case GLUT_KEY_F1:
            if (focalBool) {
                lightF[0].ambient = glm::vec3(0.0, 0.0, 0.0);
                lightF[0].diffuse = glm::vec3(0.0, 0.0, 0.0);
                lightF[0].specular = glm::vec3(0.0, 0.0, 0.0);
                focalBool = false;
            } else {
                lightF[0].ambient = glm::vec3(0.5, 0.5, 0.5);
                lightF[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
                lightF[0].specular = glm::vec3(0.9, 0.9, 0.9);
                focalBool = true;
            }
            break;
        case GLUT_KEY_F2:
            if (lightDIntensity == 0)
                lightDIntensity = 0.5;
            else
                lightDIntensity = 0;
            updateLightD();
            break;
        default:
            break;
    }glutPostRedisplay();
}