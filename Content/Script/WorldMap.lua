--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local MapSubSystem = require "MapSubSystem"
local Screen = require "Screen"

local WorldMap = Class()

WMWnd = {}
WMWnd.Open = {}

function WorldMap:GetMapHeight()

    local ULConner = self.UpperLeftConner:K2_GetActorLocation()
    local DRConner = self.DownRightConner:K2_GetActorLocation()

    return math.abs(ULConner.X - DRConner.X)
end

function WorldMap:GetMapWidth()
    local ULConner = self.UpperLeftConner:K2_GetActorLocation()
    local DRConner = self.DownRightConner:K2_GetActorLocation()

    return math.abs(ULConner.Y - DRConner.Y)
end

function WorldMap:ConvertLocationToMapLocation(Location)
    local ULConner = self.UpperLeftConner:K2_GetActorLocation()
    local ret = {}
    ret.X = math.abs(ULConner.X - Location.X)
    ret.Y = math.abs(ULConner.Y - Location.Y)
    return ret
end


function WorldMap:InitiallizeMap(Player)
    self.m_Player = Player
    self.m_MapBlockX = 20
    self.m_MapBlockY = 20
    self.m_LevelLength = 30

    MapSubSystem.SetMapConner(self)
    self.bHasScriptImplementedTick = false

    local widget_class = UE.UClass.Load("/Game/WorldMapSystem/W_WorldMap.W_WorldMap_C")
    WMWnd[Player] = NewObject(widget_class, Player)
    WMWnd[Player]:AddToViewport()
    WMWnd.Open[Player] = true
    self.m_Wnd = WMWnd[Player]
    self.bCanEverTick = false
    Player.MapClicked = function()
        if WMWnd.Open[Player] == true then 
            WMWnd.Open[Player] = false
            WMWnd[Player]:RemoveFromParent()
        else 
            WMWnd.Open[Player] = true
            WMWnd[Player]:AddToViewport()
        end
    end

    Player.MapClicked()
    
    self.m_Wnd:ClearMap()
    for i = 1, 10 do
        self.m_Wnd:DrawMapBlock(i, i, self.m_MapBlockX, self.m_MapBlockY)
    end
    self.m_Wnd:ClearMap()

    self.m_Wnd:InitCharacterBlock()
    local PlayerPosition = self:ConvertLocationToMapLocation(self.m_Player:K2_GetActorLocation())
    self.m_Wnd:SetCharacterPosition(PlayerPosition.X, PlayerPosition.Y, 
            self:GetMapHeight(), self:GetMapWidth())

    local v = MapSubSystem.NewRandomMap(20, 20, 10)
    for i = 1, v:Length() do
        local nowpoint = v:Get(i)
        self.m_Wnd:DrawMapBlock(nowpoint.X, nowpoint.Y, self.m_MapBlockX, self.m_MapBlockY)
    end
    -- print(type(v))
    -- print(type(v:Get(1).a:Get(2)))
    --print(type(v[0]))
end


--function WorldMap:Initialize(Initializer)
--end

--function WorldMap:ReceiveBeginPlay()
--end

--function WorldMap:ReceiveEndPlay()
--end

function WorldMap:ReceiveTick(DeltaSeconds)
    if self.m_Player ~= nil and WMWnd.Open[self.m_Player] == true then
        local Rotation = self.m_Player:K2_GetActorRotation()
        local PlayerPosition = self:ConvertLocationToMapLocation(self.m_Player:K2_GetActorLocation())
        
        self.m_Wnd:SetCharacterPosition(PlayerPosition.X, PlayerPosition.Y, 
                self:GetMapHeight(), self:GetMapWidth())
        self.m_Wnd:SetCharacterRotator(Rotation.Yaw)
    end
end

return WorldMap
