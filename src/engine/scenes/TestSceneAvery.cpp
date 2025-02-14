#include "TestSceneAvery.hpp"

#include "core/Application.hpp"

#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"
#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/FollowCameraController.hpp"
#include "components/TriggerCollider.hpp"
#include "components/TimerComponent.hpp"

#include "logging/GLDebug.hpp"
#include "physics/PhysicsWorld.hpp"

#include "audio/AudioPlayer.hpp"

#include "logging/Log.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeTestSceneAvery()
	{
		return std::make_unique<TestSceneAvery>();
	}

	TestSceneAvery::TestSceneAvery()
	{
	}

	TestSceneAvery::~TestSceneAvery()
	{
	}

	void TestSceneAvery::OnLoad()
	{
		// load textures
		stbi_set_flip_vertically_on_load(true);

		// Load Models
		const std::string backpackPath = "data/models/backpack/backpack.obj";
		if (backpackModel)
		{
			backpackModel.reset();
		}
		backpackModel = std::make_unique<Model>(backpackPath);
		// const std::string physicsPlaygroundPath = "data/models/leveldesign/KevWorldClouds.obj";
		const std::string physicsPlaygroundPath = "data/models/skyramps/skyramps.obj";

		if (playgroundModel)
		{
			playgroundModel.reset();
		}
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

		// BACKPACK (Character) ---------------------
		Entity& backpackObj = AddEntity("Backpack");

		// ModelInstance* modelRender = backpackObj.AddComponent<ModelInstance>();
		// modelRender->Init(backpackModel.get());
		backpackObj.SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj.SetPosition(glm::vec3(-112.5f, 505.f, 3.f));
		backpackObj.SetYawRotation(0);

		CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		characterController->Init(1.f, 3.f);

		TriggerCollider* triggerCollider = backpackObj.AddComponent<TriggerCollider>();
		std::vector<const btCollisionObject*> ignoredObjects;
		ignoredObjects.push_back(characterController->getRigidBody());
		triggerCollider->Init(characterController->getGroundCheckObj(), ignoredObjects);

		FollowCameraController* followCam = backpackObj.AddComponent<FollowCameraController>();
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 1.0f, 0.0f));

		// Timer Component EXAMPLE ----------
		TimerComponent* timerComponent = backpackObj.AddComponent<TimerComponent>();
		timerComponent->SetTargetTime(5.0f);
		timerComponent->OnTimerEnd([&]() {
			LEI_TRACE("Timer Ended!!!");
		});
		timerComponent->StartTimer();
		// -----------------------------------

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		physicsPlaygroundObj.SetYawRotation(0);

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		// Test color source
		Entity& startColorSrcObj = AddEntity("Start Color Area");
		ColorSource* startSrc = startColorSrcObj.AddComponent<ColorSource>();
		startSrc->Init(200, 10, true);
		startColorSrcObj.SetPosition(glm::vec3(-112.5, 505, 3));

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);
	}

	void TestSceneAvery::OnReset()
	{
		// Just need to reset the backpack.
		Entity* backpackObj = GetEntity("Backpack");
		backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
		// backpackObj->SetPosition(glm::vec3(-112.5f, 505.f, 3.f));
		// AudioPlayer::PlaySFXForSeconds("landing", 6, 1.0f, 0, 2);
	}

	void TestSceneAvery::OnUpdate()
	{
		// if (glfwGetKey(Application::Window(), GLFW_KEY_R) == GLFW_PRESS)
		//{
		//	Entity* backpackEntity = GetEntity("Backpack");
		//	if (backpackEntity)
		//	{
		//		std::cout << "backpackEntity" << std::endl;
		//		backpackEntity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//	}
		// }

		// if (glfwGetKey(Application::Window(), GLFW_KEY_J) == GLFW_PRESS)
		// {
		// 	LEI_TRACE("Pressing J Key");
		// }
		// if (glfwGetKey(Application::Window(), GLFW_KEY_K) == GLFW_PRESS)
		// {
		// 	LEI_TRACE("Pressing K Key");
		// 	AudioPlayer::PlaySFXForSeconds("../breakcore", 5L);
		// }
		// if (glfwGetKey(Application::Window(), GLFW_KEY_L) == GLFW_PRESS)
		// {
		// 	LEI_TRACE("Pressing L Key");
		// 	AudioPlayer::PlaySFXForSeconds("landing", 5L);
		// }
	}

	void TestSceneAvery::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d