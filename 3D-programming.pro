QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11
CONFIG      +=resources_big

TARGET      = 3D-programming

SOURCES += main.cpp \
    billboard.cpp \
    material.cpp \
    matrix4x4.cpp \
    pyramid.cpp \
    skybox.cpp \
    startmenudialog.cpp \
    textureshader.cpp \
    vec3.cpp \
    vec2.cpp \
    vec4.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    vertex.cpp \
    triangle.cpp \
    octahedron.cpp \
    sceneobject.cpp \
    texture.cpp \
    quaternion.cpp \
    ball.cpp \
    transform.cpp \
    camera.cpp \
    plane.cpp \
    house.cpp \
    beziercurve.cpp \
    beziercurvesmooth.cpp \
    curvedplane.cpp \
    light.cpp \
    trophie.cpp \
    enemy.cpp

HEADERS += \
    billboard.h \
    material.h \
    matrix4x4.h \
    pyramid.h \
    skybox.h \
    startmenudialog.h \
    textureshader.h \
    vec3.h \
    vec2.h \
    vec4.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    vertex.h \
    triangle.h \
    octahedron.h \
    sceneobject.h \
    texture.h \
    quaternion.h \
    ball.h \
    transform.h \
    camera.h \
    plane.h \
    constant.h \
    house.h \
    beziercurve.h \
    beziercurvesmooth.h \
    curvedplane.h \
    light.h \
    trophie.h \
    enemy.h

FORMS += \
    mainwindow.ui \
    startmenudialog.ui

DISTFILES += \
    billboard.frag \
    billboard.vert \
    plainfragment.frag \
    plainvertex.vert \
    textureshader.frag \
    textureshader.vert \
    phong.frag \
    phong.vert

RESOURCES +=
