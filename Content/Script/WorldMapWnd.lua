--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"
local Screen = require "Screen"
local WorldMapWnd = Class()

function WorldMapWnd:DrawMapBlock(x, y, divX, divY)
    if self.divX == nil and divX ~= nil then self.divX = divX end
    if self.divY == nil and divY ~= nil then self.divY = divY end
    
    local parent = self.MapCanvas
    local image = NewObject(UE.UImage)
    local parentsize = parent.Slot:GetSize()
    local Xsize = parentsize.X / divX
    local Ysize = parentsize.Y / divY
    local anchors = UE.FAnchors(0.0)
    anchors.Minimum = UE.FVector2D(0.0, 1.0)
    anchors.Maximum = UE.FVector2D(0.0, 1.0)

    parent:AddChild(image)
    image.Slot:SetZOrder(1)
    image.Slot:SetAnchors(anchors)
    image.Slot:SetSize(UE.FVector2D(Xsize, Ysize))
    image.Slot:SetAlignment(UE.FVector2D(0.0, 1.0))
    image.Slot:SetPosition(UE.FVector2D(Xsize * (x - 1), - Ysize * (y - 1)))

    return image
end

function WorldMapWnd:InitCharacterBlock(divX, divY)
    if self.divX == nil and divX ~= nil then self.divX = divX end
    if self.divY == nil and divY ~= nil then self.divY = divY end

    local widget_class = UE.UClass.Load("/Game/WorldMapSystem/W_WorldMapCharacter.W_WorldMapCharacter_C")
    local CharacterBlock = NewObject(widget_class)
    self.m_CharacterBlock = CharacterBlock
    local parent = self.MapCanvas
    local parentsize = parent.Slot:GetSize()
    local Xsize = CharacterBlock.SizeRate * parentsize.X / self.divX
    local Ysize = CharacterBlock.SizeRate * parentsize.Y / self.divY
    local anchors = UE.FAnchors(0.0)
    anchors.Minimum = UE.FVector2D(0.0, 1.0)
    anchors.Maximum = UE.FVector2D(0.0, 1.0)

    parent:AddChild(CharacterBlock)
    CharacterBlock.Slot:SetZOrder(1)
    CharacterBlock.Slot:SetAnchors(anchors)
    CharacterBlock.Slot:SetSize(UE.FVector2D(Xsize, Ysize))
    CharacterBlock.Slot:SetAlignment(UE.FVector2D(0.5, 0.5))
    CharacterBlock.Slot:SetPosition(UE.FVector2D(0.0, 0.0))
end

function WorldMapWnd:SetCharacterPosition(PlayerX, PlayerY, MapX, MapY)
    if self.m_CharacterBlock == nil then return end 
    local parent = self.MapCanvas
    local parentsize = parent.Slot:GetSize()
    self.m_CharacterBlock.Slot:SetPosition(UE.FVector2D(parentsize.Y * PlayerY / MapY, -parentsize.X * PlayerX / MapX))
end

function WorldMapWnd:SetCharacterRotator(Yaw)
    if self.m_CharacterBlock == nil then return end 
    self.m_CharacterBlock:SetRenderTransformAngle(Yaw)
end

function WorldMapWnd:ClearMap()
    self.MapCanvas:ClearChildren()
end


-- function WorldMapWnd:Initialize(Initializer)
-- end

--function WorldMapWnd:PreConstruct(IsDesignTime)
--end

-- function WorldMapWnd:Construct()
-- end

-- function WorldMapWnd:Tick(MyGeometry, InDeltaTime)  
-- end

return WorldMapWnd
