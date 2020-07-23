number = 10
array = {1, 1, 2, 3, 5, 10, 20}

-- simulation time control
startTime = 0.0
endTime   = 1.0
timestep  = 0.001

-- ensemble control
ensemble = {
    population = 1000,
    xDistribution  = {pdf = "gaussian", center = 0.1, width = 1.1},
    vxDistribution = {pdf = "gaussian", center = 0.2, width = 1.2},
    yDistribution  = {pdf = "gaussian", center = 0.3, width = 1.3},
    vyDistribution = {pdf = "gaussian", center = 0.4, width = 1.4},
    zDistribution  = {pdf = "delta", center = 0.5},
    vzDistribution = {pdf = "delta", center = 0.6}
}

-- a function that works with an Ensemble& (oh yeah!)
function my_function(ensemble)
    print("Hello there!")
    print(ensemble:get_population())
end


-------------------- FILTERING --------------------
-- user defines their filter functions active for this simulation like so

function filterFunc1(particle, time)
    if (particle:get_index() == 0 and time > 0.1) then 
        return true
    else
        return false
    end
end

function filterFunc2(particle, time) 
    return false
end

filters = { filterFunc1, filterFunc2 }

---------------------------------------------------

-- this should probably be in an auxillary  script that is invisible to the user
-- one of the two scripts needs a require() statement [I think]
-- even better, have this be in the C++ code?  can directly add individual lua functions to watcher/thruster etc.
-- doing it all here is probably faster since there is only one c++/lua interaction/exchange
function filter(particle, time)
    for _, f in ipairs(filters) do
        if (f(particle, time)) then return true end
    end
    return false
end

