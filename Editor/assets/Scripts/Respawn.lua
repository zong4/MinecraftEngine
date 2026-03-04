local Script = {}

local resetY = -1.0
local spawnPosition = nil

function Script:OnCreate()
    local transform = self.Entity:GetTransform()
    spawnPosition = {
        x = transform.Position.x,
        y = transform.Position.y,
        z = transform.Position.z
    }
end

function Script:OnUpdate(dt)
    local transform = self.Entity:GetTransform()
    if transform.Position.y < resetY then
        transform.Position.x = spawnPosition.x
        transform.Position.y = spawnPosition.y
        transform.Position.z = spawnPosition.z
        local rigidBody = self.Entity:GetRigidBody()
        rigidBody:Reset()
    end
end

function Script:OnDestroy()
end

return Script