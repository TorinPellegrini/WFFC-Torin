#pragma once

class Game; 

class Command {
public:
    virtual ~Command() {}
    virtual void Undo(Game& game) = 0;
    virtual void Redo(Game& game) = 0;
};