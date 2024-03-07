#pragma once
#include <optional>

#include <BoostUnitDefinitions/Units.hpp>

#include "./Constants.hpp"
#include "./Units.hpp"

namespace libGBP2 {
/**
 * A monochromatic source has a wavelength and a frequency. In vacuum,
 * the wavelength and frequency are related by c = \nu \lambda.
 *
 * In media, the relation is \frac{c}{n} = \nu \lambda. In both cases,
 * the frequency is the same. The _wavelength_ will be shorter in a media
 * than in vaccum. Let \lambda_0 be the wavelength in vacuum. Then
 *
 * Then c = \nu \lambda_0 in vacuum and c/n = \nu \lambda_0 / n = \nu \lambda in
 * media, so \lambda = \lambda_0 / n.
 */
class MonochromaticSource {
private:
  /*
   * We only need to trace 2 of the three parameters frequency, wavelength,
   * refractive index. The user will most often want to set the wavelength, so
   * it makes since to track that. Since frequency does not depend on the media,
   * it makes since to track that too. However, if user just sets the
   * wavelength, it makes sense to assume that we are in vaccuum, so we would
   * need to track whether or not the frequency has been set.
   *
   * Wavelength and frequency have not obvious default value, but refractive
   * index does.
   *
   * So, we can either track wavelength and frequency, and an additional flag to
   * track if both have been set. Or we could track frequency and refractive
   * index, with n = 1 as the default.
   *
   * We are going to track the frequency and refractive index so we don't have
   * to keep an extra flag, or use an optional.
   *
   *
   */
  /* quantity<t::nm>                m_wavelength; */
  /* std::optional<quantity<t::Hz>> m_frequency; */

  quantity<t::Hz> m_frequency;
  quantity<t::dimensionless> m_refractive_index = 1 * i::dimensionless;

public:
  MonochromaticSource() = default;
  ~MonochromaticSource() = default;
  MonochromaticSource(const MonochromaticSource &) = default;
  MonochromaticSource(MonochromaticSource &&) = default;
  MonochromaticSource &operator=(const MonochromaticSource &) = default;
  MonochromaticSource &operator=(MonochromaticSource &&) = default;

  /**
   * Set the vacuum frequency of the wave. This will change the _frequency_
   */
  template <c::Length U> void setVacuumWavelength(quantity<U> a_wavelength) {
    m_frequency = quantity<t::Hz>(constants::speed_of_light / a_wavelength);
  }
  template <c::Length U = t::nm> quantity<U> getVacuumWavelength() const {
    return quantity<U>(constants::speed_of_light / m_frequency);
  }

  /**
   * Set the vacuum frequency of the wave. This will change the _refractive
   * index_, NOT the frequency.
   */
  template <c::Length U> void setWavelength(quantity<U> a_wavelength) {
    this->setVacuumWavelength(a_wavelength * m_refractive_index.value());
  }
  template <typename U = t::nm> quantity<U> getWavelength() const {
    return this->getVacuumWavelength<U>() / m_refractive_index.value();
  }

  template <c::Frequency U> void setFrequency(quantity<U> a_frequency) {
    m_frequency = quantity<t::Hz>(a_frequency);
  }
  template <c::Frequency U = t::Hz> quantity<U> getFrequency() const {
    return quantity<U>(m_frequency);
  }

  template <c::Dimensionless R = t::dimensionless>
  quantity<R> getRefractiveIndex() const {
    return quantity<R>(m_refractive_index);
  }

  template <c::Dimensionless U> void setRefractiveIndex(quantity<U> a_val) {
    m_refractive_index = quantity<t::dimensionless>(a_val);
  }

  /**
   * An overload to allow the user to set the refracrtive index with a double.
   */
  void setRefractiveIndex(double a_val) {
    this->setRefractiveIndex(a_val * i::dimensionless);
  }
};
} // namespace libGBP2
