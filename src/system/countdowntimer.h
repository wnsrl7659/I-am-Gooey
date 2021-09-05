/******************************************************************************/
/*!
\file   countdowntimer.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	derived from the timer class, when a certain time limit is hit, executes a function

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <functional>
#include <initializer_list>

#include "../timer/timer.h"

class BaseTimerFunctionWrapper
{
public:
	enum class WrapperType
	{
		Free = 0,
		Member,
	};
	BaseTimerFunctionWrapper(WrapperType t) : type_(t) {};
	virtual ~BaseTimerFunctionWrapper() {};
	WrapperType Type() const
	{
		return type_;
	}
	virtual void Trigger() = 0;
private:
	WrapperType type_;
};

template<typename _ReturnTy, typename _DataTy>
class FreeFunctionWrapper : public BaseTimerFunctionWrapper
{
public:
	FreeFunctionWrapper() : BaseTimerFunctionWrapper(BaseTimerFunctionWrapper::WrapperType::Free) {};
	FreeFunctionWrapper(std::function<_ReturnTy(_DataTy *)> f, _DataTy * d) : BaseTimerFunctionWrapper(BaseTimerFunctionWrapper::WrapperType::Free), data_(d)
	{
		func_ = std::bind(f, d);
	}

	virtual void Trigger()
	{
		func_();
	}

	void SetTrigger(std::function<_ReturnTy(_DataTy *)> f)
	{
		func_ = f;
	}

	void SetData(_DataTy * d)
	{
		data_ = d;
	}

private:
	_DataTy * data_ = nullptr;
	std::function<_ReturnTy()> func_;
};

template<typename _ReturnTy, typename _Class, typename _DataTy>
class MemberFunctionWrapper : public BaseTimerFunctionWrapper
{
public:
	MemberFunctionWrapper() : BaseTimerFunctionWrapper(BaseTimerFunctionWrapper::WrapperType::Member) {};
	MemberFunctionWrapper(_ReturnTy(_Class::*f)(_DataTy *), _Class * c, _DataTy * d) : BaseTimerFunctionWrapper(BaseTimerFunctionWrapper::WrapperType::Member)
	{
		class_ = c;
		func_ = f;
	}

	virtual void Trigger()
	{
		if (class_)
		{
			(class_->*func_)(data_);
		}
	}

	void SetTrigger(std::function<_ReturnTy(_Class const&, _DataTy *)> f)
	{
		func_ = f;
	}

	void SetData(_DataTy * d)
	{
		data_ = d;
	}

	void SetInstance(_Class * c)
	{
		class_ = c;
	}

private:
	_DataTy * data_ = nullptr;
	_Class * class_ = nullptr;
	//std::function<_ReturnTy()> func_;
	_ReturnTy(_Class::*func_)(_DataTy *);
};

class CountdownTimer : public Timer
{
public:
	CountdownTimer() {};

	virtual ~CountdownTimer()
	{
		if (f_)
		{
			delete f_;
			f_ = nullptr;
		}
	}

	void SetLimitSeconds(double seconds)
	{
		limit_ = seconds * 1000.0;
	}

	void SetLimitMilliseconds(double ms)
	{
		limit_ = ms;
	}

	void SetTrigger(BaseTimerFunctionWrapper * w)
	{
		f_ = w;
	}

	double GetLimit() const
	{
		return limit_;
	}

	void Trigger()
	{
		if (f_)
		{
			f_->Trigger();
			if (!repeating_)
			{
				done_ = true;
			}
		}
	}

	void Update()
	{
		if (IsStarted() && !IsPaused())
		{
			if (GetMilliseconds() >= limit_)
			{
				done_ = true;
			}
		}
	}


private:
	double limit_ = 0.0;
	bool repeating_ = false;
	BaseTimerFunctionWrapper * f_ = nullptr;
};

#endif // !COUNTDOWNTIMER_H
