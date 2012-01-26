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

macx {
  CONFIG+= ordered
}

tools.depends = global event tracking setup particle likelihood trdAnalysis alignment rootplot fluxAnalysis
