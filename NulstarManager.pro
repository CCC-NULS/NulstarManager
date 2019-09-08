TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += QuaZIP \
           Definitions \
           Tools \
           About \
           Log \
           Platform \
           Software \
           CreatePackage \
           Panel
OTHER_FILES += common.pri \
               .qmake.conf
