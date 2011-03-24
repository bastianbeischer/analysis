!exists($(PERDAIXANA_PATH)) {
  error(Please set PERDAIXANA_PATH environment variable!)
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
  global \
  event \
  tracking \
  setup \
  processing \
  alignment \
  rootplot \
  tools
