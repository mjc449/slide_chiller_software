#include "sccp.h"

sccp::sccp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    SetupGUI();
    SetupSerial();
    SetupConnections();
}

void sccp::SetupGUI()
{
   //Create the central widget
   _centralWidget = new QWidget;

   //Create the principle layouts for the main window
   _parentLayout = new QHBoxLayout;
   _controlsVerticalLayout = new QVBoxLayout;
   _dataVerticalLayout = new QVBoxLayout;

   //Setup the com connection controls
   _comPortLabel = new QLabel;
   _comPortLabel->setText(QString("Port:"));
   _comScanButton = new QPushButton;
   _comScanButton->setText(QString("Scan"));
   _comScanButton->setToolTip(QString("Scans the system for available devices."));
   _comConnectButton = new QPushButton;
   _comConnectButton->setText(QString("Connect"));
   _comConnectButton->setToolTip(QString("Opens a connection to the selected device."));
   _comConnectButton->setCheckable(true);
   _comPortComboBox = new QComboBox;
   _comPortComboBox->setEditable(false);
   _comPortComboBox->setMinimumContentsLength(16);

   //Populate the com control layout
   _comLayout = new QGridLayout;
   _comLayout->addWidget(_comPortLabel, 0, 0, Qt::AlignLeft);
   _comLayout->addWidget(_comScanButton, 0, 1, Qt::AlignRight);
   _comLayout->addWidget(_comPortComboBox, 1, 0, Qt::AlignLeft);
   _comLayout->addWidget(_comConnectButton, 1, 1, Qt::AlignRight);

   //Setup the temperature controls
   _setPointLabel = new QLabel;
   _setPointLabel->setText(QString("Set point (C)"));
   _setPointLineEdit = new QLineEdit;
   _setPointLineEdit->setValidator(new QDoubleValidator(0, 100, 2));
   _setPointLineEdit->setToolTip(QString("Valid entries 0.0 - 100.0"));
   _setPointLineEdit->setMaxLength(6);
   _setPointLineEdit->setMaximumWidth(50);
   _setPointLineEdit->setAlignment(Qt::AlignRight);
   _setPointLineEdit->setText(QString::number(_setPoint));
   _setTemperatureButton = new QPushButton;
   _setTemperatureButton->setText(QString("Update"));
   _rampRateLabel = new QLabel;
   _rampRateLabel->setText(QString("Ramp (C/min)"));
   _rampRateLineEdit = new QLineEdit;
   _rampRateLineEdit->setValidator(new QDoubleValidator(0.1, 120, 2));
   _rampRateLineEdit->setToolTip("Valid entries 0.1 - 120.0");
   _rampRateLineEdit->setMaxLength(6);
   _rampRateLineEdit->setMaximumWidth(50);
   _rampRateLineEdit->setAlignment(Qt::AlignRight);
   _rampRateLineEdit->setText(QString::number(_rampRate));
   _rampTemperatureButton = new QPushButton;
   _rampTemperatureButton->setText(QString("Ramp Off"));
   _rampTemperatureButton->setCheckable(true);

   //Populate the temperature layout
   _temperatureFrame = new QFrame;
   _temperatureFrame->setFrameShape(QFrame::HLine);
   _temperatureLayout = new QGridLayout;
   _temperatureLayout->addWidget(_setPointLabel, 0, 0);
   _temperatureLayout->addWidget(_setPointLineEdit, 0, 1);
   _temperatureLayout->addWidget(_setTemperatureButton, 0, 2);
   _temperatureLayout->addWidget(_rampRateLabel, 1, 0);
   _temperatureLayout->addWidget(_rampRateLineEdit, 1, 1);
   _temperatureLayout->addWidget(_rampTemperatureButton, 1, 2);

   //Setup the plot controls
   _xScaleButtonGroup = new QButtonGroup;
   _xScaleManualButton = new QRadioButton;
   _xScaleAutomaticButton = new QRadioButton;
   _xScaleManualButton->setText(QString("Manual"));
   _xScaleAutomaticButton->setChecked(true);
   _xScaleAutomaticButton->setText(QString("Auto"));
   _xScaleButtonGroup->addButton(_xScaleAutomaticButton);
   _xScaleButtonGroup->addButton(_xScaleManualButton);
   _xScaleMinLineEdit = new QLineEdit;
   _xScaleMaxLineEdit = new QLineEdit;
   _xScaleMinLineEdit->setValidator(new QDoubleValidator(0.0, 99.0, 2));
   _xScaleMaxLineEdit->setValidator(new QDoubleValidator(1.0, 100.0, 2));
   _xScaleMinLineEdit->setToolTip(QString("Valid entries 0.0 - 99.0"));
   _xScaleMaxLineEdit->setToolTip(QString("Valid entries 1.0 - 100.0"));
   _xScaleMinLineEdit->setMaxLength(6);
   _xScaleMaxLineEdit->setMaxLength(6);
   _xScaleMinLineEdit->setFixedWidth(50);
   _xScaleMaxLineEdit->setFixedWidth(50);
   _xScaleMinLineEdit->setAlignment(Qt::AlignRight);
   _xScaleMaxLineEdit->setAlignment(Qt::AlignRight);
   _xScaleMinLineEdit->setReadOnly(true);
   _xScaleMinLineEdit->setText(QString::number(_plotMinTemp));
   _xScaleMaxLineEdit->setReadOnly(true);
   _xScaleMaxLineEdit->setText(QString::number(_plotMaxTemp));
   _xScaleLabel = new QLabel;
   _xScaleMinLabel = new QLabel;
   _xScaleMaxLabel = new QLabel;
   _xScaleLabel->setText("Chart Limits");
   _xScaleMinLabel->setText(QString("Min temp"));
   _xScaleMaxLabel->setText(QString("Max temp"));
   _yScaleLabel = new QLabel;
   _yScaleLabel->setText("Chart Length (min)");
   _yScaleWindowLineEdit = new QLineEdit;
   _yScaleWindowLineEdit->setValidator(new QIntValidator(1, 90));
   _yScaleWindowLineEdit->setToolTip("Valid entries 1 - 90");
   _yScaleWindowLineEdit->setMaxLength(3);
   _yScaleWindowLineEdit->setFixedWidth(50);
   _yScaleWindowLineEdit->setAlignment(Qt::AlignRight);
   _yScaleWindowLineEdit->setReadOnly(true);
   _yScaleWindowLineEdit->setText(QString::number(_chartLength));

   //Populate the plot controls
   _plotScaleFrame = new QFrame;
   _plotScaleFrame->setFrameShape(QFrame::HLine);
   _plotScaleLayout = new QGridLayout;
   _plotScaleLayout->addWidget(_xScaleLabel, 0, 0);
   _plotScaleLayout->addWidget(_xScaleAutomaticButton, 0, 1);
   _plotScaleLayout->addWidget(_xScaleManualButton, 0, 2);
   _plotScaleLayout->addWidget(_xScaleMinLabel, 1, 1);
   _plotScaleLayout->addWidget(_xScaleMinLineEdit, 1, 2);
   _plotScaleLayout->addWidget(_xScaleMaxLabel, 2, 1);
   _plotScaleLayout->addWidget(_xScaleMaxLineEdit, 2, 2);
   _plotScaleLayout->addWidget(_yScaleLabel, 3, 1);
   _plotScaleLayout->addWidget(_yScaleWindowLineEdit, 3, 2);

   //Setup the refresh rate controls
   _refreshRateLabel = new QLabel;
   _refreshRateLabel->setText(QString("Refresh (Hz)"));
   _refreshRateLineEdit = new QLineEdit;
   _refreshRateLineEdit->setValidator(new QDoubleValidator(0.25, 10, 2));
   _refreshRateLineEdit->setToolTip(QString("Valid entries 0.25 - 10.0"));
   _refreshRateLineEdit->setMaxLength(6);
   _refreshRateLineEdit->setFixedWidth(50);
   _refreshRateLineEdit->setAlignment(Qt::AlignRight);
   _refreshRateLineEdit->setText(QString::number(_refreshRate));
   _refreshRateButton = new QPushButton;
   _refreshRateButton->setText(QString("Update"));

   //Populate the refresh controls
   _refreshRateFrame = new QFrame;
   _refreshRateFrame->setFrameShape(QFrame::HLine);
   _refreshRateLayout = new QHBoxLayout;
   _refreshRateLayout->addWidget(_refreshRateLabel);
   _refreshRateLayout->addWidget(_refreshRateLineEdit);
   _refreshRateLayout->addWidget(_refreshRateButton);

   //Setup the tuning parameter controls
   _loopTcLabel = new QLabel;
   _loopTcButtonGroup = new QButtonGroup;
   _loopTc1Button = new QRadioButton;
   _loopTc2Button = new QRadioButton;
   _loopTcLabel->setText(QString("Loop variable"));
   _loopTc1Button->setText(QString("TC1"));
   _loopTc2Button->setText(QString("TC2"));
   _loopTc1Button->setChecked(true);
   _tc1CalibrationLabel = new QLabel;
   _tc2CalibrationLabel = new QLabel;
   _tc1CalibrationLabel->setText(QString("TC1 calibration value"));
   _tc2CalibrationLabel->setText(QString("TC2 calibration value"));
   _tc1CalibrationLineEdit = new QLineEdit;
   _tc2CalibrationLineEdit = new QLineEdit;
   _tc1CalibrationLineEdit->setToolTip(QString("Calibration offset for thermocouple 1"));
   _tc2CalibrationLineEdit->setToolTip(QString("Calibration offset for thermocouple 2"));
   _tc1CalibrationLineEdit->setText(QString::number(_tc1Calibration));
   _tc2CalibrationLineEdit->setText(QString::number(_tc2Calibration));
   _tc1CalibrationLineEdit->setValidator(new QDoubleValidator(-10, 10, 2));
   _tc2CalibrationLineEdit->setValidator(new QDoubleValidator(-10, 10, 2));
   _tc1CalibrationLineEdit->setMaxLength(6);
   _tc2CalibrationLineEdit->setMaxLength(6);
   _tc1CalibrationLineEdit->setFixedWidth(50);
   _tc2CalibrationLineEdit->setFixedWidth(50);
   _tc1CalibrationLineEdit->setAlignment(Qt::AlignRight);
   _tc2CalibrationLineEdit->setAlignment(Qt::AlignRight);
   _kpLabel = new QLabel;
   _kiLabel = new QLabel;
   _kdLabel = new QLabel;
   _kpLabel->setText(QString("Kp"));
   _kiLabel->setText(QString("Ki"));
   _kdLabel->setText(QString("Kd"));
   _tuningResetButton = new QPushButton;
   _tuningUpdateButton = new QPushButton;
   _tuningResetButton->setText(QString("Reset PID"));
   _tuningUpdateButton->setText(QString("Update PID"));
   _kpLineEdit = new QLineEdit;
   _kiLineEdit = new QLineEdit;
   _kdLineEdit = new QLineEdit;
   _kpLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
   _kiLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
   _kdLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
   _kpLineEdit->setText(QString::number(_Kp));
   _kiLineEdit->setText(QString::number(_Ki));
   _kdLineEdit->setText(QString::number(_Kd));
   QDoubleValidator* tuningValidator = new QDoubleValidator(0, 400, 2);
   _kpLineEdit->setValidator(tuningValidator);
   _kiLineEdit->setValidator(tuningValidator);
   _kdLineEdit->setValidator(tuningValidator);
   _kpLineEdit->setMaxLength(6);
   _kiLineEdit->setMaxLength(6);
   _kdLineEdit->setMaxLength(6);
   _kpLineEdit->setFixedWidth(50);
   _kiLineEdit->setFixedWidth(50);
   _kdLineEdit->setFixedWidth(50);
   int editWidth = _kpLineEdit->width();
   _kpLineEdit->setAlignment(Qt::AlignRight);
   _kiLineEdit->setAlignment(Qt::AlignRight);
   _kdLineEdit->setAlignment(Qt::AlignRight);

   //Populate the tuning control layout
   _tuningFrame = new QFrame;
   _tuningFrame->setFrameShape(QFrame::HLine);
   _tuningLayout = new QGridLayout;
   _tuningLayout->addWidget(_loopTcLabel, 0, 0, Qt::AlignRight);
   _tuningLayout->addWidget(_loopTc1Button, 0, 1, Qt::AlignRight);
   _tuningLayout->addWidget(_loopTc2Button, 0, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_tc1CalibrationLabel, 1, 0, 1, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_tc1CalibrationLineEdit, 1, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_tc2CalibrationLabel, 2, 0, 1, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_tc2CalibrationLineEdit, 2, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_kpLabel, 3, 0, Qt::AlignRight);
   _tuningLayout->addWidget(_kiLabel, 3, 1, Qt::AlignRight);
   _tuningLayout->addWidget(_kdLabel, 3, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_kpLineEdit, 4, 0, Qt::AlignRight);
   _tuningLayout->addWidget(_kiLineEdit, 4, 1, Qt::AlignRight);
   _tuningLayout->addWidget(_kdLineEdit, 4, 2, Qt::AlignRight);
   _tuningButtonsLayout = new QHBoxLayout;
   _tuningButtonsLayout->addWidget(_tuningResetButton, Qt::AlignCenter);
   _tuningButtonsLayout->addWidget(_tuningUpdateButton, Qt::AlignCenter);

   //Populate the controls layout
   _controlsVerticalLayout->addLayout(_comLayout);
   _controlsVerticalLayout->addWidget(_temperatureFrame);
   _controlsVerticalLayout->addLayout(_temperatureLayout);
   _controlsVerticalLayout->addWidget(_plotScaleFrame);
   _controlsVerticalLayout->addLayout(_plotScaleLayout);
   _controlsVerticalLayout->addWidget(_refreshRateFrame);
   _controlsVerticalLayout->addLayout(_refreshRateLayout);
   _controlsVerticalLayout->addWidget(_tuningFrame);
   _controlsVerticalLayout->addLayout(_tuningLayout);
   _controlsVerticalLayout->addLayout(_tuningButtonsLayout);
   _controlsVerticalLayout->addStretch();

   //Setup the device status fields
   _deviceStatusLabel = new QLabel;
   _communicationStatusLabel = new QLabel;
   _tc1ReadingLabel = new QLabel;
   _tc2ReadingLabel = new QLabel;
   _tecCurrentReadingLabel = new QLabel;
   _deviceStatusLabel->setText(QString("Device Status:"));
   _communicationStatusLabel->setText(QString("Communication Status:"));
   _tc1ReadingLabel->setText(QString("Temp 1:"));
   _tc2ReadingLabel->setText(QString("Temp 2:"));
   _tecCurrentReadingLabel->setText(QString("TEC current:"));
   _deviceStatusLineEdit = new QLineEdit;
   _communicationStatusLineEdit = new QLineEdit;
   _tc1ReadingLineEdit = new QLineEdit;
   _tc2ReadingLineEdit = new QLineEdit;
   _tecCurrentReadingLineEdit = new QLineEdit;
   _deviceStatusLineEdit->setFixedWidth(80);
   _deviceStatusLineEdit->setText(QString("Disconnected"));
   _deviceStatusLineEdit->setReadOnly(true);
   _deviceStatusLineEdit->setAlignment(Qt::AlignRight);
   _communicationStatusLineEdit->setFixedWidth(80);
   _communicationStatusLineEdit->setText(QString("OK"));
   _communicationStatusLineEdit->setReadOnly(true);
   _communicationStatusLineEdit->setAlignment(Qt::AlignRight);
   _tc1ReadingLineEdit->setFixedWidth(50);
   _tc1ReadingLineEdit->setText(QString("-.-"));
   _tc1ReadingLineEdit->setReadOnly(true);
   _tc1ReadingLineEdit->setAlignment(Qt::AlignRight);
   _tc2ReadingLineEdit->setFixedWidth(50);
   _tc2ReadingLineEdit->setText(QString("-.-"));
   _tc2ReadingLineEdit->setReadOnly(true);
   _tc2ReadingLineEdit->setAlignment(Qt::AlignRight);
   _tecCurrentReadingLineEdit->setFixedWidth(50);
   _tecCurrentReadingLineEdit->setText(QString("-.-"));
   _tecCurrentReadingLineEdit->setReadOnly(true);
   _tecCurrentReadingLineEdit->setAlignment(Qt::AlignRight);

   //Populate the device status layout
   _deviceStatusLayout = new QGridLayout;
   _deviceStatusLayout->addWidget(_deviceStatusLabel, 0, 0);
   _deviceStatusLayout->addWidget(_deviceStatusLineEdit, 0, 1, 1, 2);
   _deviceStatusLayout->addWidget(_communicationStatusLabel, 0, 3);
   _deviceStatusLayout->addWidget(_communicationStatusLineEdit, 0, 4, 1, 2);
   _deviceStatusLayout->addWidget(_tecCurrentReadingLabel, 1, 0);
   _deviceStatusLayout->addWidget(_tecCurrentReadingLineEdit, 1, 1);
   _deviceStatusLayout->addWidget(_tc1ReadingLabel, 1, 2);
   _deviceStatusLayout->addWidget(_tc1ReadingLineEdit, 1, 3);
   _deviceStatusLayout->addWidget(_tc2ReadingLabel, 1, 4);
   _deviceStatusLayout->addWidget(_tc2ReadingLineEdit, 1, 5);
   _deviceStatusHBoxLayout = new QHBoxLayout;
   _deviceStatusHBoxLayout->addLayout(_deviceStatusLayout);
   _deviceStatusHBoxLayout->addStretch();
   _deviceStatusHBoxLayout->setStretch(1, 1);

   //Setup the plot
   _temperaturePlot = new QChart;
   _temperaturePlotView = new QChartView(_temperaturePlot);
   _temperaturePlot->setMinimumWidth(600);
   _xAxisTime = new QValueAxis;
   _yAxisTemperature = new QValueAxis;
   _yAxisCurrent = new QValueAxis;
   _xAxisTime->setRange(0, 1);
   _xAxisTime->setLabelFormat("%.1f");
   _xAxisTime->setTitleText("Time (min)");
   _yAxisTemperature->setRange(0, 100);
   _yAxisTemperature->setLabelFormat("%.2f");
   _yAxisTemperature->setTitleText("Temperature (C)");
   _yAxisCurrent->setRange(-6, 6);
   _yAxisCurrent->setLabelFormat("%i");
   _yAxisCurrent->setTitleText("TEC current (A)");
   _temperaturePlot->addAxis(_yAxisTemperature, Qt::AlignLeft);
   _temperaturePlot->addAxis(_yAxisCurrent, Qt::AlignRight);
   _temperaturePlot->addAxis(_xAxisTime, Qt::AlignBottom);
   _tc1DataSeries = new QLineSeries;
   _tc2DataSeries = new QLineSeries;
   _tecCurrentDataSeries = new QLineSeries;
   _tc1DataSeries->setName(QString("TC1"));
   _tc2DataSeries->setName(QString("TC2"));
   _tecCurrentDataSeries->setName(QString("Current"));
   _temperaturePlot->addSeries(_tc1DataSeries);
   _temperaturePlot->addSeries(_tc2DataSeries);
   _temperaturePlot->addSeries(_tecCurrentDataSeries);
   _tc1DataSeries->attachAxis(_xAxisTime);
   _tc1DataSeries->attachAxis(_yAxisTemperature);
   _tc2DataSeries->attachAxis(_xAxisTime);
   _tc2DataSeries->attachAxis(_yAxisTemperature);
   _tecCurrentDataSeries->attachAxis(_xAxisTime);
   _tecCurrentDataSeries->attachAxis(_yAxisCurrent);

   //Populate the data layout
   _dataVerticalLayout->addLayout(_deviceStatusHBoxLayout);
   _dataVerticalLayout->addWidget(_temperaturePlotView);

   //Finalize the central layout
   _parentLayout->addLayout(_controlsVerticalLayout);
   _parentLayout->addLayout(_dataVerticalLayout);
   _parentLayout->setStretch(1, 1);
   _centralWidget->setLayout(_parentLayout);
   setCentralWidget(_centralWidget);
   return;
}

void sccp::SetupSerial()
{
   _serialPortList = QSerialPortInfo::availablePorts();
   _serialPort = new QSerialPort;
   for (QSerialPortInfo &info : _serialPortList)
   {
      _comPortComboBox->addItem(info.portName());
   }
   return;
}

void sccp::SetupConnections()
{
   connect(_comScanButton, SIGNAL(clicked()), SLOT(on_scan_button_clicked()));
   connect(_comConnectButton, SIGNAL(clicked()), SLOT(on_connect_button_clicked()));
   connect(_comPortComboBox, SIGNAL(currentIndexChanged()), SLOT(on_comPortComboBox_selection_changed()));
   return;
}

void sccp::on_scan_button_clicked()
{
   _comPortComboBox->clear();
   _serialPortList = QSerialPortInfo::availablePorts();
   for (QSerialPortInfo &info : _serialPortList)
      _comPortComboBox->addItem(info.portName());
   return;
}

void sccp::on_connect_button_clicked()
{
   if (!_comConnectButton->isChecked())
   {
      _serialPort->close();
      _comConnectButton->setText(QString("Connect"));
      _deviceStatusLineEdit->setText(QString("Disconnected"));
      _comScanButton->setEnabled(true);
      _comPortComboBox->setEnabled(true);
      on_scan_button_clicked();
      return;
   }
   if (_comPortComboBox->count() == 0)
      return;
   else
   {
      _serialPort->setPortName(_comPortComboBox->currentText());
      _serialPort->setBaudRate(QSerialPort::Baud9600);
      _serialPort->setDataBits(QSerialPort::Data8);
      _serialPort->setParity(QSerialPort::NoParity);
      _serialPort->setStopBits(QSerialPort::OneStop);
      _serialPort->setFlowControl(QSerialPort::NoFlowControl);
      if (_serialPort->open(QIODevice::ReadWrite))
      {
         _comConnectButton->setText(QString("Disconnect"));
         _comScanButton->setEnabled(false);
         _comPortComboBox->setEnabled(false);
         _deviceStatusLineEdit->setText(QString("Connected"));
         GetSensorValues();
      }
   }
   return;
}

int sccp::SerialTransfer()
{
   if (_serialPort->isOpen() && _serialPort->isWritable())
   {
      _serialBuffer[0] = 0x07;
      int bytesWritten = _serialPort->write(_serialBuffer, 25);
      _serialPort->flush();
      _serialPort->waitForReadyRead(3000);
      int readCount = 0;
      readCount = _serialPort->readLine(_serialBuffer, 26);
      while (readCount < 25)
      {
         readCount += _serialPort->readLine(_serialBuffer, 26);
      }
      if (_serialBuffer[0] != 0x00)
      {
         _communicationStatusLabel->setText(QString("Error"));
         return 1;
      }
      else
         _communicationStatusLabel->setText(QString("OK"));
   }
   return 0;
}

int sccp::GetSensorValues()
{
   _serialBuffer[0] = 0x07;
   if (SerialTransfer())
      return 1;
   double* values = (double*)(_serialBuffer + 1);
   _tc1Temperature = *values * 0.25;
   _tc2Temperature = *(values + 1) * 0.25;
   _tecCurrent = (*(values + 2) - 2047) * 0.006105006105006105006105006105;
   _tc1ReadingLineEdit->setText(QString::number(_tc1Temperature));
   _tc2ReadingLineEdit->setText(QString::number(_tc2Temperature));
   _tecCurrentReadingLineEdit->setText(QString::number(_tecCurrent));
   return 0;
}

int sccp::GetPIDConstants()
{
   return 0;
}