#include "CommandManager.h"

void CommandManager::ExecuteCommand(std::unique_ptr<Command> command, Game& game) {
    command->Redo(game); // Apply action immediately
    m_undoStack.push(std::move(command));
    while (!m_redoStack.empty()) m_redoStack.pop(); // Clear redo stack
}

void CommandManager::Undo(Game& game) {
    if (m_undoStack.empty()) return;
    auto command = std::move(m_undoStack.top()); m_undoStack.pop();
    command->Undo(game);
    m_redoStack.push(std::move(command));
}

void CommandManager::Redo(Game& game) {
    if (m_redoStack.empty()) return;
    auto command = std::move(m_redoStack.top()); m_redoStack.pop();
    command->Redo(game);
    m_undoStack.push(std::move(command));
}