#include "TerrainCommand.h"
#include "Game.h"

TerrainCommand::TerrainCommand(DisplayChunk& chunk) : m_chunk(chunk) {}

void TerrainCommand::CaptureInitialState() {
    m_initialHeightMap.assign(std::begin(m_chunk.m_heightMap), std::end(m_chunk.m_heightMap));
}

void TerrainCommand::CaptureFinalState() {
    m_finalHeightMap.assign(std::begin(m_chunk.m_heightMap), std::end(m_chunk.m_heightMap));
}

void TerrainCommand::Undo(Game& game) {
    std::copy(m_initialHeightMap.begin(), m_initialHeightMap.end(), std::begin(m_chunk.m_heightMap));
    m_chunk.UpdateTerrain();
    m_chunk.CalculateTerrainNormals();
}

void TerrainCommand::Redo(Game& game) {
    std::copy(m_finalHeightMap.begin(), m_finalHeightMap.end(), std::begin(m_chunk.m_heightMap));
    m_chunk.UpdateTerrain();
    m_chunk.CalculateTerrainNormals();
}