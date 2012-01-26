!exists($(PERDAIXANA_PATH)) {
  error(Please set PERDAIXANA_PATH environment variable!)
}

TEMPLATE = subdirs

SUBDIRS = \
  global \
  event \
  setup \
  tracking \
  particle \
  likelihood \
  processing \
  trdAnalysis \
  alignment \
  rootplot \
  fluxAnalysis \
  tools

setup.depends        = global event
tracking.depends     = global event setup
particle.depends     = global event tracking setup
likelihood.depends   = global event tracking setup particle
processing.depends   = global event tracking setup particle likelihood
trdAnalysis.depends  = global event tracking particle processing
alignment.depends    = event tracking setup particle processing
rootplot.depends     = global
fluxAnalysis.depends = global particle rootplot
tools.depends        = global event tracking setup particle likelihood alignment rootplot fluxAnalysis
