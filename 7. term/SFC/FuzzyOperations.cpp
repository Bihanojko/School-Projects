/* Author: Nikola Valesova
 * Date: 15. 10. 2017
 * Subject: Soft Computing
 * File: FuzzyOperations.cpp
 * Faculty of Information Technology
 * Brno University of Technology
*/


#include "FuzzyOperations.h"


// class contructor
FuzzyOperations::FuzzyOperations(QWidget *parent) :
    QWidget(parent),
    m_SetAComboBox(CreateSetTypesA()),
    m_SetBComboBox(CreateSetTypesB()),
    ALineEditA(CreateLineEdit("2", "a_line_edit1")),
    ALineEditB(CreateLineEdit("4", "a_line_edit2")),
    ALineEditC(CreateLineEdit("6", "a_line_edit3")),
    ALineEditD(CreateLineEdit("8", "a_line_edit4")),
    BLineEditA(CreateLineEdit("2", "b_line_edit1")),
    BLineEditB(CreateLineEdit("4", "b_line_edit2")),
    BLineEditC(CreateLineEdit("6", "b_line_edit3")),
    BLineEditD(CreateLineEdit("8", "b_line_edit4")),
    ALabelA(CreateLabel("a:", "AlabelA")),
    ALabelB(CreateLabel("b:", "AlabelB")),
    ALabelC(CreateLabel("c:", "AlabelC")),
    ALabelD(CreateLabel("d:", "AlabelD")),
    BLabelA(CreateLabel("a:", "BlabelA")),
    BLabelB(CreateLabel("b:", "BlabelB")),
    BLabelC(CreateLabel("c:", "BlabelC")),
    BLabelD(CreateLabel("d:", "BlabelD"))
{
    ConnectSlotsToSignals();

    // create main layout
    QGridLayout *baseLayout = new QGridLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Type of set A:"));
    settingsLayout->addWidget(m_SetAComboBox);
    settingsLayout->addWidget(new QLabel("Type of set B:"));
    settingsLayout->addWidget(m_SetBComboBox);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout, 0, 0, 1, 2);
    baseLayout->addLayout(CreateParameterSettingsLayout("A"), 1, 0, 1, 2);
    baseLayout->addLayout(CreateParameterSettingsLayout("B"), 2, 0, 1, 2);
    baseLayout->setRowStretch(1, 30);

    std::vector<Point> APoints = GetInputPoints(AMembershipFunctionType, "A");
    std::vector<Point> BPoints = GetInputPoints(BMembershipFunctionType, "B");

    std::vector<QVector<double>> PointVector;
    QCustomPlot *customPlot;

    customPlot = CreateCustomPlot(2);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Input Fuzzy Sets A and B", QFont("sans", 11)));
    PointVector = PointsToSeries(APoints);
    customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(0)->setName("Set A");
    PointVector = PointsToSeries(BPoints);
    customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(1)->setName("Set B");
    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(-0.02, 1.02);
    customPlot->replot();
    customPlot->setObjectName("A");
    baseLayout->addWidget(customPlot, 3, 0);
    m_customPlot << customPlot;
    baseLayout->layout()->setSpacing(15);

    customPlot = CreateCustomPlot(3);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "A Normalization, Delation and Concentration", QFont("sans", 11)));
    PointVector = PointsToSeries(ComputeDilation(APoints));
    customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(0)->setName("A Dilatation");
    PointVector = PointsToSeries(ComputeNormalization(APoints));
    customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(1)->setName("A Normalization");
    PointVector = PointsToSeries(ComputeConcentration(APoints));
    customPlot->graph(2)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(2)->setPen(QPen(QColor(105, 105, 105)));
    customPlot->graph(2)->setName("A Concentration");
    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(-0.02, 1.02);
    customPlot->replot();
    customPlot->setObjectName("RES2");
    baseLayout->addWidget(customPlot, 4, 1);
    m_customPlot << customPlot;

    customPlot = CreateCustomPlot(2);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Union and Intersection of Sets A and B", QFont("sans", 11)));
    PointVector = PointsToSeries(ComputeUnion(APoints, BPoints));
    customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(0)->setName("A Union B");
    PointVector = PointsToSeries(ComputeIntersect(APoints, BPoints));
    customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(1)->setName("A Intersection B");
    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(-0.02, 1.02);
    customPlot->replot();
    customPlot->setObjectName("RES1");
    baseLayout->addWidget(customPlot, 3, 1);
    m_customPlot << customPlot;

    customPlot = CreateCustomPlot(2);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "A Complement and Support", QFont("sans", 11)));
    PointVector = PointsToSeries(ComputeComplement(APoints));
    customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(0)->setName("A Complement");
    PointVector = PointsToSeries(ComputeSupport(APoints));
    customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
    customPlot->graph(1)->setName("A Support");
    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(-0.02, 1.02);
    customPlot->replot();
    customPlot->setObjectName("RES3");
    baseLayout->addWidget(customPlot, 4, 0);
    m_customPlot << customPlot;

    setLayout(baseLayout);
    updateUI();
}



QCustomPlot *FuzzyOperations::CreateCustomPlot(int SeriesCount) const
{
    QCustomPlot *customPlot = new QCustomPlot();
    customPlot->plotLayout()->insertRow(0);
    customPlot->legend->setVisible(true);

    for(int i = 0; i < SeriesCount; i++)
        customPlot->addGraph();

    customPlot->graph(0)->setPen(QPen(QColor(0, 179, 179)));
    customPlot->graph(1)->setPen(QPen(QColor(255, 0, 0)));
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("m(x)");

    return customPlot;
}



// class destructor, memory deallocation
FuzzyOperations::~FuzzyOperations()
{
    for(QCustomPlot *customPlot: m_customPlot)
        delete customPlot;

    delete m_SetAComboBox;
    delete m_SetBComboBox;

    delete ALineEditA;
    delete ALineEditB;
    delete ALineEditC;
    delete ALineEditD;

    delete BLineEditA;
    delete BLineEditB;
    delete BLineEditC;
    delete BLineEditD;

    delete ALabelA;
    delete ALabelB;
    delete ALabelC;
    delete ALabelD;
    delete BLabelA;
    delete BLabelB;
    delete BLabelC;
    delete BLabelD;
}


// combo box for selecting type of membership function A
QComboBox *FuzzyOperations::CreateSetTypesA() const
{
    QComboBox *SetAComboBox = new QComboBox();

    SetAComboBox->addItem("Triangle");
    SetAComboBox->addItem("Trapezoidal");
    SetAComboBox->addItem("Gaussian");
    SetAComboBox->addItem("Bell");
    SetAComboBox->addItem("Sigmoidal");

    return SetAComboBox;
}


// combo box for selecting type of membership function B
QComboBox *FuzzyOperations::CreateSetTypesB() const
{
    QComboBox *SetBComboBox = new QComboBox();

    SetBComboBox->addItem("Triangle");
    SetBComboBox->addItem("Trapezoidal");
    SetBComboBox->addItem("Gaussian");
    SetBComboBox->addItem("Bell");
    SetBComboBox->addItem("Sigmoidal");

    return SetBComboBox;
}


// connection of signals to appropriate slots
void FuzzyOperations::ConnectSlotsToSignals()
{
    // combo boxes for selecting types of membership functions
    connect(m_SetAComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &FuzzyOperations::updateUI);
    connect(m_SetBComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &FuzzyOperations::updateUI);

    // line edit fields for inserting parameters of membership function
    connect(ALineEditA,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(ALineEditB,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(ALineEditC,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(ALineEditD,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(BLineEditA,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(BLineEditB,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(BLineEditC,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
    connect(BLineEditD,
            SIGNAL(editingFinished()),
            this, SLOT(updateUI()));
}


// according to chosen type of set SetType, get points of input fuzzy set named SetName
std::vector<Point> FuzzyOperations::GetInputPoints(int SetType, std::string SetName)
{
    std::vector<Point> InputPoints;
    double a, b, c, d;

    if(SetName == "A")
    {
        a = A_a;
        b = A_b;
        c = A_c;
        d = A_d;
    }
    else
    {
        a = B_a;
        b = B_b;
        c = B_c;
        d = B_d;
    }

    switch (SetType) {
    // Triangle
    case 0:
        InputPoints = ComputeTrianglePoints(a, b, c);
        break;

    // Trapezoidal
    case 1:
        InputPoints = ComputeTrapezoidalPoints(a, b, c, d);
        break;

    // Gaussian
    case 2:
        InputPoints = ComputeGaussianPoints(a, b);
        break;

    // Bell
    case 3:
        InputPoints = ComputeBellPoints(a, b, c);
        break;

    // Sigmoidal
    case 4:
        InputPoints = ComputeSigmoidalPoints(a, b);
        break;

    default:
        break;
    }

    return InputPoints;
}


// convert point vector to vector of two QVectors of doubles for plotting
std::vector<QVector<double>> FuzzyOperations::PointsToSeries(std::vector<Point> InputPoints) const
{
    std::vector<QVector<double>> series;
    QVector<double> SeriesX, SeriesY;

    for(unsigned int x = 0; x < InputPoints.size(); x++)
    {
        SeriesX.push_back(std::get<0>(InputPoints[x]) / 100.0);
        SeriesY.push_back(std::get<1>(InputPoints[x]));
    }

    series.push_back(SeriesX);
    series.push_back(SeriesY);

    return series;
}


// create a layout for input set parameters and fill it with appropriate widgets
QHBoxLayout *FuzzyOperations::CreateParameterSettingsLayout(std::string SetName) const
{
    QHBoxLayout *ParametersLayout = new QHBoxLayout();

    QLabel *label = new QLabel(("Parameters of set " + SetName + " membership function: ").c_str());
    ParametersLayout->addWidget(label);
    ParametersLayout->addSpacing(15);

    QHBoxLayout *InnerLayout = new QHBoxLayout();
    InnerLayout->addWidget((SetName == "A") ? ALabelA : BLabelA);
    InnerLayout->addWidget((SetName == "A") ? ALineEditA : BLineEditA);
    InnerLayout->addSpacing(20);

    QHBoxLayout *InnerLayout2 = new QHBoxLayout();
    InnerLayout2->addWidget((SetName == "A") ? ALabelB : BLabelB);
    InnerLayout2->addWidget((SetName == "A") ? ALineEditB : BLineEditB);
    InnerLayout2->addSpacing(20);

    QHBoxLayout *InnerLayout3 = new QHBoxLayout();
    InnerLayout3->addWidget((SetName == "A") ? ALabelC : BLabelC);
    InnerLayout3->addWidget((SetName == "A") ? ALineEditC : BLineEditC);
    InnerLayout3->addSpacing(20);

    QHBoxLayout *InnerLayout4 = new QHBoxLayout();
    (SetName == "A") ? ALabelD->setVisible(false) : BLabelD->setVisible(false);
    InnerLayout4->addWidget((SetName == "A") ? ALabelD : BLabelD);
    if(SetName == "A")
    {
        ALineEditD->setVisible(false);
        InnerLayout4->addWidget(ALineEditD);
    }
    else
    {
        BLineEditD->setVisible(false);
        InnerLayout4->addWidget(BLineEditD);
    }

    InnerLayout3->addLayout(InnerLayout4);
    InnerLayout2->addLayout(InnerLayout3);
    InnerLayout->addLayout(InnerLayout2);
    ParametersLayout->addLayout(InnerLayout);
    ParametersLayout->addStretch();

    return ParametersLayout;
}


// create new QLabel and set its text to content and its name to name
QLabel *FuzzyOperations::CreateLabel(std::string content, std::string name) const
{
    QLabel *label = new QLabel(content.c_str());
    label->setAccessibleName(name.c_str());
    label->setMaximumWidth(10);

    return label;
}


// create new QLineEdit, set its content to content and its name to name
QLineEdit *FuzzyOperations::CreateLineEdit(std::string content, std::string name) const
{
    QLineEdit *line_edit = new QLineEdit;
    line_edit->setText(content.c_str());
    line_edit->setAccessibleName(name.c_str());
    line_edit->setMaximumWidth(45);

    return line_edit;
}


// according to input set type, make visible appropriate number of input fields
void FuzzyOperations::SetParameterVisibility() const
{
    // for set A
    ALineEditC->setVisible((AMembershipFunctionType == 2 || AMembershipFunctionType == 4) ? false : true);
    ALineEditD->setVisible((AMembershipFunctionType == 1) ? true : false);
    ALabelC->setVisible((AMembershipFunctionType == 2 || AMembershipFunctionType == 4) ? false : true);
    ALabelD->setVisible((AMembershipFunctionType == 1) ? true : false);

    // for set B
    BLineEditC->setVisible((BMembershipFunctionType == 2 || BMembershipFunctionType == 4) ? false : true);
    BLineEditD->setVisible((BMembershipFunctionType == 1) ? true : false);
    BLabelC->setVisible((BMembershipFunctionType == 2 || BMembershipFunctionType == 4) ? false : true);
    BLabelD->setVisible((BMembershipFunctionType == 1) ? true : false);
}


// check if LineEdit contains a valid double value, if not, set its frame color to red
bool FuzzyOperations::IsDouble(QLineEdit *LineEdit) const
{
    char *endptr = 0;
    strtod(LineEdit->text().toLatin1(), &endptr);

    if(*endptr != '\0' || endptr == LineEdit->text())
    {
        LineEdit->setStyleSheet("border: 1px solid red");
        return false;
    }

    return true;
}


// reset new input values
void FuzzyOperations::ActualizeInputValues()
{
    A_a = ALineEditA->text().toDouble();
    A_b = ALineEditB->text().toDouble();
    A_c = ALineEditC->text().toDouble();
    A_d = ALineEditD->text().toDouble();
    B_a = BLineEditA->text().toDouble();
    B_b = BLineEditB->text().toDouble();
    B_c = BLineEditC->text().toDouble();
    B_d = BLineEditD->text().toDouble();
}


// check input parameter values if they are logically correct
bool FuzzyOperations::CheckInputParameterValues()
{
    bool ValuesCorrect = true;

    if(!(IsDouble(ALineEditA) && IsDouble(ALineEditB) && IsDouble(ALineEditC) && IsDouble(ALineEditD) && \
          IsDouble(BLineEditA) && IsDouble(BLineEditB) && IsDouble(BLineEditC) && IsDouble(BLineEditD)))
        ValuesCorrect = false;

    if(ValuesCorrect)
        ActualizeInputValues();

    if(AMembershipFunctionType < 2)
    {
        if(A_a >= A_b)
        {
            ALineEditA->setStyleSheet("border: 1px solid red");
            ValuesCorrect = false;
        }
        if(A_b >= A_c)
        {
            ALineEditB->setStyleSheet("border: 1px solid red");
            ValuesCorrect = false;
        }
    }

    if(AMembershipFunctionType == 1 && A_c >= A_d)
    {
        ALineEditC->setStyleSheet("border: 1px solid red");
        ValuesCorrect = false;
    }

    if(AMembershipFunctionType == 3 && A_a == 0.0)
    {
        ALineEditA->setStyleSheet("border: 1px solid red");
        ValuesCorrect = false;
    }

    if(BMembershipFunctionType < 2)
    {
        if(B_a >= B_b)
        {
            BLineEditA->setStyleSheet("border: 1px solid red");
            ValuesCorrect = false;
        }
        if(B_b >= B_c)
        {
            BLineEditB->setStyleSheet("border: 1px solid red");
            ValuesCorrect = false;
        }
    }

    if(BMembershipFunctionType == 1 && B_c >= B_d)
    {
        BLineEditC->setStyleSheet("border: 1px solid red");
        ValuesCorrect = false;
    }

    if(BMembershipFunctionType == 3 && B_a == 0.0)
    {
        BLineEditA->setStyleSheet("border: 1px solid red");
        ValuesCorrect = false;
    }

    return ValuesCorrect;
}


// sets the text of labels to appropriate values according to chosen set type
void FuzzyOperations::SetLabelContent() const
{
    ALabelA->setText(AMembershipFunctionType == 2 ? "c:" : "a:");
    ALabelB->setText(AMembershipFunctionType == 2 ? "s:" : (AMembershipFunctionType == 4 ? "c:" : "b:"));
    BLabelA->setText(BMembershipFunctionType == 2 ? "c:" : "a:");
    BLabelB->setText(BMembershipFunctionType == 2 ? "s:" : (BMembershipFunctionType == 4 ? "c:" : "b:"));
}


// indicate valid input values by setting all frame colors to black
void FuzzyOperations::SetBorderColors() const
{
    ALineEditA->setStyleSheet("border: 1px solid black");
    ALineEditB->setStyleSheet("border: 1px solid black");
    ALineEditC->setStyleSheet("border: 1px solid black");
    ALineEditD->setStyleSheet("border: 1px solid black");
    BLineEditA->setStyleSheet("border: 1px solid black");
    BLineEditB->setStyleSheet("border: 1px solid black");
    BLineEditC->setStyleSheet("border: 1px solid black");
    BLineEditD->setStyleSheet("border: 1px solid black");
}


// actualize graphs according to member function types and parameters
void FuzzyOperations::updateUI()
{
    std::vector<QVector<double>> PointVector;

    // reset x-axis limits
    LowerLimit = 1000;
    HigherLimit = -1000;

    // get input values
    AMembershipFunctionType = m_SetAComboBox->currentIndex();
    BMembershipFunctionType = m_SetBComboBox->currentIndex();

    // check input values
    SetParameterVisibility();
    if(!CheckInputParameterValues())
        return;
    SetBorderColors();
    SetLabelContent();

    // regenerate points of input sets
    std::vector<Point> APoints = GetInputPoints(AMembershipFunctionType, "A");
    std::vector<Point> BPoints = GetInputPoints(BMembershipFunctionType, "B");
    APoints = GetInputPoints(AMembershipFunctionType, "A");

    // recreate chart contents
    for(QCustomPlot *customPlot : m_customPlot)
    {
        if(customPlot->objectName() == "A")
        {
            PointVector = PointsToSeries(APoints);
            customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
            PointVector = PointsToSeries(BPoints);
            customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
        }

        if(customPlot->objectName() == "RES1")
        {
            PointVector = PointsToSeries(ComputeUnion(APoints, BPoints));
            customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
            PointVector = PointsToSeries(ComputeIntersect(APoints, BPoints));
            customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
        }

        if(customPlot->objectName() == "RES2")
        {
            PointVector = PointsToSeries(ComputeDilation(APoints));
            customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
            PointVector = PointsToSeries(ComputeNormalization(APoints));
            customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
            PointVector = PointsToSeries(ComputeConcentration(APoints));
            customPlot->graph(2)->setData(PointVector[0], PointVector[1]);
        }

        if(customPlot->objectName() == "RES3")
        {
            PointVector = PointsToSeries(ComputeComplement(APoints));
            customPlot->graph(0)->setData(PointVector[0], PointVector[1]);
            PointVector = PointsToSeries(ComputeSupport(APoints));
            customPlot->graph(1)->setData(PointVector[0], PointVector[1]);
        }

        customPlot->rescaleAxes();
        customPlot->yAxis->setRange(-0.02, 1.02);
        customPlot->replot();
    }
}


// return the smaller number of two arguments
double FuzzyOperations::min(double a, double b) const
{
    return (a > b) ? b : a;
}


// return the bigger number of two arguments
double FuzzyOperations::max(double a, double b) const
{
    return (a > b) ? a : b;
}


// create a list of points of triangle membership function
std::vector<Point> FuzzyOperations::ComputeTrianglePoints(double a, double b, double c)
{
    std::vector<Point> ListofPoints;

    LowerLimit = (round((a - 2) * 100.0) < LowerLimit) ? round((a - 2) * 100.0) : LowerLimit;
    HigherLimit = (round((c + 2) * 100.0) > HigherLimit) ? round((c + 2) * 100.0) : HigherLimit;

    for(int x = LowerLimit; x < round((b) * 100.0); x++)
        ListofPoints.push_back(std::make_pair(x, max((x/100.0 - a) / (b - a), 0)));

    for(int x = round((b) * 100.0); x < HigherLimit; x++)
        ListofPoints.push_back(std::make_pair(x, max((c - x/100.0) / (c - b), 0)));

    return ListofPoints;
}


// create a list of points of trapezoidal membership function
std::vector<Point> FuzzyOperations::ComputeTrapezoidalPoints(double a, double b, double c, double d)
{
    std::vector<Point> ListofPoints;
    LowerLimit = (round((a - 2) * 100.0) < LowerLimit ? round((a - 2) * 100.0) : LowerLimit);
    HigherLimit = (round((d + 2) * 100.0) > HigherLimit) ? round((d + 2) * 100.0) : HigherLimit;

    for(int x = LowerLimit; x < round((b) * 100.0); x++)
     ListofPoints.push_back(std::make_pair(x, max((x/100.0 - a) / (b - a), 0)));

    for(int x = round((b) * 100.0); x < round((c) * 100.0); x++)
     ListofPoints.push_back(std::make_pair(x, 1));

    for(int x = round((c) * 100.0); x < HigherLimit; x++)
     ListofPoints.push_back(std::make_pair(x, max((d - x/100.0) / (d - c), 0)));

    return ListofPoints;
}



// create a list of points of Gaussian membership function
std::vector<Point> FuzzyOperations::ComputeGaussianPoints(double a, double b)
{
    std::vector<Point> ListofPoints;
    LowerLimit = (round((a - 4 * b) * 100.0) < LowerLimit) ? round((a - 4 * b) * 100.0) : LowerLimit;
    HigherLimit = (round((a + 4 * b) * 100.0) > HigherLimit) ? round((a + 4 * b) * 100.0) : HigherLimit;

    for(int x = LowerLimit; x < HigherLimit; x++)
        ListofPoints.push_back(std::make_pair(x, exp(-0.5 * pow(double((x/100.0 - a) / b), 2))));

    return ListofPoints;
}


// create a list of points of bell membership function
std::vector<Point> FuzzyOperations::ComputeBellPoints(double a, double b, double c)
{
    std::vector<Point> ListofPoints;

    if(a < 0)
    {
        LowerLimit = (round((c - 4 * (-a)) * 100.0) < LowerLimit) ? round((c - 4 * (-a)) * 100.0) : LowerLimit;
        HigherLimit = (round((c + 4 * (-a)) * 100.0) > HigherLimit) ? round((c + 4 * (-a)) * 100.0) : HigherLimit;
    }
    else
    {
        LowerLimit = (round((c - 4 * a) * 100.0) < LowerLimit) ? round((c - 4 * a) * 100.0) : LowerLimit;
        HigherLimit = (round((c + 4 * a) * 100.0) > HigherLimit) ? round((c + 4 * a) * 100.0) : HigherLimit;
    }

    for(int x = LowerLimit; x < HigherLimit; x++)
        ListofPoints.push_back(std::make_pair(x, double(1 / (1 + pow(double((x/100.0 - c) / a), 2 * b)))));

    return ListofPoints;
}


// create a list of points of sigmoidal membership function
std::vector<Point> FuzzyOperations::ComputeSigmoidalPoints(double a, double c)
{
    std::vector<Point> ListofPoints;

    if(a < 0)
    {
        LowerLimit = (round((c - 2 * (-a)) * 100.0) < LowerLimit) ? round((c - 2 * (-a)) * 100.0) : LowerLimit;
        HigherLimit = (round((c + 2 * (-a)) * 100.0) > HigherLimit) ? round((c + 2 * (-a)) * 100.0) : HigherLimit;
    }
    else
    {
        LowerLimit = (round((c - 2 * a) * 100.0) < LowerLimit) ? round((c - 2 * a) * 100.0) : LowerLimit;
        HigherLimit = (round((c + 2 * a) * 100.0) > HigherLimit) ? round((c + 2 * a) * 100.0) : HigherLimit;
    }

    for(int x = LowerLimit; x < HigherLimit; x++)
        ListofPoints.push_back(std::make_pair(x, double(1 / (1 + exp(-a * (x/100.0 - c))))));

    return ListofPoints;
}


// compute complement of a set passed as argument
std::vector<Point> FuzzyOperations::ComputeComplement(std::vector<Point> APoints) const
{
    std::vector<Point> ListofComplementPoints;

    for(unsigned int x = 0; x < APoints.size(); x++)
        ListofComplementPoints.push_back(std::make_pair(std::get<0>(APoints[x]), 1 - std::get<1>(APoints[x])));

    return ListofComplementPoints;
}


// compute union of two sets passed as arguments
std::vector<Point> FuzzyOperations::ComputeUnion(std::vector<Point> APoints, std::vector<Point> BPoints) const
{
    std::vector<Point> ListofUnionPoints;

    unsigned int indexA = 0;
    unsigned int indexB = 0;

    while (indexA < APoints.size() && indexB < BPoints.size())
    {
        // x-coordinates of both sets are equal
        if(std::get<0>(APoints[indexA]) == std::get<0>(BPoints[indexB]))
        {
            // append the point with greater y value
            if(round((std::get<1>(APoints[indexA])) * 1000) > round(std::get<1>(BPoints[indexB]) * 1000))
                ListofUnionPoints.push_back(APoints[indexA]);
            else
                ListofUnionPoints.push_back(BPoints[indexB]);

            indexA++;
            indexB++;
        }
        else
        {
            // x-coordinate of set A is greater than in set B, B is preceding A where A is not yet defined
            if(std::get<0>(APoints[indexA]) > std::get<0>(BPoints[indexB]))
            {
                // append point from set B
                ListofUnionPoints.push_back(BPoints[indexB]);
                indexB++;
            }
            // A preceding B
            else
            {
                // append point from set A
                ListofUnionPoints.push_back(APoints[indexA]);
                indexA++;
            }
        }
    }

    // append remaining points
    for(; indexA < APoints.size(); indexA++)
        ListofUnionPoints.push_back(APoints[indexA]);

    for(; indexB < BPoints.size(); indexB++)
        ListofUnionPoints.push_back(BPoints[indexB]);

    return ListofUnionPoints;
}


// compute intersection of two sets passed as arguments
std::vector<Point> FuzzyOperations::ComputeIntersect(std::vector<Point> APoints, std::vector<Point> BPoints) const
{
    std::vector<Point> ListofIntersectPoints;

    unsigned int indexA = 0;
    unsigned int indexB = 0;

    while (indexA < APoints.size() && indexB < BPoints.size())
    {
        // x-coordinates of both sets are equal
        if(std::get<0>(APoints[indexA]) == std::get<0>(BPoints[indexB]))
        {
            // append the point with lower y value
            if(round((std::get<1>(APoints[indexA])) * 1000) > round(std::get<1>(BPoints[indexB]) * 1000))
                ListofIntersectPoints.push_back(BPoints[indexB]);
            else
                ListofIntersectPoints.push_back(APoints[indexA]);

            indexA++;
            indexB++;
        }        
        else
        {
            // x-coordinate of set A is greater than in set B, B is preceding A where A is not yet defined
            if(std::get<0>(APoints[indexA]) > std::get<0>(BPoints[indexB]))
            {
                // append point from set B
                ListofIntersectPoints.push_back(std::make_pair(std::get<0>(BPoints[indexB]), 0));
                indexB++;
            }
            // A preceding B
            else
            {
                // append point from set A
                ListofIntersectPoints.push_back(std::make_pair(std::get<0>(APoints[indexA]), 0));
                indexA++;
            }
        }
    }

    // append remaining points
    for(; indexA < APoints.size(); indexA++)
        ListofIntersectPoints.push_back(std::make_pair(std::get<0>(APoints[indexA]), 0));

    for(; indexB < BPoints.size(); indexB++)
        ListofIntersectPoints.push_back(std::make_pair(std::get<0>(BPoints[indexB]), 0));

    return ListofIntersectPoints;
}


// compute concentration of a set passed as argument
std::vector<Point> FuzzyOperations::ComputeConcentration(std::vector<Point> APoints) const
{
    std::vector<Point> ListofConcentrationPoints;

    for(unsigned int x = 0; x < APoints.size(); x++)
        ListofConcentrationPoints.push_back(std::make_pair(std::get<0>(APoints[x]), pow(std::get<1>(APoints[x]), 2)));

    return ListofConcentrationPoints;
}


// compute dilation of a set passed as argument
std::vector<Point> FuzzyOperations::ComputeDilation(std::vector<Point> APoints) const
{
    std::vector<Point> ListofDilationPoints;

    for(unsigned int x = 0; x < APoints.size(); x++)
        ListofDilationPoints.push_back(std::make_pair(std::get<0>(APoints[x]), std::sqrt(std::get<1>(APoints[x]))));

    return ListofDilationPoints;
}


// get maximum membership value in a set passed as argument
double FuzzyOperations::GetMaxMembershipValue(std::vector<Point> ListofPoints) const
{
    double max_value = 0;

    for(unsigned int x = 0; x < ListofPoints.size(); x++)
    {
        if(std::get<1>(ListofPoints[x]) > max_value)
            max_value = std::get<1>(ListofPoints[x]);
    }

    return max_value;
}


// compute normalization of a set passed as argument
std::vector<Point> FuzzyOperations::ComputeNormalization(std::vector<Point> APoints) const
{
    std::vector<Point> ListofNormalizationPoints;
    double max_value = GetMaxMembershipValue(APoints);

    for(unsigned int x = 0; x < APoints.size(); x++)
        ListofNormalizationPoints.push_back(std::make_pair(std::get<0>(APoints[x]), double(std::get<1>(APoints[x]) / max_value)));

    return ListofNormalizationPoints;
}


// get support of a set passed as argument
std::vector<Point> FuzzyOperations::ComputeSupport(std::vector<Point> APoints) const
{
    std::vector<Point> ListofSupportPoints;

    for(unsigned int x = 0; x < APoints.size(); x++)
    {
        if(std::get<1>(APoints[x]) > 0)
            ListofSupportPoints.push_back(APoints[x]);
    }

    return ListofSupportPoints;
}
