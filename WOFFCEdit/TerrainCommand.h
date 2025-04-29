#pragma once
#include "Command.h"
#include "DisplayChunk.h"
#include <vector>

class TerrainCommand : public Command {
public:
    TerrainCommand(DisplayChunk& chunk);

    void CaptureInitialState();
    void CaptureFinalState();

    virtual void Undo(Game& game) override;
    virtual void Redo(Game& game) override;

private:
    DisplayChunk& m_chunk;
    std::vector<unsigned char> m_initialHeightMap;
    std::vector<unsigned char> m_finalHeightMap;
};