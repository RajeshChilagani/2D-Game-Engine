#include "ClassLoader.h"
#include "Core/Math/Vector3.h"
#include "EngineHigh/GlobalSystem/GlobalSystem.h"
namespace Engine
{
	namespace ClassLoader
	{
		void ProcessFileContents(std::vector<uint8_t> i_FileContent, std::function<void(nlohmann::json)> i_Processor, Engine::Event* i_pFinishEvent)
		{
			if (!i_FileContent.empty())
			{
				if (!Engine::JobSystem::ShutdownRequested())
				{
					Json JsonData = Json::parse(i_FileContent);
					i_Processor(JsonData);
				}

			}

			std::cout << "ProcessFileContents finished processing file.\n";

			if (i_pFinishEvent)
				i_pFinishEvent->Signal();
		}

		void LoadJsonFromFile(const char* i_FileName)
		{
			using namespace std::placeholders;

			Engine::JobSystem::CreateQueue("Default", 2);

			{
				Engine::JobSystem::RunJob("CreateGameObjects", std::bind(ProcessFile(i_FileName, std::bind(CreateObjectsFromJSON, _1), nullptr)), "Default");

				do
				{
					Sleep(10);
				} while (Engine::JobSystem::HasJobs("Default"));
			}
			

		}
		void CreateObjectsFromJSON(Json i_JsonData)
		{
			for (unsigned int i = 0; i < i_JsonData["GameObject"].size(); ++i)
			{
				Json Entity = i_JsonData["GameObject"][i];
				std::string EntityName = Entity["Name"];
				GlobalSystem::AddEntityToWorld(EntityName,Entity);
			}

		}
	}
	
}