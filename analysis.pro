!exists($(PERDAIXANA_PATH)) {
  error(Please set PERDAIXANA_PATH environment variable!)
}

TEMPLATE = subdirs

SUBDIRS = \
  global \
  event \
  widgets \
  setup \
  tracking \
  particle \
  likelihood \
  processing \
  trdAnalysis \
  alignment \
  rootplot \
  tools

macx {
  CONFIG+= ordered
}

tools.depends = global event widgets tracking setup particle likelihood trdAnalysis alignment rootplot
