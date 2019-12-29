#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "../Camera/FreeCamera.h"
#include "../Camera/HermiteCamera.h"
#include "../Camera/CarCamera.h"
#include "../Scene/Entity.h"
#include "../Loaders/TrackLoader.h"
#include "../Physics/PhysicsEngine.h"
#include "../Loaders/CarLoader.h"
#include "../RaceNet/CarAgent.h"
#include "../Util/Logger.h"
#include "../Config.h"

#include "HermiteCurve.h"
#include "CarRenderer.h"
#include "TrackRenderer.h"
#include "SkyRenderer.h"
#include "ShadowMapRenderer.h"
#include "DebugRenderer.h"
/*#include "MenuRenderer.h"*/

class Renderer {
public:
    Renderer(GLFWwindow *glWindow, std::shared_ptr<Logger> &onfsLogger, const std::vector<NfsAssetList> &installedNFS, std::shared_ptr<ONFSTrack> currentTrack, std::shared_ptr<BulletDebugDrawer> debugDrawer);
    ~Renderer();
    static void GlfwError(int id, const char *description) {
        LOG(WARNING) << description;
    }
    static void WindowSizeCallback(GLFWwindow *window, int width, int height) {
        Config::get().resX = width;
        Config::get().resY = height;
    }
    static GLFWwindow *InitOpenGL(int resolutionX, int resolutionY, const std::string &windowName);
    bool Render(float totalTime,
                const std::shared_ptr<Camera> &activeCamera,
                const std::shared_ptr<HermiteCamera> &hermiteCamera,
                ParamData &userParams,
                AssetData &loadedAssets,
                std::shared_ptr<Car> &playerCar,
                const std::vector<CarAgent> &racers);

private:
    void _InitialiseIMGUI();
    static void _DrawMetadata(Entity *targetEntity);
    bool _DrawMenuBar(AssetData &loadedAssets);
    void _DrawUI(ParamData &userParams, const std::shared_ptr<Camera> &camera, const std::shared_ptr<Car> &playerCar);
    static std::vector<uint32_t> _GetLocalTrackBlockIDs(const shared_ptr<ONFSTrack> &track, const std::shared_ptr<Camera> &camera, ParamData &userParams);
    static std::vector<std::shared_ptr<Entity>> _FrustumCull(const std::shared_ptr<ONFSTrack> &track, const std::shared_ptr<Camera> &camera, ParamData &userParams);

    GLFWwindow *m_pWindow;
    std::shared_ptr<Logger> m_logger;
    std::vector<NfsAssetList> m_nfsAssetList;
    std::shared_ptr<ONFSTrack> m_track;

    TrackRenderer m_trackRenderer;
    CarRenderer m_carRenderer;
    SkyRenderer m_skyRenderer;
    ShadowMapRenderer m_shadowMapRenderer;
    DebugRenderer m_debugRenderer;
    /*MenuRenderer menuRenderer;*/

    /* State */
    bool m_entityTargeted = false;
    Entity *m_pTargetedEntity = nullptr;

    // TODO: Move sun to an orbital manager class so the sunsets can look lit af
    GlobalLight m_sun;
};
