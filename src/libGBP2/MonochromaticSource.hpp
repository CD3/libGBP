#pragma once
#include <optional>

#include <BoostUnitDefinitions/Units.hpp>

#include "./Units.hpp"

namespace libGBP2
{
class MonochromaticSource
{
 private:
  quantity<t::nm>                m_wavelength;
  std::optional<quantity<t::Hz>> m_frequency;
  const quantity<t::m_s_n1>      m_speed_of_light = 299792458 * i::m_s_n1;

 public:
  template<typename U>
  void
  setWavelength(quantity<U> a_val)
  {
    m_wavelength = quantity<t::nm>(a_val);
  }
  template<typename U = t::nm>
  quantity<U>
  getWavelength() const
  {
    return quantity<U>(m_wavelength);
  }

  template<typename U>
  void
  setFrequency(quantity<U> a_val)
  {
    m_frequency = quantity<t::Hz>(a_val);
  }
  template<typename U = t::Hz>
  quantity<U>
  getFrequency() const
  {
    if(m_frequency)
      return quantity<U>(m_frequency.value());
    else
      return quantity<U>(m_speed_of_light / m_wavelength);
  }

  quantity<t::dimensionless>
  getRefractiveIndex() const
  {
    if(m_frequency)
      return quantity<t::dimensionless>(m_speed_of_light / (m_wavelength * m_frequency.value()));
    else
      return quantity<t::dimensionless>::from_value(1);
  }
  void
  setRefractiveIndex(quantity<t::dimensionless> a_val)
  {
    if(!m_frequency)
      m_frequency = quantity<t::Hz>(m_speed_of_light / m_wavelength);

    m_wavelength = quantity<t::nm>(m_speed_of_light / (m_frequency.value() * a_val));
  }
};
}  // namespace libGBP2
