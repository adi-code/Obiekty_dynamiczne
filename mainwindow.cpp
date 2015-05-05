#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QFont big_bold_font("Verdana", 16, QFont::Bold);
    QFont medium_bold_font("Verdana", 12, QFont::Bold);
    QFont small_bold_font("Verdana", 10, QFont::Bold);
    QFont common_font("Verdana", 10);

    m_push_button_run_algorithm = new QPushButton(tr("Uruchom algorytm"));
    m_first_algorithm_button = new QRadioButton(tr("Algorytm nr 1"));
    m_second_algorithm_button = new QRadioButton(tr("Algorytm nr 2"));
    m_third_algorithm_button = new QRadioButton(tr("Algorytm nr 3"));
    m_algorithm_buttons_group = new QButtonGroup;
    m_algorithm_buttons_group->addButton(m_first_algorithm_button);
    m_algorithm_buttons_group->addButton(m_second_algorithm_button);
    m_algorithm_buttons_group->addButton(m_third_algorithm_button);
    m_push_button_run_algorithm->setFont(small_bold_font);
    m_first_algorithm_button->setFont(big_bold_font);
    m_second_algorithm_button->setFont(big_bold_font);
    m_third_algorithm_button->setFont(big_bold_font);

    m_spin_box_population_size = new QSpinBox;
    m_spin_box_population_size->setMinimum(2);
    m_spin_box_population_size->setMaximum(10*1000);
    m_spin_box_population_size->setValue(500);
    m_spin_box_population_size->setMinimumWidth(50);

    m_spin_box_number_of_iterations = new QSpinBox;
    m_spin_box_number_of_iterations->setMinimum(1);
    m_spin_box_number_of_iterations->setMaximum(10*1000);
    m_spin_box_number_of_iterations->setValue(200);
    m_spin_box_number_of_iterations->setMinimumWidth(50);

    m_spin_box_max_kr = new QSpinBox;
    m_spin_box_max_kr->setMinimum(0);
    m_spin_box_max_kr->setMaximum(10*1000);
    m_spin_box_max_kr->setValue(1000);
    m_spin_box_max_kr->setMinimumWidth(50);

    m_spin_box_max_ti = new QSpinBox;
    m_spin_box_max_ti->setMinimum(0);
    m_spin_box_max_ti->setMaximum(10*1000);
    m_spin_box_max_ti->setValue(1000);
    m_spin_box_max_ti->setMinimumWidth(50);

    m_spin_box_max_td = new QSpinBox;
    m_spin_box_max_td->setMinimum(0);
    m_spin_box_max_td->setMaximum(10*1000);
    m_spin_box_max_td->setValue(1000);
    m_spin_box_max_td->setMinimumWidth(50);

    m_spin_box_max_kd = new QSpinBox;
    m_spin_box_max_kd->setMinimum(0);
    m_spin_box_max_kd->setMaximum(10*1000);
    m_spin_box_max_kd->setValue(1000);
    m_spin_box_max_kd->setMinimumWidth(50);

    m_spin_box_response_time = new QSpinBox;
    m_spin_box_response_time->setMinimum(0);
    m_spin_box_response_time->setMaximum(10*1000);
    m_spin_box_response_time->setValue(100);
    m_spin_box_response_time->setMinimumWidth(50);

    m_spin_box_controlled_process_dimension = new QSpinBox;
    m_spin_box_controlled_process_dimension->setMinimum(1);
    m_spin_box_controlled_process_dimension->setMaximum(9);
    m_spin_box_controlled_process_dimension->setValue(1);
    m_spin_box_controlled_process_dimension->setMinimumWidth(50);

    m_label_algorithm_parameters = new QLabel(tr("Parametry algorytmu"));
    m_label_algorithm_parameters->setFont(medium_bold_font);
    m_label_algorithm_parameters->setAlignment(Qt::AlignCenter);

    m_label_optimalization_problem_parameters = new QLabel(tr("Parametry zadania optymalizacji"));
    m_label_optimalization_problem_parameters->setFont(medium_bold_font);
    m_label_optimalization_problem_parameters->setAlignment(Qt::AlignCenter);

    m_label_result = new QLabel(tr("Brak wyników"));
    m_label_result->setFont(medium_bold_font);
    m_label_result->setAlignment(Qt::AlignCenter);

    m_label_population_size = new QLabel(tr("Liczebność populacji"));
    m_label_population_size->setFont(common_font);
    m_label_population_size->setAlignment(Qt::AlignCenter);

    m_label_number_of_iterations = new QLabel(tr("Liczba iteracji"));
    m_label_number_of_iterations->setFont(common_font);
    m_label_number_of_iterations->setAlignment(Qt::AlignCenter);

    m_label_max_kr = new QLabel(tr("Maksymalne Kr"));
    m_label_max_kr->setFont(common_font);
    m_label_max_kr->setAlignment(Qt::AlignCenter);

    m_label_max_ti = new QLabel(tr("Maksymalne Ti"));
    m_label_max_ti->setFont(common_font);
    m_label_max_ti->setAlignment(Qt::AlignCenter);

    m_label_max_td = new QLabel(tr("Maksymalne Td"));
    m_label_max_td->setFont(common_font);
    m_label_max_td->setAlignment(Qt::AlignCenter);

    m_label_max_kd = new QLabel(tr("Maksymalne Kd"));
    m_label_max_kd->setFont(common_font);
    m_label_max_kd->setAlignment(Qt::AlignCenter);

    m_label_response_time = new QLabel(tr("Czas analizy odpowiedzi sk."));
    m_label_response_time->setFont(common_font);
    m_label_response_time->setAlignment(Qt::AlignCenter);

    m_label_controlled_process_dimension = new QLabel(tr("Rząd obiektu"));
    m_label_controlled_process_dimension->setFont(common_font);
    m_label_controlled_process_dimension->setAlignment(Qt::AlignCenter);

    m_spin_box_numerator_parameter = new QDoubleSpinBox*[10];
    m_spin_box_denominator_parameter = new QDoubleSpinBox*[10];
    m_label_parameter_index = new QLabel*[10];
    m_label_numerator_parameter = new QLabel*[10];
    m_label_denominator_parameter = new QLabel*[10];

    for(int i=0; i<10; i++)
    {
        m_spin_box_numerator_parameter[i] = new QDoubleSpinBox;
        m_spin_box_numerator_parameter[i]->setMinimum(-10*1000);
        m_spin_box_numerator_parameter[i]->setSingleStep(0.01);
        m_spin_box_numerator_parameter[i]->setMaximum(10*1000);
        m_spin_box_numerator_parameter[i]->setValue(1);

        m_spin_box_denominator_parameter[i] = new QDoubleSpinBox;
        m_spin_box_denominator_parameter[i]->setMinimum(-10*1000);
        m_spin_box_denominator_parameter[i]->setSingleStep(0.01);
        m_spin_box_denominator_parameter[i]->setMaximum(10*1000);
        m_spin_box_denominator_parameter[i]->setValue(1);

        if(i > 1)
        {
            m_spin_box_numerator_parameter[i]->setDisabled(true);
            m_spin_box_denominator_parameter[i]->setDisabled(true);
        }
        m_spin_box_numerator_parameter[i]->setMinimumWidth(50);
        m_spin_box_denominator_parameter[i]->setMinimumWidth(50);

        QString m_label_string = "Parametr nr " + QString::number(i) + ":";
        m_label_parameter_index[i] = new QLabel(m_label_string);
        m_label_parameter_index[i]->setFont(small_bold_font);
        m_label_parameter_index[i]->setAlignment(Qt::AlignCenter);

        m_label_numerator_parameter[i] = new QLabel(tr("Licznik"));
        m_label_numerator_parameter[i]->setFont(common_font);
        m_label_numerator_parameter[i]->setAlignment(Qt::AlignCenter);

        m_label_denominator_parameter[i] = new QLabel(tr("Mianownik"));
        m_label_denominator_parameter[i]->setFont(common_font);
        m_label_denominator_parameter[i]->setAlignment(Qt::AlignCenter);
    }

    m_layout = new QGridLayout();
    m_layout->addWidget(m_first_algorithm_button, 0, 1, 2, 3, Qt::AlignCenter);
    m_layout->addWidget(m_second_algorithm_button, 0, 4, 2, 3, Qt::AlignCenter);
    m_layout->addWidget(m_third_algorithm_button, 0, 7, 2, 3, Qt::AlignCenter);
    m_layout->addWidget(m_label_algorithm_parameters, 2, 0, 1, 2, Qt::AlignCenter);
    m_layout->addWidget(m_label_population_size, 3, 0, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_population_size, 3, 1, Qt::AlignCenter);
    m_layout->addWidget(m_label_number_of_iterations, 4, 0, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_number_of_iterations, 4, 1, Qt::AlignCenter);
    m_layout->addWidget(m_label_optimalization_problem_parameters, 2, 3, 1, 7, Qt::AlignCenter);
    m_layout->addWidget(m_label_max_kr, 3, 4, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_max_kr, 3, 5, Qt::AlignCenter);
    m_layout->addWidget(m_label_max_ti, 3, 6, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_max_td, 3, 7, Qt::AlignCenter);
    m_layout->addWidget(m_label_max_td, 3, 8, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_max_ti, 3, 9, Qt::AlignCenter);
    m_layout->addWidget(m_label_max_kd, 4, 4, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_max_kd, 4, 5, Qt::AlignCenter);
    m_layout->addWidget(m_label_response_time, 4, 6, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_response_time, 4, 7, Qt::AlignCenter);
    m_layout->addWidget(m_label_controlled_process_dimension, 4, 8, Qt::AlignCenter);
    m_layout->addWidget(m_spin_box_controlled_process_dimension, 4, 9, Qt::AlignCenter);
    m_layout->addWidget(m_push_button_run_algorithm, 15, 0, 1, 2, Qt::AlignCenter);
    m_layout->addWidget(m_label_result, 15, 2, 1, 8, Qt::AlignCenter);
    for(int i=0; i<10; i++)
    {
        m_layout->addWidget(m_label_parameter_index[i], 5+i, 3, Qt::AlignCenter);
        m_layout->addWidget(m_label_numerator_parameter[i], 5+i, 4, Qt::AlignCenter);
        m_layout->addWidget(m_spin_box_numerator_parameter[i], 5+i, 5, Qt::AlignCenter);
        m_layout->addWidget(m_label_denominator_parameter[i], 5+i, 6, Qt::AlignCenter);
        m_layout->addWidget(m_spin_box_denominator_parameter[i], 5+i, 7, Qt::AlignCenter);
    }

    m_main_widget = new QWidget();
    m_main_widget->setLayout(m_layout);
    setCentralWidget(m_main_widget);

    m_controlled_process_dimension = 1;

    m_algorithms_handler = new AlgorithmsHandler;
    m_algorithms_handler->moveToThread(&m_calculating_thread);


    connect(this, SIGNAL(RunAlgorithm()),
            m_algorithms_handler, SLOT(RunAlgorithm()));
    connect(m_algorithms_handler, SIGNAL(EndOfAlgorithm()),
            this, SLOT(HandleEndOfAlgorithm()));
    connect(&m_calculating_thread, SIGNAL(finished()),
            m_algorithms_handler, SLOT(deleteLater()));
    connect(m_push_button_run_algorithm, SIGNAL(clicked()),
            this, SLOT(PrepareOptimizationAlgorithm()));
    connect(m_spin_box_controlled_process_dimension, SIGNAL(valueChanged(int)),
            this, SLOT(ChangeEnabledParameters(int)));
}

MainWindow::~MainWindow()
{
    m_calculating_thread.quit();
    m_calculating_thread.wait();
    delete m_push_button_run_algorithm;
    delete m_first_algorithm_button;
    delete m_second_algorithm_button;
    delete m_third_algorithm_button;
    delete m_algorithm_buttons_group;
    delete m_spin_box_population_size;
    delete m_spin_box_number_of_iterations;
    delete m_spin_box_max_kr;
    delete m_spin_box_max_ti;
    delete m_spin_box_max_td;
    delete m_spin_box_max_kd;
    delete m_spin_box_response_time;
    delete m_spin_box_controlled_process_dimension;
    delete m_label_algorithm_parameters;
    delete m_label_optimalization_problem_parameters;
    delete m_label_population_size;
    delete m_label_number_of_iterations;
    delete m_label_max_kr;
    delete m_label_max_ti;
    delete m_label_max_td;
    delete m_label_max_kd;
    delete m_label_response_time;
    delete m_label_controlled_process_dimension;
    delete m_label_result;
    for(int i=0; i<10; i++)
    {
        delete m_label_parameter_index[i];
        delete m_label_numerator_parameter[i];
        delete m_label_denominator_parameter[i];
        delete m_spin_box_numerator_parameter[i];
        delete m_spin_box_denominator_parameter[i];
    }
    delete m_label_parameter_index;
    delete m_label_numerator_parameter;
    delete m_label_denominator_parameter;
    delete m_spin_box_numerator_parameter;
    delete m_spin_box_denominator_parameter;

    delete m_layout;
    delete m_main_widget;
}

void MainWindow::PrepareOptimizationAlgorithm()
{
    m_push_button_run_algorithm->setDisabled(true);
    m_spin_box_controlled_process_dimension->setDisabled(true);
    for(int index = 0; index <= m_controlled_process_dimension; index++)
    {
        m_spin_box_numerator_parameter[index]->setDisabled(true);
        m_spin_box_denominator_parameter[index]->setDisabled(true);
    }
    m_spin_box_max_kd->setDisabled(true);
    m_spin_box_max_kr->setDisabled(true);
    m_spin_box_response_time->setDisabled(true);
    m_spin_box_max_td->setDisabled(true);
    m_spin_box_max_ti->setDisabled(true);
    m_spin_box_number_of_iterations->setDisabled(true);
    m_spin_box_population_size->setDisabled(true);

    m_algorithms_handler->SetControlledProcessDimension(
                m_controlled_process_dimension);
    std::vector<double> numerator_parameters;
    std::vector<double> denominator_parameters;
    numerator_parameters.clear();
    denominator_parameters.clear();
    for(int index = 0; index <= m_controlled_process_dimension; index ++)
    {
        numerator_parameters.push_back(
                    m_spin_box_numerator_parameter[index]->value());
        denominator_parameters.push_back(
                    m_spin_box_denominator_parameter[index]->value());
    }
    m_algorithms_handler->SetNumeratorParameters(numerator_parameters);
    m_algorithms_handler->SetDenominatorParameters(denominator_parameters);
    m_algorithms_handler->SetMaxResponseTime(
                m_spin_box_response_time->value());

    m_label_result->setText("Obliczenia w toku...");

    m_calculating_thread.start();
    emit RunAlgorithm();
}

void MainWindow::HandleEndOfAlgorithm()
{
    m_push_button_run_algorithm->setDisabled(false);
    m_spin_box_controlled_process_dimension->setDisabled(false);
    for(int index = 0; index <= m_controlled_process_dimension; index++)
    {
        m_spin_box_numerator_parameter[index]->setDisabled(false);
        m_spin_box_denominator_parameter[index]->setDisabled(false);
    }
    m_spin_box_max_kd->setDisabled(false);
    m_spin_box_max_kr->setDisabled(false);
    m_spin_box_response_time->setDisabled(false);
    m_spin_box_max_td->setDisabled(false);
    m_spin_box_max_ti->setDisabled(false);
    m_spin_box_number_of_iterations->setDisabled(false);
    m_spin_box_population_size->setDisabled(false);

    m_label_result->setText("Obliczenia skończone");
}

void MainWindow::ChangeEnabledParameters(int p_controlled_process_dimension)
{
    if(m_controlled_process_dimension > p_controlled_process_dimension)
    {
        for(int index = 0;
            index < m_controlled_process_dimension - p_controlled_process_dimension;
            index++)
        {
            m_spin_box_numerator_parameter[
                    p_controlled_process_dimension + index + 1]->setDisabled(true);
            m_spin_box_denominator_parameter[
                    p_controlled_process_dimension + index + 1]->setDisabled(true);
        }
    }
    else
    {
        for(int index = m_controlled_process_dimension - p_controlled_process_dimension;
            index < 0;
            index++)
        {
            m_spin_box_numerator_parameter[
                    p_controlled_process_dimension + index + 1]->setDisabled(false);
            m_spin_box_denominator_parameter[
                    p_controlled_process_dimension + index + 1]->setDisabled(false);
        }
    }
    m_controlled_process_dimension = p_controlled_process_dimension;
}
