#include "sccp.h"
#include <iostream>

sccp::sccp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    SetupGUI();
    SetupCom();
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
   _comConnectButton->setEnabled(false);
   _comPortComboBox = new QComboBox;
   _comPortComboBox->setEditable(false);
   _comPortComboBox->addItem(QString("none"));
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
   _xScaleMinLineEdit->setText(QString("0.00"));
   _xScaleMaxLineEdit->setReadOnly(true);
   _xScaleMaxLineEdit->setText(QString("100.00"));
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
   _yScaleWindowLineEdit->setText(QString("60"));

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
   _kpLabel = new QLabel;
   _kiLabel = new QLabel;
   _kdLabel = new QLabel;
   _kpLabel->setText(QString("Kp)"));
   _kiLabel->setText(QString("Ki"));
   _kdLabel->setText(QString("Kd"));
   _tuningResetButton = new QPushButton;
   _tuningUpdateButton = new QPushButton;
   _tuningResetButton->setText(QString("Reset"));
   _tuningUpdateButton->setText(QString("Update"));
   _kpLineEdit = new QLineEdit;
   _kiLineEdit = new QLineEdit;
   _kdLineEdit = new QLineEdit;
   _kpLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
   _kiLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
   _kdLineEdit->setToolTip(QString("Valid entries 0.0 - 400.0"));
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
   _tuningLayout->addWidget(_kpLabel, 0, 0, Qt::AlignRight);
   _tuningLayout->addWidget(_kiLabel, 0, 1, Qt::AlignRight);
   _tuningLayout->addWidget(_kdLabel, 0, 2, Qt::AlignRight);
   _tuningLayout->addWidget(_kpLineEdit, 1, 0, Qt::AlignRight);
   _tuningLayout->addWidget(_kiLineEdit, 1, 1, Qt::AlignRight);
   _tuningLayout->addWidget(_kdLineEdit, 1, 2, Qt::AlignRight);
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

   _statusLabel = new QLabel;
   _statusLabel->setText(QString("Device Status"));
   _dataVerticalLayout->addWidget(_statusLabel);

   _parentLayout->addLayout(_controlsVerticalLayout);
   _parentLayout->addLayout(_dataVerticalLayout);
   _parentLayout->setStretch(1, 1);

   _centralWidget->setLayout(_parentLayout);
   setCentralWidget(_centralWidget);
   return;
}

void sccp::SetupCom()
{
   connect(_comScanButton, SIGNAL(clicked()), SLOT(on_scan_button_clicked()));
   return;
}

void sccp::on_scan_button_clicked()
{
   return;
}