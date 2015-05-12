#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "algorithmshandler.h"
#include <QList>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QThread>
#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QPushButton* m_push_button_run_algorithm;
    QRadioButton* m_first_algorithm_button;
    QRadioButton* m_second_algorithm_button;
    QRadioButton* m_third_algorithm_button;
    QButtonGroup* m_algorithm_buttons_group;
    QSpinBox* m_spin_box_population_size;
    QSpinBox* m_spin_box_number_of_iterations;
    QSpinBox* m_spin_box_max_kr;
    QSpinBox* m_spin_box_max_ti;
    QSpinBox* m_spin_box_max_td;
    QSpinBox* m_spin_box_max_kd;
    QSpinBox* m_spin_box_response_time;
    QSpinBox* m_spin_box_controlled_process_dimension;
    QDoubleSpinBox** m_spin_box_numerator_parameter;
    QDoubleSpinBox** m_spin_box_denominator_parameter;
    QGridLayout* m_layout;
    QLabel* m_label_algorithm_parameters;
    QLabel* m_label_optimalization_problem_parameters;
    QLabel* m_label_population_size;
    QLabel* m_label_number_of_iterations;
    QLabel* m_label_max_kr;
    QLabel* m_label_max_ti;
    QLabel* m_label_max_td;
    QLabel* m_label_max_kd;
    QLabel* m_label_response_time;
    QLabel* m_label_controlled_process_dimension;
    QLabel* m_label_result;
    QLabel** m_label_parameter_index;
    QLabel** m_label_numerator_parameter;
    QLabel** m_label_denominator_parameter;
    QWidget* m_main_widget;

    QList<QWidget*> m_custom_widgets;

    unsigned int m_controlled_process_dimension;

    AlgorithmsHandler* m_algorithms_handler;
    QThread m_calculating_thread;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(); 
    
private:
    void ClearCustomWidgets();
    std::vector<double> getParameters();

public slots:
    void AddWidget(const QString& p_label, unsigned int p_type = 0,
                   double p_low_limit = 0, double p_high_limit = 1,
                   double p_value = 0);
    void ShowMessage(const QString& p_msg);

private slots:
    void PrepareOptimizationAlgorithm();
    void HandleEndOfAlgorithm();
    void ChangeEnabledParameters(int p_controlled_process_dimension);
    void SelectAlgorithm(int p_i);

signals:
    void RunAlgorithm();
    
};

#endif // MAINWINDOW_H
