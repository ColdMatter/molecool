number = 10
array = {1, 1, 2, 3, 5, 10, 20}

-- simulation time control
startTime = 0.0
endTime   = 1.0
timestep  = 0.001

-- ensemble control
ensemble = {
    population = 100,
    xDistribution  = {pdf = "gaussian", center = 0.1, width = 1.1},
    vxDistribution = {pdf = "gaussian", center = 0.2, width = 1.2},
    yDistribution  = {pdf = "gaussian", center = 0.3, width = 1.3},
    vyDistribution = {pdf = "gaussian", center = 0.4, width = 1.4},
    zDistribution  = {pdf = "delta", center = 0.5},
    vzDistribution = {pdf = "delta", center = 0.6}
}

function addStuff(a, b)
    print("[LUA]: adding " .. a .. " and " .. b)
    return a + b
end

-- a function that works with an Ensemble& (oh yeah!)
function my_function(ensemble)
    print("Hello there!")
    print(ensemble:get_population())
end



--/////////////// FILTERING ////////////////////////
function filterFunc1()
    print("func1!")
    return false
end

function filterFunc2() 
    print("func2!")
    return false
end

-- user defines their filter functions active for this simulation like so
filters = { filterFunc1, filterFunc2 }
--//////////////////////////////////////////////////

-- this should probably be in an auxillary  script that is invisible to the user
-- one of the two scripts needs a require() statement [I think]
function apply_filters()
    for _, filter in ipairs(filters) do
        if (filter()) then return true end
    end
    return false
end

