#ifndef _sccp_h
#define _sccp_h


#include <QtWidgets/QMainWindow>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QWidget>
#include <QtCharts\QtCharts>
#include "ui_sccp.h"

class sccp : public QMainWindow
{
    Q_OBJECT

public:
    sccp(QWidget *parent = Q_NULLPTR);

private:
    Ui::sccpClass ui;

    //Central widget for main window
    QWidget *_centralWidget;

    //Layouts
    QHBoxLayout *_parentLayout, *_tuningButtonsLayout, *_refreshRateLayout;
    QVBoxLayout *_controlsVerticalLayout, *_dataVerticalLayout;
    QGridLayout *_comLayout, *_tuningLayout, *_temperatureLayout, *_plotScaleLayout;

    //Frames
    QFrame *_tuningFrame, *_refreshRateFrame, *_temperatureFrame, *_plotScaleFrame;

    //Buttons, boxes, etc.
    QComboBox *_comPortComboBox;
    QPushButton *_comConnectButton, *_comScanButton, *_tuningResetButton, *_tuningUpdateButton, *_setTemperatureButton, *_rampTemperatureButton, *_livePlotButton, *_refreshRateButton;
    QButtonGroup *_xScaleButtonGroup;
    QRadioButton *_xScaleManualButton, *_xScaleAutomaticButton;

    //Labels
    QLabel *_comPortLabel, *_statusLabel, *_kpLabel, *_kiLabel, *_kdLabel, *_setPointLabel, *_rampRateLabel, *_startRampLabel, *_refreshRateLabel, *_xScaleLabel, *_xScaleMinLabel, *_xScaleMaxLabel, *_yScaleLabel;

    //Input fields
    QLineEdit *_kpLineEdit, *_kiLineEdit, *_kdLineEdit, *_setPointLineEdit, *_rampRateLineEdit, *_refreshRateLineEdit, *_xScaleMinLineEdit, *_xScaleMaxLineEdit, *_yScaleWindowLineEdit;

    //Graphics
    QChartView *_temperaturePlot;

    //Functions
    void SetupGUI();
    void SetupCom();

signals:

public slots:

private slots:
   void on_scan_button_clicked();
};

#endif // !_sccp_h