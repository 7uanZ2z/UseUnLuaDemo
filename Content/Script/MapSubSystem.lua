local M = {}

function M.SetMapConner(WorldMap)
    M.MapSystem = UE.USubsystemBlueprintLibrary.GetGameInstanceSubsystem(WorldMap, UE.UMapSubsystem)
    WorldMap.UpperLeftConner = M.MapSystem:GetActorInLevelByName("UpperLeft")
    WorldMap.DownRightConner = M.MapSystem:GetActorInLevelByName("LowerRight")
end

function M.NewRandomMap(length, width, mainlength)
    return M.MapSystem:SetNewMap(length, width, mainlength)
end

return M