#ifndef Builders_MediaStackBuilder_hpp
#define Builders_MediaStackBuilder_hpp

/** @file MediaStackBuilder.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 07/29/16
  */

#include "Builder.hpp"
#include "MediaBuilder.hpp"

using boost::property_tree::keyIntComp;

/** @class MediaStackBuilder
  * @brief 
  * @author C.D. Clark III
  */
template<typename LengthUnitType>
class MediaStackBuilder : public Builder< MediaStack<LengthUnitType> >
{
  public:

    void configure( MediaStack<LengthUnitType>* stack, const ptree& configTree );

};

template<typename T>
void
MediaStackBuilder<T>::configure( MediaStack<T>* stack, const ptree& configTree )
{
  stack->clear();

  auto mediaConfig = configTree.get_child_optional("media");
  if(!mediaConfig)
    return;

  MediaBuilder<T> builder;
  boost::optional< double > frontPosition, thickness, backPosition;

  // check for a background media
  auto backgroundMedia = mediaConfig.value().get_child_optional("background");
  if( backgroundMedia )
    stack->setBackgroundMedia( Media_ptr<T>( builder.build( backgroundMedia.value() ) ) );

  for( auto iter: getSortedChildren( mediaConfig.value(), keyIntComp, isInt ) )
  {
    frontPosition = iter->second.get_optional<double>("position");
    thickness     = iter->second.get_optional<double>("thickness");
    // check if frontPosition and backPosition are both defined now
    // if so, the back of the previous layer might night match the front
    // of this layer and we need to fill the gap with background.
    //
    // we will just insert the background media and clear the back position
    if(frontPosition && backPosition)
    {
      stack->addBoundary( stack->getBackgroundMedia(), T()*backPosition.value() );
      backPosition = boost::none;
    }
  
    if(!frontPosition)
      frontPosition = backPosition;

    if(!frontPosition)
      frontPosition = 0;

    if(thickness)
      backPosition = frontPosition.value() + thickness.value();
    else
      backPosition = boost::none;


    stack->addBoundary( Media_ptr<T>( builder.build( iter->second ) ), T()*frontPosition.value() );
  }

  // check if a back position was defined. if so, we need to put the background
  // layer on the back.
  if( backPosition )
    stack->addBoundary( stack->getBackgroundMedia(), T()*backPosition.value() );

}


#endif // include protector
