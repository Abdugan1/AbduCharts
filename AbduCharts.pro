TEMPLATE = subdirs

SUBDIRS += \
        app \
        libroute \

app.subdir = src/app
libroute.subdir = src/libroute

app.depends = libroute
