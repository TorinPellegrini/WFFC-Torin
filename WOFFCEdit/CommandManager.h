#pragma once
#include <stack>
#include <memory>
#include "Command.h"

class CommandManager {
public:
    void ExecuteCommand(std::unique_ptr<Command> command, Game& game);

    void Undo(Game& game);
    void Redo(Game& game);

private:
    std::stack<std::unique_ptr<Command>> m_undoStack;
    std::stack<std::unique_ptr<Command>> m_redoStack;
};
