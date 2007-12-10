DEFINES += _QCODE_EDIT_BUILD_

CONFIG *= qnfa

QT *= xml

# Input

HEADERS += libqce/qce-config.h \
	libqce/qeditor.h \
	libqce/qeditorfactory.h \
	libqce/qcodeedit.h \
	libqce/qpanellayout.h \
	libqce/qformatfactory.h \
	libqce/qlinemarksinfocenter.h \
	libqce/document/qdocument.h \
	libqce/document/qdocument_p.h \
	libqce/document/qdocumentcommand.h \
	libqce/document/qdocumentcursor.h \
	libqce/document/qdocumentline.h \
	libqce/document/qdocumentsearch.h \
	libqce/language/qcompletionengine.h \
	libqce/language/qhighlighter.h \
	libqce/language/qhighlighterinterface.h \
	libqce/language/qindenter.h \
	libqce/language/qindenterinterface.h \
	libqce/language/qlanguagedefinition.h \
	libqce/language/qlanguagefactory.h \
	libqce/language/qmarker.h \
	libqce/language/qmarkerinterface.h \
	libqce/language/qmatcher.h \
	libqce/language/qmatcherinterface.h \
	libqce/widgets/qpanel.h \
	libqce/widgets/qlinenumberpanel.h \
	libqce/widgets/qlinemarkpanel.h \
	libqce/widgets/qfoldpanel.h \
	libqce/widgets/qstatuspanel.h \
	libqce/widgets/qsearchreplacepanel.h \
	libqce/widgets/qgotolinedialog.h \
	libqce/widgets/qeditconfig.h \
	libqce/widgets/qcalltip.h

SOURCES += libqce/qeditor.cpp \
	libqce/qeditorfactory.cpp \
	libqce/qcodeedit.cpp \
	libqce/qpanellayout.cpp \
	libqce/qformatfactory.cpp \
	libqce/qlinemarksinfocenter.cpp \
	libqce/document/qdocument.cpp \
	libqce/document/qdocumentcommand.cpp \
	libqce/document/qdocumentcursor.cpp \
	libqce/document/qdocumentline.cpp \
	libqce/document/qdocumentsearch.cpp \
	libqce/language/qcompletionengine.cpp \
	libqce/language/qhighlighter.cpp \
	libqce/language/qindenter.cpp \
	libqce/language/qlanguagedefinition.cpp \
	libqce/language/qlanguagefactory.cpp \
	libqce/language/qmarker.cpp \
	libqce/language/qmatcher.cpp \
	libqce/widgets/qpanel.cpp \
	libqce/widgets/qlinenumberpanel.cpp \
	libqce/widgets/qlinemarkpanel.cpp \
	libqce/widgets/qfoldpanel.cpp \
	libqce/widgets/qstatuspanel.cpp \
	libqce/widgets/qsearchreplacepanel.cpp \
	libqce/widgets/qgotolinedialog.cpp \
	libqce/widgets/qeditconfig.cpp \
	libqce/widgets/qcalltip.cpp

FORMS += libqce/widgets/searchreplace.ui \
	libqce/widgets/gotoline.ui \
	libqce/widgets/editconfig.ui

qnfa {
	DEFINES += QNFA_BUILD

	HEADERS += libqce/qnfa/qnfa.h \
		libqce/qnfa/qnfadefinition.h \
		libqce/qnfa/light_vector.h

	SOURCES += libqce/qnfa/qnfa.cpp \
		libqce/qnfa/qnfadefinition.cpp \
		libqce/qnfa/xml2qnfa.cpp
}

