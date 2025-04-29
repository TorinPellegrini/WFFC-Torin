#include "MoveCommand.h"
#include "Game.h"

void MoveCommand::Undo(Game& game)
{
    for (size_t i = 0; i < m_selectedIDs.size(); ++i) {
        game.SetObjectPosition(m_selectedIDs[i], m_initialPositions[i]);

    }
}

void MoveCommand::Redo(Game& game)
{
    
       for (size_t i = 0; i < m_selectedIDs.size(); ++i) {
           game.SetObjectPosition(m_selectedIDs[i], m_finalPositions[i]);

       }
    

}
