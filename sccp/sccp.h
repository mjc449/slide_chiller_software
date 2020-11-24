#ifndef _sccp_h
#define _sccp_h


#include <QtWidgets/QMainWindow>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QWidget>
#include <QtCharts>
#include <QSerialPort>
#include <QSerialPortInfo>
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
   QHBoxLayout *_parentLayout, *_tuningButtonsLayout, *_refreshRateLayout, *_deviceStatusHBoxLayout;
   QVBoxLayout *_controlsVerticalLayout, *_dataVerticalLayout;
   QGridLayout *_comLayout, *_tuningLayout, *_temperatureLayout, *_plotScaleLayout, *_deviceStatusLayout;

   //Frames
   QFrame *_tuningFrame, *_refreshRateFrame, *_temperatureFrame, *_plotScaleFrame;

   //Buttons, boxes, etc.
   QComboBox *_comPortComboBox;
   QPushButton *_comConnectButton, *_comScanButton, *_tuningResetButton, *_tuningUpdateButton, *_setTemperatureButton, *_rampTemperatureButton, *_livePlotButton, *_refreshRateButton;
   QButtonGroup *_xScaleButtonGroup, *_loopTcButtonGroup;
   QRadioButton *_xScaleManualButton, *_xScaleAutomaticButton, *_loopTc1Button, *_loopTc2Button;

   //Labels
   QLabel *_comPortLabel, *_deviceStatusLabel, *_kpLabel, *_kiLabel, *_kdLabel, *_setPointLabel, *_rampRateLabel, *_startRampLabel, *_refreshRateLabel, *_xScaleLabel, *_xScaleMinLabel, *_xScaleMaxLabel, *_yScaleLabel, *_loopTcLabel, *_tc1CalibrationLabel, *_tc2CalibrationLabel, *_tc1ReadingLabel, *_tc2ReadingLabel, *_tecCurrentReadingLabel, *_communicationStatusLabel;

   //Input fields
   QLineEdit *_setPointLineEdit, *_rampRateLineEdit, *_refreshRateLineEdit, *_xScaleMinLineEdit, *_xScaleMaxLineEdit, *_yScaleWindowLineEdit, *_kpLineEdit, *_kiLineEdit, *_kdLineEdit, *_tc1CalibrationLineEdit, *_tc2CalibrationLineEdit, *_tc1ReadingLineEdit, *_tc2ReadingLineEdit, *_tecCurrentReadingLineEdit, *_deviceStatusLineEdit, *_communicationStatusLineEdit;

   //Graphics
   QChart *_temperaturePlot;
   QChartView *_temperaturePlotView;
   QValueAxis *_xAxisTime, *_yAxisTemperature, *_yAxisCurrent;

   //Device data
   QLineSeries *_tc1DataSeries;
   QLineSeries *_tc2DataSeries;
   QLineSeries *_tecCurrentDataSeries;

   //Serial connection
   QList<QSerialPortInfo> _serialPortList;
   QSerialPort *_serialPort;

   //Timer

   //Functions
   void SetupGUI();
   void SetupSerial();
   void SetupConnections();
   int SerialTransfer();
   int GetSensorValues();
   int GetPIDConstants();

   //Variables
   double _setPoint{ 25.0 };
   double _rampRate{ 1.0 };
   double _plotMinTemp{ 0.00 };
   double _plotMaxTemp{ 100.0 };
   double _chartLength{ 60 };
   double _refreshRate{ 1.0 };
   int _loopControlTc{ 0 };
   double _Kp{ 2.0 };
   double _Ki{ 5.0 };
   double _Kd{ 1.0 };
   double _tc1Calibration{ 0.0 };
   double _tc2Calibration{ 0.0 };
   double _tc1Temperature{};
   double _tc2Temperature{};
   double _tecCurrent{};
   char _serialBuffer[25];

signals:

public slots:

private slots:
   void on_scan_button_clicked();
   void on_connect_button_clicked();
};

#endif // !_sccp_h