#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

//void PlayScene::moveStarShip() const
//{
//	if(m_bToggleGrid)
//	{
//		m_pStarShip->setDesiredVelocity(m_pTarget->getTransform()->position);
//		m_pStarShip->getRigidBody()->velocity = m_pStarShip->getDesiredVelocity();
//		m_pStarShip->getTransform()->position += m_pStarShip->getRigidBody()->velocity;
//	}
//}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Setup the Grid
	m_buildGrid();
	
	// Add Target to Scene
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pTarget);

	// Add StarShip to Scene
	m_pStarShip = new StarShip();
	m_pStarShip->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pStarShip);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - M2021 - Lab 4", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Checkbox("Toggle Grid", &m_bToggleGrid))
	{
		// toggle grid on/off
		m_setGridEnabled(m_bToggleGrid);
	}

	ImGui::Separator();

	static float start_position[2] = { m_pStarShip->getTransform()->position.x, m_pStarShip->getTransform()->position.y };
	if(ImGui::SliderFloat2("Start Position", start_position, 0.0f, 800.0f))
	{

	}

	ImGui::Separator();
	
	static float goal_position[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Goal Position", goal_position, 0.0f, 800.0f))
	{
		
	}

	ImGui::Separator();

	if (ImGui::Button("Reset"))
	{

	}

	
	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE; // alias for Tile size

	// add tiles to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create a new empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize); // world position
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabels();
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}
}

void PlayScene::m_setGridEnabled(const bool state)
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}

	m_isGridEnabled = state;
	
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}
