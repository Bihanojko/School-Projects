/* Author: Nikola Valesova
 * Date: 15. 10. 2017
 * Subject: Soft Computing
 * File: FuzzyOperations.h
 * Faculty of Information Technology
 * Brno University of Technology
*/


#ifndef FUZZYOPERATIONS_H
#define FUZZYOPERATIONS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <cmath>
#include "libs/qcustomplot.h"


QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE


typedef std::pair<int, double> Point;


class FuzzyOperations: public QWidget
{
    Q_OBJECT

public:
    explicit FuzzyOperations(QWidget *parent = 0);
    ~FuzzyOperations();

private Q_SLOTS:
    void updateUI();

private:
    // ATTRIBUTES

    // vector of points of membership functions A and B
    std::vector<Point> APoints;
    std::vector<Point> BPoints;

    // properties for storing GUI elements
    QList<QCustomPlot *> m_customPlot;
    QComboBox *m_SetAComboBox;
    QComboBox *m_SetBComboBox;

    // line edit fields for inserting input parameters
    QLineEdit *ALineEditA;
    QLineEdit *ALineEditB;
    QLineEdit *ALineEditC;
    QLineEdit *ALineEditD;

    QLineEdit *BLineEditA;
    QLineEdit *BLineEditB;
    QLineEdit *BLineEditC;
    QLineEdit *BLineEditD;

    // labels describing line edit fields
    QLabel *ALabelA;
    QLabel *ALabelB;
    QLabel *ALabelC;
    QLabel *ALabelD;
    QLabel *BLabelA;
    QLabel *BLabelB;
    QLabel *BLabelC;
    QLabel *BLabelD;

    // type of membership functions of set A and B
    int AMembershipFunctionType = 0;
    int BMembershipFunctionType = 0;

    // limits of x-coordinations in graphs
    int LowerLimit = 1000;
    int HigherLimit = -1000;

    // input parameters for computing membership functions
    double A_a = 2.0;
    double A_b = 4.0;
    double A_c = 6.0;
    double A_d = 8.0;
    double B_a = 2.0;
    double B_b = 4.0;
    double B_c = 6.0;
    double B_d = 8.0;


    // METHODS

    // methods computing points of input membership functions
    std::vector<Point> ComputeTrianglePoints(double a, double b, double c);
    std::vector<Point> ComputeTrapezoidalPoints(double a, double b, double c, double d);
    std::vector<Point> ComputeGaussianPoints(double a, double b);
    std::vector<Point> ComputeBellPoints(double a, double b, double c);
    std::vector<Point> ComputeSigmoidalPoints(double a, double c);

    // binary operations over membership functions
    std::vector<Point> ComputeUnion(std::vector<Point> APoints, std::vector<Point> BPoints) const;
    std::vector<Point> ComputeIntersect(std::vector<Point> APoints, std::vector<Point> BPoints) const;

    // unary operations over membership functions
    std::vector<Point> ComputeComplement(std::vector<Point> ListofPoints) const;
    std::vector<Point> ComputeConcentration(std::vector<Point> ListofPoints) const;
    std::vector<Point> ComputeDilation(std::vector<Point> ListofPoints) const;
    std::vector<Point> ComputeNormalization(std::vector<Point> ListofPoints) const;
    std::vector<Point> ComputeSupport(std::vector<Point> APoints) const;

    // methods creating and manipulating the GUI
    QComboBox *CreateSetTypesA() const;
    QComboBox *CreateSetTypesB() const;
    QHBoxLayout *CreateParameterSettingsLayout(std::string SetName) const;
    QLineEdit *CreateLineEdit(std::string content, std::string name) const;
    QLabel *CreateLabel(std::string content, std::string name) const;
    QCustomPlot *CreateCustomPlot(int SeriesCount) const;
    void ConnectSlotsToSignals();
    void SetParameterVisibility() const;
    void SetBorderColors() const;
    void SetLabelContent() const;
    void ActualizeInputValues();

    // other auxiliary and control methods
    std::vector<Point> GetInputPoints(int SetType, std::string SetName);
    bool CheckInputParameterValues();
    bool IsDouble(QLineEdit *LineEdit) const;
    std::vector<QVector<double> > PointsToSeries(std::vector<Point> InputPoints) const;
    double GetMaxMembershipValue(std::vector<Point> ListofPoints) const;
    double min(double a, double b) const;
    double max(double a, double b) const;
};

#endif /* FUZZYOPERATIONS_H */
