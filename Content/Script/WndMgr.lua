local M = {}

local MainWidget = {}
local SubWidget = {}

function M.ShowMainWidget(Name, Wnd)
    MainWidget[Name] = Wnd
    Wnd:AddToViewport()
end

function M.CloseMainWidget(Name)
    MainWidget[Name]:RemoveFromParent()
    MainWidget[Name] = nil
end

function M.GetMainWidget(Name)
    return MainWidget[Name]
end

function M.SetSubWidget(Name, Wnd)
    SubWidget[Name] = Wnd
end

function M.GetSubWidget(Name)
    return SubWidget[Name]
end

function M.RemoveSubWidget(Name)
    SubWidget[Name] = nil
end


return M