TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += QuaZIP \
           Definitions \
           Tools \
           About \
           #Qftp \
           #Connector \
           #Log \
           #SystemObject \
           #Software \
           #Client \
           Panel
OTHER_FILES += common.pri \
               .qmake.conf
