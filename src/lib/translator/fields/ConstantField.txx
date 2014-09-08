/*************************************************************************
 * Dicomifier - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "ConstantField.h"

namespace dicomifier
{
    
namespace translator
{
    
template<DcmEVR VR>
typename ConstantField<VR>::Pointer
ConstantField<VR>
::New()
{
    return Pointer(new Self());
}

template<DcmEVR VR>
typename ConstantField<VR>::Pointer
ConstantField<VR>
::New(ValueType const & value)
{
    return Pointer(new Self(value));
}

template<DcmEVR VR>
typename ConstantField<VR>::Pointer
ConstantField<VR>
::New(ArrayType const & array)
{
    return Pointer(new Self(array));
}
    
template<DcmEVR VR>
ConstantField<VR>
::ConstantField()
    :SubTag<VR>()
{
    // Nothing to do
}

template<DcmEVR VR>
ConstantField<VR>
::ConstantField(ValueType const & value)
    :SubTag<VR>()
{
    this->_array = { value };
}

template<DcmEVR VR>
ConstantField<VR>
::ConstantField(ArrayType const & value)
    :SubTag<VR>()
{
    this->_array = value;
}

template<DcmEVR VR>
ConstantField<VR>
::~ConstantField()
{
    // Nothing to do
}

template<DcmEVR VR>
void
ConstantField<VR>
::run(DcmDataset* dataset,
      dicomifier::bruker::BrukerDataset* brukerdataset)
{
    // Nothing to do
}

} // namespace translator
    
} // namespace dicomifier