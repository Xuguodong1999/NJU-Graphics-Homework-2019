# ������
# Դ��ʹ��GBK���룬����ǰ��������IDEǰ��ΪGBK����
# ���� qt creator������ ����-ѡ��-�ı��༭��-��Ϊ-�ļ����� ΪGBK
# ���� vs2015/2017/2019�������޸�Ĭ������
# ������
# ��̬�����ʱ��ע�͵�  main.cpp  �����
#     #include <QtCore/QtPlugin>
#     Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
# ������
# �ڷ�windowsƽ̨�����ʱ��ע�͵������ RC_FILE += 171860633_CG2019.rc
# ������

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/cli.cpp  src/cmd.cpp  src/main.cpp  src/proc.cpp  src/scribblearea.cpp

HEADERS += src/cli.h  src/cmd.h  src/proc.h  src/scribblearea.h

FORMS +=

# RC_ICONS = 171860633_CG2019.ico

RC_FILE += 171860633_CG2019.rc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += src/mainwidget.qrc

TARGET = index

DESTDIR = $${PWD}/../binary
