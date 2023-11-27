#pragma once

class CommandContext;

class Renderable {
public:
    virtual ~Renderable() {}
    virtual void OnRender(CommandContext& commandContext) = 0;
};