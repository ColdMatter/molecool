number = 10
array = {1, 1, 2, 3, 5, 10, 20}

-- simulation time control
startTime = 0.0
endTime   = 1.0
timestep  = 0.001

-- ensemble control
ensemble = {
    population = 1000,
    xDistribution  = {pdf = "gaussian", center = 0.0, width = 1.0},
    vxDistribution = {pdf = "gaussian", center = 0.0, width = 1.0},
    yDistribution  = {pdf = "gaussian", center = 0.0, width = 1.0},
    vyDistribution = {pdf = "gaussian", center = 0.0, width = 1.0},
    zDistribution  = {pdf = "delta", center = 0.0},
    vzDistribution = {pdf = "delta", center = 0.0}
}