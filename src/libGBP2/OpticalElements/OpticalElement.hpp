#pragma once
#include <iostream>

#include <boost/units/lambda.hpp>
#include <boost/units/quantity.hpp>

#include <Eigen/Dense>

#include "../Units.hpp"

namespace libGBP2
{

template<c::Length LengthUnit = t::cm>
class OpticalElement
{
 public:
  using L = LengthUnit;
  using K =
      typename boost::units::divide_typeof_helper<t::dimensionless, L>::type;
  using MatrixType = Eigen::Matrix<double, 2, 2>;

 private:
  // we need to track these so we can figure out
  // what the wavelength and beam waist position of
  // the beam emerging from the element are.
  quantity<t::dimensionless> m_refractive_index_scale = 1 * i::dimensionless;
  quantity<L>                m_displacement           = quantity<L>::from_value(0);
  // store the matrix elements
  quantity<t::dimensionless> m_A = 1 * i::dimensionless;
  quantity<L>                m_B = quantity<L>::from_value(0);
  quantity<K>                m_C = quantity<K>::from_value(0);
  quantity<t::dimensionless> m_D = 1 * i::dimensionless;

 public:
  OpticalElement() = default;
  // copy constructor  from element using same unit
  OpticalElement(const OpticalElement &a_other) = default;
  // copy constructor from element using different unit
  template<typename U>
  OpticalElement(const OpticalElement<U> &a_other)
  {
    *this = a_other;
  }
  // assignment from an element using the same unti for length
  OpticalElement &operator=(const OpticalElement &a_other) = default;
  // assignment from an element using a different unit for length
  template<typename U>
  OpticalElement &operator=(const OpticalElement<U> &a_other)
  {
    m_displacement           = a_other.template getDisplacement<L>();
    m_refractive_index_scale = a_other.template getRefractiveIndexScale<t::dimensionless>();
    m_A                      = a_other.template getA<t::dimensionless>();
    m_B                      = a_other.template getB<L>();
    m_C                      = a_other.template getC<K>();
    m_D                      = a_other.template getD<t::dimensionless>();
    return *this;
  }
  OpticalElement(quantity<L>                a_dispalcement,
                 quantity<t::dimensionless> a_refractive_index_scale,
                 MatrixType                 a_mat)
      : m_displacement(a_dispalcement),
        m_refractive_index_scale(a_refractive_index_scale)
  {
    m_A = quantity<t::dimensionless>::from_value(a_mat(0, 0));
    m_B = quantity<L>::from_value(a_mat(0, 1));
    m_C = quantity<K>::from_value(a_mat(1, 0));
    m_D = quantity<t::dimensionless>::from_value(a_mat(1, 1));
  }

  /**
   * Set the displacement that is incurred by the element. i.e.
   * for free space propagation or a thick lens, the q-parameter
   * comming out of the element does not correspond to the same
   * position that the q-parameter going in did.
   */
  template<c::Length U>
  void setDisplacement(quantity<U> a_displacement)
  {
    m_displacement = quantity<L>(a_displacement);
  }

  template<c::Length U = L>
  quantity<U> getDisplacement() const
  {
    return quantity<U>(m_displacement);
  }
  /**
   * Set the refractive index scale induced by the element. i.e.
   * going from water to glass causes the refractive index to
   * scale by ~ 1.5 / 1.3 and the q-parameter comming out of
   * the element will have a different wavelength than the
   * q-parameter going in did.
   */
  template<c::Dimensionless U>
  void setRefractiveIndexScale(quantity<U> a_refractive_index_scale)
  {
    m_refractive_index_scale =
        quantity<t::dimensionless>(a_refractive_index_scale);
  }

  template<c::Dimensionless U = t::dimensionless>
  quantity<U> getRefractiveIndexScale() const
  {
    return quantity<U>(m_refractive_index_scale);
  }

  template<c::Dimensionless U>
  void setA(quantity<U> a_A)
  {
    m_A = quantity<t::dimensionless>(a_A);
  }
  template<c::Length U>
  void setB(quantity<U> a_B)
  {
    m_B = quantity<L>(a_B);
  }
  template<c::InverseLength U>
  void setC(quantity<U> a_C)
  {
    m_C = quantity<K>(a_C);
  }
  template<c::Dimensionless U>
  void setD(quantity<U> a_D)
  {
    m_D = quantity<t::dimensionless>(a_D);
  }
  template<c::Dimensionless U = t::dimensionless>
  quantity<U> getA() const
  {
    return quantity<U>(m_A);
  }
  template<c::Length U = L>
  quantity<U> getB() const
  {
    return quantity<U>(m_B);
  }
  template<c::InverseLength U = K>
  quantity<U> getC() const
  {
    return quantity<U>(m_C);
  }
  template<c::Dimensionless U = t::dimensionless>
  quantity<U> getD() const
  {
    return quantity<U>(m_D);
  }
  /**
   * Return the Ray Transfer Matrix for the element expressed in a given length
   * unit.
   */
  template<c::Length U = L>
  MatrixType getRayTransferMatrix() const
  {
    using INVU =
        typename boost::units::divide_typeof_helper<t::dimensionless, U>::type;
    MatrixType mat;
    mat << m_A.value(), quantity<U>(m_B).value(), quantity<INVU>(m_C).value(),
        m_D.value();
    return mat;
  }

  template<c::Length U>
  OpticalElement<L> operator*(const OpticalElement<U> &a_right) const
  {
    quantity<L> D =
        this->getDisplacement() + a_right.template getDisplacement<L>();
    quantity<t::dimensionless> N =
        this->getRefractiveIndexScale() * a_right.getRefractiveIndexScale();
    auto mat = this->getRayTransferMatrix<L>() *
               a_right.template getRayTransferMatrix<L>();

    return OpticalElement<L>(D, N, mat);
  }

  template<c::Length U>
  quantity<U, std::complex<double>> operator*(quantity<U, std::complex<double>> &a_q) const
  {
    auto                 mat         = this->getRayTransferMatrix<U>();
    std::complex<double> numerator   = a_q.value() * mat(0, 0) + mat(0, 1);
    std::complex<double> denominator = a_q.value() * mat(1, 0) + mat(1, 1);
    std::complex<double> q_prime     = numerator / denominator;

    return quantity<U, std::complex<double>>::from_value(q_prime);
  }
};
}  // namespace libGBP2
