--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local M = Class()
local PrintString = UE.UKismetSystemLibrary.PrintString

local function Print(text, color, duration)
    color = color or UE.FLinearColor(1, 1, 1, 1)
    duration = duration or 2
    PrintString(nil, text, true, false, color, duration)
end



--function Lua_ActorTest:Initialize(Initializer)
--end

--function Lua_ActorTest:UserConstructionScript()
--end

--function Lua_ActorTest:ReceiveBeginPlay()
--end

--function Lua_ActorTest:ReceiveEndPlay()
--end

-- function Lua_ActorTest:ReceiveTick(DeltaSeconds)
-- end

--function Lua_ActorTest:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
--end

--function Lua_ActorTest:ReceiveActorBeginOverlap(OtherActor)
--end

--function Lua_ActorTest:ReceiveActorEndOverlap(OtherActor)
--end

return Lua_ActorTest
