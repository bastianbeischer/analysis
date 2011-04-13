!exists($(PERDAIXANA_PATH)) {
  error(Please set PERDAIXANA_PATH environment variable!)
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
  event \
  global \
  tracking \
  setup \
  particle \
  processing \
  alignment \
  rootplot \
  tools
