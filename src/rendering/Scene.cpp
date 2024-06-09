#include "Scene.h"

// SCENE

Scene::Scene() {
}

bool Scene::LoadFromConfig(const std::string fileName) {
    return NULL;
}

void Scene::RenderScene() {

}

Scene::~Scene() {
}

// SCENE LOADER

SceneLoader::SceneLoader() {

}

bool SceneLoader::LoadScene(Scene* scene, const std::string fileName) {
    YAML::Node config;
    try {
        config = YAML::LoadFile(fileName);
    }
    catch (YAML::ParserException& e) {
        printf("Scene file malformed: %s\n", fileName.c_str());
        return false;
    }
    catch (YAML::BadFile& e) {
        printf("Could not load file: %s\n", fileName.c_str());
        return false;
    }

    if (config["models"]) {
        LoadModels(scene, config);
    }

    return true;
}

/**
 * Method that loads model configs from the yaml file.
 * Methods are defined by:
 *   - fileName: the model file name
 *   - position: 3-value float vector describing the coordinates to place the
       model at.
 *   - rotations: 3-value float vector describing the rotations along each axis
       in degrees. optional.
 *   - name: the name to call the model by. optional.
 */
bool SceneLoader::LoadModels(Scene* scene, YAML::Node modelConfig) {
    // TODO
    return false;
}

bool SceneLoader::LoadLights(Scene* scene, YAML::Node lightConfig) {
    // TODO
    return false;
}

SceneLoader::~SceneLoader() {
}