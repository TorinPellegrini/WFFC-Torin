#include "SelectCommand.h"
#include "Game.h"

void SelectCommand::Undo(Game& game) {
    game.SetSelection(m_prevSelection);
}

void SelectCommand::Redo(Game& game) {
    game.SetSelection(m_newSelection);
}