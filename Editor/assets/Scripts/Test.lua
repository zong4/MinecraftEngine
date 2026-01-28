return {
    OnCreate = function(self)
        print("Lua OnStart")
    end,

    OnUpdate = function(self, deltaTime)
        print("Lua OnUpdate", deltaTime)
    end,

    OnDestroy = function(self)
        print("Lua OnDestroy")
    end
}
