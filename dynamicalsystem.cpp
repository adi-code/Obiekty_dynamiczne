#include "dynamicalsystem.h"

using namespace boost::numeric::ublas;

DynamicalSystem::DynamicalSystem(unsigned int p_system_dimension,
                                 std::vector<double> p_numerator_parameters,
                                 std::vector<double> p_denominator_parameters) :
    m_system_dimension(p_system_dimension),
    m_a_matrix(p_system_dimension, p_system_dimension),
    m_b_matrix(p_system_dimension, 1),
    m_c_matrix(1, p_system_dimension),
    m_d_matrix(1, 1),
    m_numerator_parameters(p_numerator_parameters),
    m_denominator_parameters(p_denominator_parameters),
    m_u_part(m_b_matrix, 0)
{
    if(p_numerator_parameters.size() != p_system_dimension + 1)
    {
        throw inproper_system_data;
    }
    if(p_denominator_parameters.size() != p_system_dimension + 1)
    {
        throw inproper_system_data;
    }

    //CalculateMatrices();
}

DynamicalSystem::DynamicalSystem(DynamicalSystem *p_controlled_process,
                                 DynamicalSystem *p_regulator) :
    m_system_dimension(p_controlled_process->GetSystemDimension() +
            p_regulator->GetSystemDimension()),
    m_a_matrix(m_system_dimension, m_system_dimension),
    m_b_matrix(m_system_dimension, 1),
    m_c_matrix(1, m_system_dimension),
    m_d_matrix(1, 1),
    m_u_part(m_b_matrix, 0)
{
    unsigned int controlled_process_dimension =
            p_controlled_process->GetSystemDimension();
    unsigned int regulator_dimension =
            p_regulator->GetSystemDimension();

    //Wyznaczanie współczynników licznika
    m_numerator_parameters.clear();
    m_numerator_parameters.resize(m_system_dimension + 1, 0);
    for(unsigned int numerator_index = 0; numerator_index <= m_system_dimension / 2;
         numerator_index++)
    {
        for(unsigned int iterator = 0; iterator <= numerator_index; iterator++)
        {
            if(p_controlled_process->GetNumerator().size() > iterator &&
                    p_regulator->GetNumerator().size() > numerator_index - iterator)
            {
                m_numerator_parameters[numerator_index] +=
                        p_controlled_process->GetNumerator()[iterator] *
                        p_regulator->GetNumerator()[numerator_index - iterator];
            }
            if(iterator <= numerator_index &&
                    controlled_process_dimension >= iterator)
            {
                m_numerator_parameters[m_system_dimension - numerator_index] +=
                        p_controlled_process->GetNumerator()[controlled_process_dimension - iterator] *
                        p_regulator->GetNumerator()[regulator_dimension + iterator - numerator_index];
            }

        }
    }
    if(m_system_dimension % 2 == 0)
    {
        for(unsigned int iterator = 0; iterator <= (m_system_dimension / 2) + 1; iterator++)
        {
            if(p_controlled_process->GetNumerator().size() > iterator &&
                    p_regulator->GetNumerator().size() > (m_system_dimension / 2) + 1 - iterator)
            {
                m_numerator_parameters[(m_system_dimension / 2) + 1] +=
                        p_controlled_process->GetNumerator()[iterator] *
                        p_regulator->GetNumerator()[(m_system_dimension / 2) + 1 - iterator];
            }
        }
    }

    //Wyznaczanie współczynników mianownika
    m_numerator_parameters.clear();
    m_numerator_parameters.resize(m_system_dimension + 1, 0);
    for(int numerator_index = 0; numerator_index <= m_system_dimension / 2;
         numerator_index++)
    {
        for(int iterator = 0; iterator <= numerator_index; iterator++)
        {
            if(p_controlled_process->GetNumerator().size() > iterator &&
                    p_regulator->GetNumerator().size() > numerator_index - iterator)
            {
                m_numerator_parameters[numerator_index] +=
                        p_controlled_process->GetNumerator()[iterator] *
                        p_regulator->GetNumerator()[numerator_index - iterator];
            }
            if(iterator <= numerator_index &&
                    controlled_process_dimension >= iterator)
            {
                m_numerator_parameters[m_system_dimension - numerator_index] +=
                        p_controlled_process->GetNumerator()[controlled_process_dimension - iterator] *
                        p_regulator->GetNumerator()[regulator_dimension + iterator - numerator_index];
            }

        }
    }
    if(m_system_dimension % 2 == 0)
    {
        m_numerator_parameters[(m_system_dimension / 2)] /= 2;
//        m_denominator_parameters[(m_system_dimension / 2)] /= 2;
    }

//    if(m_system_dimension % 2 == 0)
//    {
//        for(int iterator = 0; iterator <= (m_system_dimension / 2); iterator++)
//        {
//            if(p_controlled_process->GetNumerator().size() > iterator &&
//                    p_regulator->GetNumerator().size() > (m_system_dimension / 2) - iterator)
//            {
//                m_numerator_parameters[(m_system_dimension / 2)] +=
//                        p_controlled_process->GetNumerator()[iterator] *
//                        p_regulator->GetNumerator()[(m_system_dimension / 2) - iterator];
//            }
//        }
//    }

    //Wyznaczanie współczynników mianownika
    m_denominator_parameters.clear();
    m_denominator_parameters.resize(m_system_dimension + 1, 0);
    for(int denominator_index = 0; denominator_index <= m_system_dimension / 2;
         denominator_index++)
    {
        for(int iterator = 0; iterator <= denominator_index; iterator++)
        {
            if(p_controlled_process->GetDenominator().size() > iterator &&
                    p_regulator->GetDenominator().size() > denominator_index - iterator)
            {
                m_denominator_parameters[denominator_index] +=
                        p_controlled_process->GetDenominator()[iterator] *
                        p_regulator->GetDenominator()[denominator_index - iterator];
            }
            if(iterator <= denominator_index &&
                    controlled_process_dimension >= iterator)
            {
                m_denominator_parameters[m_system_dimension - denominator_index] +=
                        p_controlled_process->GetDenominator()[controlled_process_dimension - iterator] *
                        p_regulator->GetDenominator()[regulator_dimension + iterator - denominator_index];
            }
        }
        m_denominator_parameters[denominator_index] += m_numerator_parameters[denominator_index];
        m_denominator_parameters[m_system_dimension - denominator_index] +=
                m_numerator_parameters[m_system_dimension - denominator_index];
    }

    if(m_system_dimension % 2 == 0)
    {
//        m_numerator_parameters[(m_system_dimension / 2)] /= 2;
        m_denominator_parameters[(m_system_dimension / 2)] /= 2;
    }

//    if(m_system_dimension % 2 == 0)
//    {
//        for(int iterator = 0; iterator <= (m_system_dimension / 2); iterator++)
//        {
//            if(p_controlled_process->GetDenominator().size() > iterator &&
//                    p_regulator->GetDenominator().size() > (m_system_dimension / 2) - iterator)
//            {
//                m_denominator_parameters[(m_system_dimension / 2)] +=
//                        p_controlled_process->GetDenominator()[iterator] *
//                        p_regulator->GetDenominator()[(m_system_dimension / 2) - iterator];
//            }

//        }
//        m_denominator_parameters[(m_system_dimension / 2)] += m_numerator_parameters[(m_system_dimension / 2)];
//    }

    m_a_matrix.resize(m_system_dimension, m_system_dimension);
    m_a_matrix.clear();
    m_b_matrix.resize(m_system_dimension, 1);
    m_b_matrix.clear();
    m_c_matrix.resize(1, m_system_dimension);
    m_c_matrix.clear();
    m_d_matrix.resize(1, 1);
    m_d_matrix.clear();

    CalculateMatrices();

    ///////////////////////////////////
    //Alternatywne wyznaczanie macierzy
    ///////////////////////////////////

//    DSIdentityMatrix e_matrix(1);
//    DSMatrix common_matrix(1, 1);

//    //Obliczanie [I+D*Dr]^(-1)
//    common_matrix.SetAsResultOfMultiply(p_controlled_process->GetDMatrix(),
//                             p_regulator->GetDMatrix());
//    common_matrix = e_matrix + common_matrix;
//    common_matrix.InvertMatrix();

//    //Obliczanie A
//    m_a_matrix.resize(m_system_dimension, m_system_dimension);
//    m_a_matrix.clear();
//    DSMatrix auxilary_matrix;
//    auxilary_matrix.SetAsResultOfMultiply(p_controlled_process->GetBMatrix(),
//                               p_regulator->GetDMatrix());
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               common_matrix);
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_controlled_process->GetCMatrix());
//    auxilary_matrix = p_controlled_process->GetAMatrix() - auxilary_matrix;
//    project(m_a_matrix,
//            range(0, controlled_process_dimension),
//            range(0, controlled_process_dimension)) =
//            auxilary_matrix;

//    auxilary_matrix.SetAsResultOfMultiply(p_regulator->GetDMatrix(),
//                               common_matrix);
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_controlled_process->GetDMatrix());
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_regulator->GetCMatrix());
//    auxilary_matrix = p_regulator->GetCMatrix() - auxilary_matrix;
//    auxilary_matrix.SetAsResultOfMultiply(p_controlled_process->GetBMatrix(),
//                               auxilary_matrix);
//    project(m_a_matrix,
//            range(0, controlled_process_dimension),
//            range(controlled_process_dimension, m_system_dimension)) =
//            auxilary_matrix;

//    auxilary_matrix.SetAsResultOfMultiply(p_regulator->GetBMatrix(),
//                               common_matrix);
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_controlled_process->GetCMatrix());
//    auxilary_matrix = (-1) * auxilary_matrix;
//    project(m_a_matrix,
//            range(controlled_process_dimension, m_system_dimension),
//            range(0, controlled_process_dimension)) =
//            auxilary_matrix;

//    auxilary_matrix.SetAsResultOfMultiply(p_regulator->GetBMatrix(),
//                               common_matrix);
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_controlled_process->GetDMatrix());
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_regulator->GetCMatrix());
//    auxilary_matrix = p_regulator->GetAMatrix() - auxilary_matrix;

//    project(m_a_matrix,
//            range(controlled_process_dimension, m_system_dimension),
//            range(controlled_process_dimension, m_system_dimension)) =
//            auxilary_matrix;

//    //Obliczanie B
//    m_b_matrix.resize(m_system_dimension, 1);
//    m_b_matrix.clear();
//    DSMatrix second_auxilary_matrix;
//    auxilary_matrix.SetAsResultOfMultiply(common_matrix,
//                               p_controlled_process->GetDMatrix());
//    second_auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_regulator->GetDMatrix());
//    second_auxilary_matrix = e_matrix - second_auxilary_matrix;
//    auxilary_matrix.SetAsResultOfMultiply(p_regulator->GetBMatrix(),
//                               second_auxilary_matrix);
//    project(m_b_matrix,
//            range(controlled_process_dimension, m_system_dimension),
//            range(0, 1)) =
//            auxilary_matrix;

//    auxilary_matrix.SetAsResultOfMultiply(p_regulator->GetDMatrix(),
//                               second_auxilary_matrix);
//    auxilary_matrix.SetAsResultOfMultiply(p_controlled_process->GetBMatrix(),
//                               auxilary_matrix);
//    project(m_b_matrix,
//            range(0, controlled_process_dimension),
//            range(0, 1)) =
//            auxilary_matrix;

//    //Obliczanie C
//    m_c_matrix.resize(1, m_system_dimension);
//    m_c_matrix.clear();
//    auxilary_matrix.SetAsResultOfMultiply(common_matrix,
//                               p_controlled_process->GetCMatrix());
//    project(m_c_matrix,
//            range(0, 1),
//            range(0, controlled_process_dimension)) =
//            auxilary_matrix;

//    auxilary_matrix.SetAsResultOfMultiply(common_matrix,
//                               p_controlled_process->GetDMatrix());
//    auxilary_matrix.SetAsResultOfMultiply(auxilary_matrix,
//                               p_regulator->GetCMatrix());
//    project(m_c_matrix,
//            range(0, 1),
//            range(controlled_process_dimension, m_system_dimension)) =
//            auxilary_matrix;

//    //Obliczanie D
//    m_d_matrix.resize(1, 1);
//    m_d_matrix.SetAsResultOfMultiply(p_controlled_process->GetDMatrix(),
//                               p_regulator->GetDMatrix());

}

DynamicalSystem::DynamicalSystem() :
    m_system_dimension(0),
    m_a_matrix(m_system_dimension, m_system_dimension),
    m_b_matrix(m_system_dimension, 1),
    m_c_matrix(1, m_system_dimension),
    m_d_matrix(1, 1),
    m_u_part(m_b_matrix, 0)
{

}

unsigned int DynamicalSystem::GetSystemDimension()
{
    return m_system_dimension;
}

DSMatrix DynamicalSystem::GetAMatrix()
{
    return m_a_matrix;
}

DSMatrix DynamicalSystem::GetBMatrix()
{
    return m_b_matrix;
}

DSMatrix DynamicalSystem::GetCMatrix()
{
    return m_c_matrix;
}

DSMatrix DynamicalSystem::GetDMatrix()
{
    return m_d_matrix;
}

std::vector<double> DynamicalSystem::GetNumerator()
{
    return m_numerator_parameters;
}

std::vector<double> DynamicalSystem::GetDenominator()
{
    return m_denominator_parameters;
}

bool DynamicalSystem::IsSystemStable()
{
//    // alternative method using Eigen library
//    // checking real part of poles
//    // remeber to #include "Eigen/Eigenvalues"
//    CalculateMatrices();
//    Eigen::MatrixXd m(1,1);
//    unsigned int size1 = m_a_matrix.size1();
//    unsigned int size2 = m_a_matrix.size2();
//    m.resize(size1, size2);
//    for(unsigned int i=0;i<size1;++i) {
//        for(unsigned int j=0;j<size2;++j) {
//            m(i, j) = m_a_matrix(i, j);
//        }
//    }

//    Eigen::EigenSolver<Eigen::MatrixXd> es(m);
//    auto eigenvalues = es.eigenvalues();
//    unsigned int size = eigenvalues.rows();
//    for(unsigned int i=0;i<size;++i) {
//        std::complex<double> lambda = eigenvalues[i];
//        if(lambda.real() >= 0.0) {
//            return false;
//        }
//    }

//    return true;

    DSMatrix hurwitz_matrix(m_system_dimension, m_system_dimension);
    hurwitz_matrix.clear();
    bool is_system_astatic = false;
    for(unsigned int parameter_index = 0;
        parameter_index <= m_system_dimension;
        parameter_index++)
    {
        if(m_denominator_parameters[parameter_index] < 0)
        {
            return false;
        }
        else if(m_denominator_parameters[parameter_index] == 0 &&
                !is_system_astatic)
        {
            is_system_astatic = true;
        }
        else if(m_denominator_parameters[parameter_index] == 0 &&
                is_system_astatic)
        {
            return false;
        }
    }

    for(unsigned int row_index = 1;
        row_index <= m_system_dimension;
        row_index++)
    {
        for(unsigned int column_index = 1;
            column_index <= m_system_dimension;
            column_index++)
        {
            int parameter_index = (2 * column_index) - row_index;
            if(parameter_index >= 0 && static_cast<unsigned int>(parameter_index) <= m_system_dimension)
            {
                hurwitz_matrix(row_index - 1, column_index - 1) =
                        m_denominator_parameters[parameter_index];
            }
        }
    }
    DSMatrix auxilary_matrix;
    for(unsigned int index = 1; index < m_system_dimension - 1; index++)
    {
        auxilary_matrix.resize(index + 1, index + 1);
        auxilary_matrix = project(hurwitz_matrix, range(0, index +1), range(0, index +1));
        if(auxilary_matrix.GetDeterminant() < 0)
        {
            return false;
        }
    }
    return true;
}

void DynamicalSystem::operator ()(const vector<double> &x, vector<double> &dxdt, const double /*t*/)
{
    axpy_prod(m_a_matrix, x, dxdt);
//    dxdt += column(m_b_matrix, 0);
    dxdt += m_u_part;
}

void DynamicalSystem::CalculateMatrices()
{
    const double coeff = m_numerator_parameters[m_system_dimension] / m_denominator_parameters[m_system_dimension];
    for(unsigned int i=0;i<m_system_dimension;++i) {
        m_b_matrix(i, 0) =  m_numerator_parameters[i] -
                coeff * m_denominator_parameters[i];
    }

    const unsigned int dec_sys_dim = m_system_dimension - 1;
    m_a_matrix(0, dec_sys_dim) = - m_denominator_parameters[0] /
            m_denominator_parameters[m_system_dimension];
    for(unsigned int i=1;i<m_system_dimension;++i) {
        m_a_matrix(i, i-1) = 1;
        m_a_matrix(i, dec_sys_dim) = - m_denominator_parameters[i] /
                m_denominator_parameters[m_system_dimension];
    }

    m_c_matrix(0, dec_sys_dim) = 1 /
            m_denominator_parameters[m_system_dimension];

    m_d_matrix(0, 0) = m_numerator_parameters[m_system_dimension] /
            m_denominator_parameters[m_system_dimension];

    m_u_part = column(m_b_matrix, 0);

    /*for(unsigned int row_iterator = 1; row_iterator <= m_system_dimension;
        row_iterator++)
    {
        m_b_matrix(row_iterator - 1, 0) = m_numerator_parameters[row_iterator - 1] -
                m_numerator_parameters[m_system_dimension] *
                m_denominator_parameters[row_iterator - 1] /
                m_denominator_parameters[m_system_dimension];
        if(row_iterator != m_system_dimension)
        {
            m_c_matrix(0, row_iterator - 1) = 0;
        }
        else
        {
            m_c_matrix(0, row_iterator - 1) = 1 /
                    m_denominator_parameters[m_system_dimension];
        }
        for(unsigned int column_iterator = 1; column_iterator <= m_system_dimension;
            column_iterator++)
        {
            if(row_iterator == column_iterator + 1)
            {
                m_a_matrix(row_iterator - 1, column_iterator - 1) = 1;
            }
            else if(column_iterator == m_system_dimension)
            {
                m_a_matrix(row_iterator - 1, column_iterator - 1) =
                        - m_denominator_parameters[row_iterator - 1] /
                        m_denominator_parameters[m_system_dimension];
            }
            else
            {
                 m_a_matrix(row_iterator - 1, column_iterator - 1) = 0;
            }
        }
    }
    m_d_matrix(0, 0) = m_numerator_parameters[m_system_dimension] /
            m_denominator_parameters[m_system_dimension];

    m_u_part = column(m_b_matrix, 0);*/
}
