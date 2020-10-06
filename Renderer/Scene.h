#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"

class Scene {
public:
    Scene();

    void RenderScene();
    bool LoadFromConfig(const std::string fileName);

    ~Scene();
private:

    Camera camera;
    Skybox skybox;

    int pointLightcount;
    int spotLightCount;
    int directionalLightCount;
    
    DirectionalLight mainLight;
    std::vector<Model*> models;
    std::vector<SpotLight*> spotLights;
    std::vector<PointLight*> pointLights;
    friend class SceneLoader;
};

/**
 * Class that loads scenes from YAML configs.
 * 
 * While YAML parsing is probably not the most performant
 * method of serializing and deserializing scenes, it ought
 * to make it easy to manually create and edit scenes for
 * the time being.
 * 
 * In YAML, a scene is described in sequences of models
 * and lights. All objects will be described by their positions
 * and orientations.
 * 
 * Models are generally described by the path
 * to their models file(s) and size scaling parameters.
 * 
 * Lights are defined by their lighting properties.
 */
class SceneLoader {
public:
    SceneLoader();

    bool LoadScene(Scene* scene, const std::string fileName);
    bool LoadModels(Scene* scene, YAML::Node modelConfig);
    bool LoadLights(Scene* scene, YAML::Node lightConfig);

    ~SceneLoader();
private:
    
};