#pragma once

#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "core/Entity.hpp"
#include "core/Scene.hpp"
#include "physics/PhysicsWorld.hpp"
#include "rendering/Shader.hpp"

namespace lei3d
{

	class Entity;
	class Model;
	class Shader;
	class SkyBox;
	class PhysicsWorld;

	class SceneTutorial : public Scene
	{
	public:
		SceneTutorial();
		~SceneTutorial();

		void OnLoad() override;
		void OnPhysicsUpdate() override;
		void OnUpdate() override;
		void OnReset() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;
		std::unordered_map<std::string, std::unique_ptr<Model>> m_EnviromentModels;
		
		float m_fishTheta;
		float m_fishRadius;
		float m_deltaTheta;
	};

	extern std::unique_ptr<Scene> MakeSceneTutorial();
} // namespace lei3d