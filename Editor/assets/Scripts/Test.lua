local Script = {}

function Script:OnCreate()
    local transform = self.Entity:GetTransform()
    print("Lua OnCreate:", transform.Position.x, transform.Position.y, transform.Position.z)
end

function Script:OnUpdate(dt)
end

function Script:OnDestroy()
end

return Script