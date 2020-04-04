#pragma once
#include "Vendor/Json.h"
#include "EngineLow/FileIO/LoadFile.h"
#include "Core/JobSystem/JobSystem/JobSystem.h"
#include "Core/JobSystem/Syncronization/Events.h"
#include <vector>
#include <iostream>
namespace Engine
{
	namespace ClassLoader
	{
		void ProcessFileContents(std::vector<uint8_t> i_FileContent, std::function<void(nlohmann::json)> i_Processor, Engine::Event* i_pFinishEvent = nullptr);
		class ProcessFile
		{
		public:
			ProcessFile(const char* i_pFilename, std::function<void(nlohmann::json)> i_Processor, Engine::Event* i_pFinishEvent = nullptr) :
				m_pFilename(i_pFilename),
				m_Processor(i_Processor),
				m_pFinishEvent(i_pFinishEvent)
			{
				assert(m_pFilename);
			}

			void operator() ()
			{
				if (m_pFilename)
				{

					std::vector<uint8_t> FileContents = LoadFileToBuffer(m_pFilename);

					if (!FileContents.empty())
					{
						if (Engine::JobSystem::ShutdownRequested())
						{

						}
						else
						{
							std::cout << "ProcessFile finished loading file.\n";

							// this works around C++11 issue with capturing member variable by value
							std::function<void(nlohmann::json)> Processor = m_Processor;
							Engine::Event* pFinishEvent = m_pFinishEvent;

							Engine::JobSystem::RunJob("ProcessFileContents",
								[FileContents, Processor, pFinishEvent]()
							{
								ProcessFileContents(FileContents, Processor, pFinishEvent);
							},
								"Default"
								);

						}
					}
				}
			}


		private:
			const char* m_pFilename;
			std::function<void(nlohmann::json)>  m_Processor;
			Engine::Event* m_pFinishEvent;
		};
		void LoadJsonFromFile(const char* i_FileName);
		void CreateObjectsFromJSON(Json i_JsonData);		
	}
}
