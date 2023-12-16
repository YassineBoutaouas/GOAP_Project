#include "Functors.h"

template<typename T>
TFunctor<T>::TFunctor(const T& defaultVal)
	: m_DefaultValue(defaultVal)
{ }

template<typename T>
TFunctor<T>::~TFunctor() { }

template<typename T>
const T TFunctor<T>::operator()() {
	return m_DefaultValue;
}

BoolFunctor::BoolFunctor(const bool& defaultVal)
	: m_DefaultValue(defaultVal)
{ }

BoolFunctor::~BoolFunctor() { }

const bool BoolFunctor::operator()() {
	return m_DefaultValue;
}

FloatFunctor::FloatFunctor(const float& defaultVal) 
	: m_DefaultValue(defaultVal)
{ }

FloatFunctor::~FloatFunctor() { }

const float FloatFunctor::operator()() {
	return m_DefaultValue;
}