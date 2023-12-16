#pragma once

template<typename T>
class TFunctor {
public:
	TFunctor(const TFunctor&) = delete;
	TFunctor(TFunctor&&) noexcept = default;
	TFunctor(const T& defaultVal);
	virtual ~TFunctor();

	const virtual T operator()();

	TFunctor& operator=(TFunctor&& other) noexcept = default;

protected:
	const T m_DefaultValue;
};

class BoolFunctor {
public:
	BoolFunctor(const BoolFunctor&) = delete;
	BoolFunctor(BoolFunctor&&) noexcept = default;
	BoolFunctor(const bool& defaultVal);
	virtual ~BoolFunctor();

	const virtual bool operator()();

	BoolFunctor& operator=(BoolFunctor&& other) noexcept = default;

protected:
	const bool m_DefaultValue;
};

class FloatFunctor {
public:
	FloatFunctor(const FloatFunctor&) = delete;
	FloatFunctor(FloatFunctor&&) noexcept = default;
	FloatFunctor(const float& defaultVal);
	virtual ~FloatFunctor();

	const virtual float operator()();

	FloatFunctor& operator=(FloatFunctor&& other) noexcept = default;

protected:
	const float m_DefaultValue;
};