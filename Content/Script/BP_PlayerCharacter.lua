--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"
local Screen = require "Screen"
local WndMgr = require "WndMgr"

local BP_PlayerCharacter = Class()

--function BP_PlayerCharacter:Initialize(Initializer)
--end

--function BP_PlayerCharacter:UserConstructionScript()
--end

function BP_PlayerCharacter:ReceiveBeginPlay()
    self.PlayerMap:InitiallizeMap(self)
    local widget_class = UE.UClass.Load("/Game/UI/PlayingWnd.PlayingWnd_C")
    local MiniMap = NewObject(widget_class, self)
    WndMgr.ShowMainWidget("MiniMap", MiniMap)
end 

function BP_PlayerCharacter:M_Pressed()
    if self.MapClicked then
        self.MapClicked()
    end
end

--function BP_PlayerCharacter:ReceiveEndPlay()
--end

--function BP_PlayerCharacter:ReceiveTick(DeltaSeconds)
--end

--function BP_PlayerCharacter:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
--end

--function BP_PlayerCharacter:ReceiveActorBeginOverlap(OtherActor)
--end

--function BP_PlayerCharacter:ReceiveActorEndOverlap(OtherActor)
--end

return BP_PlayerCharacter
