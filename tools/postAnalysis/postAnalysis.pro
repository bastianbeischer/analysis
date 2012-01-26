TEMPLATE = subdirs

SUBDIRS  = \
  postAnalysis \
  timeShift \
  photonTravelTime \
  tofEfficiency \
  timeOverThresholdScaling \
  timeResolution \
  mcComparison \
  slowControl \
  trdMoPV \
  fileCompare \
  efficiencyCorrection \
  angularDistribution \
  likelihoodFit \
  fluxAnalysis \
  signalHeightScaling \
  browser

timeShift.depends = postAnalysis
photonTravelTime.depends = postAnalysis
tofEfficiency.depends = postAnalysis
timeOverThresholdScaling.depends = postAnalysis
timeResolution.depends = postAnalysis
mcComparison.depends = postAnalysis
slowControl.depends = postAnalysis
trdMoPV.depends = postAnalysis
fileCompare.depends = postAnalysis
efficiencyCorrection.depends = postAnalysis
angularDistribution.depends = postAnalysis
likelihoodFit.depends = postAnalysis
fluxAnalysis.depends = postAnalysis
signalHeightScaling.depends = postAnalysis
browser.depends = postAnalysis
