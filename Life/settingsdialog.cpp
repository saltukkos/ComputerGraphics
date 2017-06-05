#include "settingsdialog.h"

#include <numeric>
#include <string>
#include "constants.h"
#include "utils.h"

SettingsDialog::SettingsDialog(ViewSettings *viewSettings, ModelSettings *modelSettings, QWidget *parent) :
    QDialog(parent),
    viewSettings(viewSettings),
    modelSettings(modelSettings)
{
    QVBoxLayout *mouseModeLayout = new QVBoxLayout();
    buttonFill = new QRadioButton("Filling", this);
    buttonXOR = new QRadioButton("XOR", this);

    mouseModeLayout->addWidget(buttonFill);
    mouseModeLayout->addWidget(buttonXOR);

    QGridLayout *fieldSizeLayout = new QGridLayout();
    width = new QSpinBox(this);
    height = new QSpinBox(this);
    width->setRange(Constants::MIN_FIELD_WIDTH, Constants::MAX_FIELD_WIDTH);
    height->setRange(Constants::MIN_FIELD_HEIGHT, Constants::MAX_FIELD_HEIGHT);

    fieldSizeLayout->addWidget(new QLabel("Width", this), 0, 0);
    fieldSizeLayout->addWidget(new QLabel("Height", this), 1, 0);
    fieldSizeLayout->addWidget(width, 0, 1);
    fieldSizeLayout->addWidget(height, 1, 1);

    QGridLayout *propLayout = new QGridLayout();

    QSlider *cellSizeSlider = new QSlider(Qt::Horizontal, this);
    QSlider *timeoutSlider = new QSlider(Qt::Horizontal, this);
    cellSizeLine = new QSpinBox(this);
    timeoutLine = new QSpinBox(this);
    cellSizeSlider->setRange(Constants::MIN_CELL_SIZE, Constants::MAX_CELL_SIZE);
    timeoutSlider->setRange(0, Constants::MAX_TIMER_PERIOD);
    cellSizeLine->setRange(Constants::MIN_CELL_SIZE, Constants::MAX_CELL_SIZE);
    timeoutLine->setRange(0, Constants::MAX_TIMER_PERIOD);

    propLayout->addWidget(new QLabel("Cell size", this), 0, 0);
    propLayout->addWidget(new QLabel("Timer period", this), 1, 0);
    propLayout->addWidget(cellSizeLine, 0, 1);
    propLayout->addWidget(timeoutLine, 1, 1);
    propLayout->addWidget(cellSizeSlider, 0, 2);
    propLayout->addWidget(timeoutSlider, 1, 2);

    QGridLayout *impactLayout = new QGridLayout(this);
    firstImpact = new QLineEdit(this);
    secondImpact = new QLineEdit(this);
    liveBegin = new QLineEdit(this);
    liveEnd = new QLineEdit(this);
    birthBegin = new QLineEdit(this);
    birthEnd = new QLineEdit(this);

    QDoubleValidator *validator = new QDoubleValidator(
                std::numeric_limits<int>::min()/Constants::IMPACT_MULT,
                std::numeric_limits<int>::max()/Constants::IMPACT_MULT,
                Utils::digits(Constants::IMPACT_MULT) - 1, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    firstImpact->setValidator(validator);
    secondImpact->setValidator(validator);
    liveBegin->setValidator(validator);
    liveEnd->setValidator(validator);
    birthBegin->setValidator(validator);
    birthEnd->setValidator(validator);

    impactLayout->addWidget(new QLabel("FIRST_IMPACT", this), 0, 0);
    impactLayout->addWidget(new QLabel("SECOND_IMPACT", this), 0, 1);
    impactLayout->addWidget(new QLabel("LIVE_BEGIN", this), 0, 2);
    impactLayout->addWidget(new QLabel("LIVE_END", this), 0, 3);
    impactLayout->addWidget(new QLabel("BIRTH_BEGIN", this), 0, 4);
    impactLayout->addWidget(new QLabel("BIRTH_END", this), 0, 5);
    impactLayout->addWidget(firstImpact, 1, 0);
    impactLayout->addWidget(secondImpact, 1, 1);
    impactLayout->addWidget(liveBegin, 1, 2);
    impactLayout->addWidget(liveEnd, 1, 3);
    impactLayout->addWidget(birthBegin, 1, 4);
    impactLayout->addWidget(birthEnd, 1, 5);

    QGroupBox *mouseMode = new QGroupBox("Editing mode", this);
    QGroupBox *fieldSize = new QGroupBox("Simulation field size", this);
    QGroupBox *propEdit = new QGroupBox("Simulator settings", this);
    QGroupBox *impactEdit = new QGroupBox("Game rules", this);

    mouseMode->setLayout(mouseModeLayout);
    fieldSize->setLayout(fieldSizeLayout);
    propEdit->setLayout(propLayout);
    impactEdit->setLayout(impactLayout);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(checkInput()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout *mainLayout = new QGridLayout();

    mainLayout->addWidget(mouseMode, 0, 0);
    mainLayout->addWidget(fieldSize, 0, 1);
    mainLayout->addWidget(propEdit, 0, 2);
    mainLayout->addWidget(impactEdit, 1, 0, 1, 3);
    mainLayout->addWidget(buttons, 2, 0, 1, 3);

    setLayout(mainLayout);
    setWindowTitle("Settings");

    connect(cellSizeLine, SIGNAL(valueChanged(int)), cellSizeSlider, SLOT(setValue(int)));
    connect(cellSizeSlider, SIGNAL(sliderMoved(int)), cellSizeLine, SLOT(setValue(int)));
    connect(timeoutLine, SIGNAL(valueChanged(int)), timeoutSlider, SLOT(setValue(int)));
    connect(timeoutSlider, SIGNAL(sliderMoved(int)), timeoutLine, SLOT(setValue(int)));

    buttonXOR->setChecked(viewSettings->modeXor);
    buttonFill->setChecked(!viewSettings->modeXor);
    width->setValue(modelSettings->width);
    height->setValue(modelSettings->height);
    cellSizeLine->setValue(viewSettings->cellSize);
    timeoutLine->setValue(modelSettings->timerPeriod);

    QLocale locale;
    QChar point = locale.decimalPoint(); // I have no idea why QDoubleValidator works only with current locale point when QString::number only with '.'

    firstImpact->setText(QString::number((double)modelSettings->firstImpact/Constants::IMPACT_MULT).replace(".", point));
    secondImpact->setText(QString::number((double)modelSettings->secondImpact/Constants::IMPACT_MULT).replace(".", point));
    liveBegin->setText(QString::number((double)modelSettings->liveBegin/Constants::IMPACT_MULT).replace(".", point));
    liveEnd->setText(QString::number((double)modelSettings->liveEnd/Constants::IMPACT_MULT).replace(".", point));
    birthBegin->setText(QString::number((double)modelSettings->birthBegin/Constants::IMPACT_MULT).replace(".", point));
    birthEnd->setText(QString::number((double)modelSettings->birthEnd/Constants::IMPACT_MULT).replace(".", point));

}

void SettingsDialog::checkInput()
{
    simLineEdit(firstImpact->text(), &modelSettings->firstImpact);
    simLineEdit(secondImpact->text(), &modelSettings->secondImpact);
    simLineEdit(liveBegin->text(), &modelSettings->liveBegin);
    simLineEdit(liveEnd->text(), &modelSettings->liveEnd);
    simLineEdit(birthBegin->text(), &modelSettings->birthBegin);
    simLineEdit(birthEnd->text(), &modelSettings->birthEnd);

    if (modelSettings->liveBegin > modelSettings->birthBegin ||
            modelSettings->birthBegin > modelSettings->birthEnd ||
            modelSettings->birthEnd > modelSettings->liveEnd){
        QMessageBox message;
        message.setText("Rule \"LIVE_BEGIN ≤ BIRTH_BEGIN ≤ BIRTH_END ≤ LIVE_END\" should be observed");
        message.setIcon(QMessageBox::Warning);
        message.exec();
        return;
    }

    viewSettings->modeXor = buttonXOR->isChecked();
    viewSettings->cellSize = cellSizeLine->value();
    modelSettings->width = width->value();
    modelSettings->height = height->value();
    modelSettings->timerPeriod = timeoutLine->value();

    accept();
}



void SettingsDialog::simLineEdit(QString string, int *res)
{
    double temp = std::stod(string.toStdString());
    *res = qRound(temp * Constants::IMPACT_MULT);
}
