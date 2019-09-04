TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += QuaZIP \
           Definitions \
           Tools \
           About \
           Log \
           Platform \
           Software \
           Panel
OTHER_FILES += common.pri \
               .qmake.conf
