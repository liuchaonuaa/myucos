/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *Load;
    QGridLayout *gridLayout_6;
    QSpacerItem *verticalSpacer;
    QTextEdit *loadTextEdit;
    QProgressBar *loadProgressBar;
    QWidget *binView;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_2;
    QTextEdit *binTextEdit;
    QWidget *Set;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_3;
    QGroupBox *binBox;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *fileEdit;
    QPushButton *fileButton;
    QLabel *sizeLabel;
    QPushButton *loadButton;
    QPushButton *viewButton;
    QPushButton *eraseButton;
    QGridLayout *gridLayout_4;
    QGroupBox *usartBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *uNameLabel;
    QComboBox *uNameBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *parityLabel;
    QComboBox *parityBox;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QLabel *baundRateLabel;
    QComboBox *baundRateBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *dataBitLabel;
    QComboBox *dataBitBox;
    QHBoxLayout *horizontalLayout_8;
    QLabel *stopBitLabel;
    QComboBox *stopBitBox;
    QPushButton *openButton;
    QPushButton *refreshButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(420, 448);
        Widget->setMinimumSize(QSize(420, 448));
        Widget->setMaximumSize(QSize(420, 448));
        QFont font;
        font.setPointSize(12);
        Widget->setFont(font);
        Widget->setContextMenuPolicy(Qt::DefaultContextMenu);
        Widget->setAutoFillBackground(false);
        Widget->setStyleSheet(QString::fromUtf8("QTextEdit{		\n"
"	\n"
"	background-color: rgb(130, 162, 172);\n"
"	border:1px solid rgb(173, 216, 230);\n"
"	color: back;\n"
"}\n"
"/****************************************************************/\n"
"QScrollBar:vertical{\n"
"	border:2px solid rgb(173, 216, 230);\n"
"	background:lightblue;\n"
"	width: 20px;\n"
"	padding-top:20px;\n"
"	padding-bottom:20px;\n"
"}\n"
"QScrollBar::handle:vertical{\n"
"	width:8px;\n"
"	background:rgb(170, 211, 225);\n"
"	min-height:30px;\n"
"	margin: 1px 0px 1px 1px;\n"
"}\n"
"QScrollBar::add-line:vertical   \n"
"{\n"
"    height:20px;width:18px;\n"
"    subcontrol-position:bottom;\n"
"	background:rgb(173, 216, 230);\n"
"	border: 1px solid rgb(155, 155, 155);\n"
"	\n"
"	image: url(:/image/arrow_down.png);\n"
"}\n"
"QScrollBar::sub-line:vertical   \n"
"{\n"
"    height:20px;width:18px;\n"
"    subcontrol-position:top;\n"
"	background:rgb(173, 216, 230);\n"
"	border: 1px solid rgb(155, 155, 155);\n"
"	\n"
"	image: url(:/image/arrow_up.png);\n"
"}\n"
"/**************************QPus"
                        "hButton*************************/\n"
"QPushButton{\n"
"	background-color:lightblue;\n"
"	border:1px solid rgb(119, 149, 158);\n"
"	border-radius:0px;\n"
"	font:15px bold Khmer UI\n"
"}\n"
"QPushButton:hover{\n"
"	/*border:1px solid blue;*/\n"
"	background:rgb(170, 211, 225);\n"
"}\n"
"QPushButton:pressed{\n"
"	background: rgb(125, 155, 165);\n"
"}\n"
"/**************************\347\273\204\345\220\210\346\241\206***********************/\n"
"QComboBox{\n"
"	background-color: lightblue;\n"
"	border: 1px solid rgb(119, 149, 158);\n"
"	border-radius: 0px;\n"
"	padding:1px 2px 1px 2px;\n"
"	font:15px bold Khmer UI\n"
"}\n"
"QComboBox:hover {\n"
"	background:rgb(170, 211, 225);\n"
"}\n"
"QComboBox::dowm-arrow{\n"
"	\n"
"}\n"
"QComboBox::drop-down{\n"
"	subcontrol-origin:padding;\n"
"	subcontrol-position: top right;\n"
"	width:13px;\n"
"	\n"
"	border-left-width:2px;\n"
"	border-left-color:rgb(119, 149, 158);\n"
"	border-left-style:solid;\n"
"\n"
"	image: url(:/image/arrow_down.png);\n"
"}\n"
"/**********************"
                        "********QTabWidget************************/\n"
"QTabWidget::pane{\n"
"	border: 2px solid lightblue;\n"
"}\n"
"QTabWidget::tab-bar{\n"
"	left: 50px;\n"
"}\n"
"QTabBar::tab{\n"
"    background:lightblue;\n"
"    border: 1px solid rgb(119, 149, 158);\n"
"    border-top-left-radius: 0px;\n"
"    border-top-right-radius: 0px;\n"
"    min-width: 30ex;\n"
"    padding: 2px;	\n"
"}\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"	background:rgb(170, 211, 225);\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    border-color: #9B9B9B;\n"
"    border-bottom-color: lightblue; /* same as pane color */\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}\n"
"QTabBar::tab:selected {\n"
"    /* expand/overlap to the left and right by 4px */\n"
"    margin-left: -1px;\n"
"    margin-right: -1px;\n"
"}\n"
"QTabBar::tab:first:selected {\n"
"    margin-left: 0; /* the first selected tab has nothing to overlap with on the left */\n"
"}\n"
"\n"
"QTabBar::tab:last:selected "
                        "{\n"
"    margin-right: 0; /* the last selected tab has nothing to overlap with on the right */\n"
"}\n"
"\n"
"QTabBar::tab:only-one {\n"
"    margin: 0; /* if there is only one tab, we don't want overlapping margins */\n"
"}\n"
"/****************************Progress Bar***************************/\n"
"QProgressBar{\n"
"	border:2px solid rgb(119, 149, 158);\n"
"	text-align:center;\n"
"}\n"
"QProgressBar::chunk{\n"
"	background-color:rgb(173, 216, 230);\n"
"	width:20px;\n"
"	margin:0.5px;\n"
"}"));
        gridLayout = new QGridLayout(Widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QFont font1;
        font1.setFamily(QStringLiteral("Khmer UI"));
        font1.setPointSize(13);
        tabWidget->setFont(font1);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setMovable(false);
        Load = new QWidget();
        Load->setObjectName(QStringLiteral("Load"));
        gridLayout_6 = new QGridLayout(Load);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer, 2, 0, 1, 1);

        loadTextEdit = new QTextEdit(Load);
        loadTextEdit->setObjectName(QStringLiteral("loadTextEdit"));
        loadTextEdit->setMaximumSize(QSize(16777215, 311));
        QFont font2;
        font2.setFamily(QStringLiteral("Khmer UI"));
        font2.setPointSize(12);
        loadTextEdit->setFont(font2);
        loadTextEdit->setStyleSheet(QStringLiteral(""));
        loadTextEdit->setFrameShape(QFrame::Box);
        loadTextEdit->setFrameShadow(QFrame::Sunken);

        gridLayout_6->addWidget(loadTextEdit, 0, 0, 1, 1);

        loadProgressBar = new QProgressBar(Load);
        loadProgressBar->setObjectName(QStringLiteral("loadProgressBar"));
        loadProgressBar->setMaximumSize(QSize(16777215, 15));
        loadProgressBar->setValue(0);
        loadProgressBar->setTextVisible(true);
        loadProgressBar->setOrientation(Qt::Horizontal);
        loadProgressBar->setTextDirection(QProgressBar::TopToBottom);

        gridLayout_6->addWidget(loadProgressBar, 1, 0, 1, 1);

        tabWidget->addTab(Load, QString());
        binView = new QWidget();
        binView->setObjectName(QStringLiteral("binView"));
        gridLayout_2 = new QGridLayout(binView);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 1, 0, 1, 1);

        binTextEdit = new QTextEdit(binView);
        binTextEdit->setObjectName(QStringLiteral("binTextEdit"));
        QFont font3;
        font3.setFamily(QStringLiteral("Times New Roman"));
        font3.setPointSize(11);
        font3.setBold(false);
        font3.setWeight(50);
        font3.setKerning(true);
        binTextEdit->setFont(font3);

        gridLayout_2->addWidget(binTextEdit, 0, 0, 1, 1);

        tabWidget->addTab(binView, QString());
        Set = new QWidget();
        Set->setObjectName(QStringLiteral("Set"));
        gridLayout_5 = new QGridLayout(Set);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        binBox = new QGroupBox(Set);
        binBox->setObjectName(QStringLiteral("binBox"));
        verticalLayoutWidget_5 = new QWidget(binBox);
        verticalLayoutWidget_5->setObjectName(QStringLiteral("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(10, 50, 351, 61));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        fileEdit = new QLineEdit(verticalLayoutWidget_5);
        fileEdit->setObjectName(QStringLiteral("fileEdit"));
        fileEdit->setFont(font);
        fileEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        fileEdit->setReadOnly(false);

        horizontalLayout_9->addWidget(fileEdit);

        fileButton = new QPushButton(verticalLayoutWidget_5);
        fileButton->setObjectName(QStringLiteral("fileButton"));
        fileButton->setMinimumSize(QSize(75, 30));
        fileButton->setMaximumSize(QSize(75, 30));

        horizontalLayout_9->addWidget(fileButton);


        verticalLayout_7->addLayout(horizontalLayout_9);

        sizeLabel = new QLabel(verticalLayoutWidget_5);
        sizeLabel->setObjectName(QStringLiteral("sizeLabel"));
        sizeLabel->setMinimumSize(QSize(121, 21));
        sizeLabel->setMaximumSize(QSize(121, 21));

        verticalLayout_7->addWidget(sizeLabel);

        loadButton = new QPushButton(binBox);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(299, 130, 61, 30));
        QFont font4;
        font4.setFamily(QStringLiteral("12 Khmer UI"));
        font4.setBold(false);
        font4.setItalic(false);
        font4.setWeight(50);
        loadButton->setFont(font4);
        viewButton = new QPushButton(binBox);
        viewButton->setObjectName(QStringLiteral("viewButton"));
        viewButton->setGeometry(QRect(204, 130, 61, 30));
        viewButton->setFont(font4);
        eraseButton = new QPushButton(binBox);
        eraseButton->setObjectName(QStringLiteral("eraseButton"));
        eraseButton->setGeometry(QRect(100, 130, 61, 31));

        gridLayout_3->addWidget(binBox, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        usartBox = new QGroupBox(Set);
        usartBox->setObjectName(QStringLiteral("usartBox"));
        verticalLayoutWidget_2 = new QWidget(usartBox);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 30, 163, 72));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        uNameLabel = new QLabel(verticalLayoutWidget_2);
        uNameLabel->setObjectName(QStringLiteral("uNameLabel"));
        uNameLabel->setMinimumSize(QSize(50, 0));
        uNameLabel->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(uNameLabel);

        uNameBox = new QComboBox(verticalLayoutWidget_2);
        uNameBox->setObjectName(QStringLiteral("uNameBox"));
        uNameBox->setMinimumSize(QSize(99, 25));
        uNameBox->setMaximumSize(QSize(99, 16777215));

        horizontalLayout->addWidget(uNameBox);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        parityLabel = new QLabel(verticalLayoutWidget_2);
        parityLabel->setObjectName(QStringLiteral("parityLabel"));
        parityLabel->setMinimumSize(QSize(50, 0));
        parityLabel->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(parityLabel);

        parityBox = new QComboBox(verticalLayoutWidget_2);
        parityBox->setObjectName(QStringLiteral("parityBox"));
        parityBox->setMinimumSize(QSize(99, 25));
        parityBox->setMaximumSize(QSize(99, 16777215));

        horizontalLayout_2->addWidget(parityBox);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayoutWidget_4 = new QWidget(usartBox);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(190, 30, 173, 112));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(10);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        baundRateLabel = new QLabel(verticalLayoutWidget_4);
        baundRateLabel->setObjectName(QStringLiteral("baundRateLabel"));
        baundRateLabel->setMinimumSize(QSize(50, 0));
        baundRateLabel->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_3->addWidget(baundRateLabel);

        baundRateBox = new QComboBox(verticalLayoutWidget_4);
        baundRateBox->setObjectName(QStringLiteral("baundRateBox"));
        baundRateBox->setMinimumSize(QSize(99, 28));
        baundRateBox->setMaximumSize(QSize(99, 16777215));
        baundRateBox->setFont(font4);

        horizontalLayout_3->addWidget(baundRateBox);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(10);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        dataBitLabel = new QLabel(verticalLayoutWidget_4);
        dataBitLabel->setObjectName(QStringLiteral("dataBitLabel"));
        dataBitLabel->setMinimumSize(QSize(50, 0));
        dataBitLabel->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_7->addWidget(dataBitLabel);

        dataBitBox = new QComboBox(verticalLayoutWidget_4);
        dataBitBox->setObjectName(QStringLiteral("dataBitBox"));
        dataBitBox->setMinimumSize(QSize(99, 28));
        dataBitBox->setMaximumSize(QSize(99, 16777215));

        horizontalLayout_7->addWidget(dataBitBox);


        verticalLayout_6->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(10);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        stopBitLabel = new QLabel(verticalLayoutWidget_4);
        stopBitLabel->setObjectName(QStringLiteral("stopBitLabel"));
        stopBitLabel->setMinimumSize(QSize(50, 0));
        stopBitLabel->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_8->addWidget(stopBitLabel);

        stopBitBox = new QComboBox(verticalLayoutWidget_4);
        stopBitBox->setObjectName(QStringLiteral("stopBitBox"));
        stopBitBox->setMinimumSize(QSize(99, 28));
        stopBitBox->setMaximumSize(QSize(99, 16777215));
        stopBitBox->setFont(font4);

        horizontalLayout_8->addWidget(stopBitBox);


        verticalLayout_6->addLayout(horizontalLayout_8);

        openButton = new QPushButton(usartBox);
        openButton->setObjectName(QStringLiteral("openButton"));
        openButton->setEnabled(true);
        openButton->setGeometry(QRect(10, 120, 101, 41));
        openButton->setFont(font4);
        openButton->setCheckable(false);
        openButton->setAutoDefault(false);
        openButton->setDefault(false);
        openButton->setFlat(false);
        refreshButton = new QPushButton(usartBox);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));
        refreshButton->setGeometry(QRect(120, 120, 51, 41));

        gridLayout_4->addWidget(usartBox, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_4);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 1, 1);

        tabWidget->addTab(Set, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "\346\261\237\350\213\217\346\231\266\346\265\251\342\200\224\342\200\224\350\257\255\351\237\263\350\212\257\347\211\207\346\233\264\346\226\260", 0));
        tabWidget->setTabText(tabWidget->indexOf(Load), QApplication::translate("Widget", "bin\344\270\213\350\275\275", 0));
        tabWidget->setTabText(tabWidget->indexOf(binView), QApplication::translate("Widget", "bin\346\237\245\347\234\213", 0));
        binBox->setTitle(QApplication::translate("Widget", "\346\226\207\344\273\266\350\267\257\345\276\204", 0));
        fileButton->setText(QApplication::translate("Widget", "\346\265\217\350\247\210", 0));
        sizeLabel->setText(QApplication::translate("Widget", "Size\357\274\2320     B", 0));
        loadButton->setText(QApplication::translate("Widget", "\344\270\213\350\275\275", 0));
        viewButton->setText(QApplication::translate("Widget", "\346\237\245\347\234\213Bin", 0));
        eraseButton->setText(QApplication::translate("Widget", "\346\223\246\345\206\231", 0));
        usartBox->setTitle(QApplication::translate("Widget", "\344\270\262\345\217\243\351\205\215\347\275\256", 0));
        uNameLabel->setText(QApplication::translate("Widget", "\344\270\262\345\217\243\357\274\232", 0));
        parityLabel->setText(QApplication::translate("Widget", "\346\240\241\351\252\214\357\274\232", 0));
        baundRateLabel->setText(QApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", 0));
        dataBitLabel->setText(QApplication::translate("Widget", "\346\225\260\346\215\256\344\275\215\357\274\232", 0));
        stopBitLabel->setText(QApplication::translate("Widget", "\345\201\234\346\255\242\344\275\215\357\274\232", 0));
        openButton->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        refreshButton->setText(QApplication::translate("Widget", "\345\210\267\346\226\260", 0));
        tabWidget->setTabText(tabWidget->indexOf(Set), QApplication::translate("Widget", "\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
