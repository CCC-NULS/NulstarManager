TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += QuaZIP \
           Definitions \
           Tools \
           About \
           Log \
           Platform \
           Panel
OTHER_FILES += common.pri \
               .qmake.conf
