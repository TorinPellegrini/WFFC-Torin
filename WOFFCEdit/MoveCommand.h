#pragma once
#include "d3d11.h"
#include <SimpleMath.h>
#include <vector>

#include "Command.h"
#include "DisplayObject.h"

class MoveCommand : public Command
{
public:

    MoveCommand(const std::vector<int>& selectedIDs, const std::vector<DirectX::SimpleMath::Vector3>& initialPositions, const std::vector<DirectX::SimpleMath::Vector3>& finalPositions)
        : m_selectedIDs(selectedIDs), m_initialPositions(initialPositions), m_finalPositions(finalPositions) {}

    // Undo the movement: move objects back to their initial positions
    virtual void Undo(Game& game) override;

    // Redo the movement: move objects to their final positions
    virtual void Redo(Game& game) override;
private:
    std::vector<int> m_selectedIDs;        // The IDs of selected objects
    std::vector<DirectX::SimpleMath::Vector3> m_initialPositions;  // Initial positions of the objects
    std::vector<DirectX::SimpleMath::Vector3> m_finalPositions;  // Final positions of the objects
};

