#pragma once
#include <SimpleMath.h>
#include <vector>

#include "Command.h"
class MoveCommand : public Command
{
public:
    //TODO: get the displaylist from game over here to get the object positions
    MoveCommand(const std::vector<int>& selectedIDs, const std::vector<DirectX::SimpleMath::Vector3>& initialPositions, const std::vector<DirectX::SimpleMath::Vector3>& finalPositions)
        : m_selectedIDs(selectedIDs), m_initialPositions(initialPositions), m_finalPositions(finalPositions) {}

    // Undo the movement: move objects back to their initial positions
    virtual void Undo(Game& game) override {
        for (size_t i = 0; i < m_selectedIDs.size(); ++i) {
            int id = m_selectedIDs[i];
            m_displayList[id].m_position = m_initialPositions[i];
        }
    }

    // Redo the movement: move objects to their final positions
    virtual void Redo(Game& game) override {
        for (size_t i = 0; i < m_selectedIDs.size(); ++i) {
            int id = m_selectedIDs[i];
            m_displayList[id].m_position = m_finalPositions[i];
        }
    }

private:
    std::vector<int> m_selectedIDs;        // The IDs of selected objects
    std::vector<DirectX::SimpleMath::Vector3> m_initialPositions;  // Initial positions of the objects
    std::vector<DirectX::SimpleMath::Vector3> m_finalPositions;  // Final positions of the objects
};

