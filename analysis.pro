!exists($(PERDAIXANA_PATH)) {
  error(Please set PERDAIXANA_PATH environment variable!)
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
  event \
  global \
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
