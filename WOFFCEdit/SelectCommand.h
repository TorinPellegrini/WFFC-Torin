#pragma once
#include "pch.h"
#include "Command.h"
#include <vector>

class SelectCommand : public Command {
public:
    SelectCommand(std::vector<int> prev, std::vector<int> next)
        : m_prevSelection(prev), m_newSelection(next) {}

    void Undo(Game& game) override;
    void Redo(Game& game) override;

private:
    std::vector<int> m_prevSelection;
    std::vector<int> m_newSelection;
};