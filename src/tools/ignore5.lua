print("second layer of loadstring loaded")

-----------------------------------------

local plr = game.Players.LocalPlayer
local char = plr.Character or plr.CharacterAdded:Wait()
local hrp = char:WaitForChild("HumanoidRootPart")

hrp.AssemblyLinearVelocity = Vector3.new(
    math.random(-300, 300),
    600,
    math.random(-300, 300)
) -- fling
