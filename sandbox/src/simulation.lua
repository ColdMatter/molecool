

-- simulation time control
startTime = 0.0
endTime   = 1.0
timestep  = 0.001

-- ensemble control
ensemble = {
    population = 1000,
    --species = "CaF",
    xDistribution  = {pdf = "gaussian", center = 0.1, width = 1.1},
    vxDistribution = {pdf = "gaussian", center = 0.2, width = 1.2},
    yDistribution  = {pdf = "gaussian", center = 0.3, width = 1.3},
    vyDistribution = {pdf = "gaussian", center = 0.4, width = 1.4},
    zDistribution  = {pdf = "delta", center = 0.5},
    vzDistribution = {pdf = "delta", center = 0.6}
}


