export IGNORE_SOURCES=" \
  src/plugins/storagebackends/nosqlbackend/*/* \
  src/libelos_lite/private/* \
  src/libelos_lite/public/elos/libelos_lite.h \
  src/libelos_lite/public/elos/lite/event.h \
  src/demos/demo_eloslite.c \
  test/utest/libelos_lite/eloslitePublish/* \
  # temporary solution, should be removed if connectionmanager is reworked
  src/plugins/clients/connectionmanager/interface/connectionmanager/* \
  "
export UNUSED_SOURCES=" \
  src/components/processfilter/public/elos/processfilter/vector.h \
  "
