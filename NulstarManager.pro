TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += QuaZIP \
           Definitions \
           Tools \
           About \
           Log \
           Panel
OTHER_FILES += common.pri \
               .qmake.conf
